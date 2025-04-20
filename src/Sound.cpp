#include "Sound.hpp"
#include "UveDX.hpp"
#include <cstring>
#include <format>

namespace UveDX {
Sound::Sound(UveDX *uveDX, const std::string &filename)
    : UveBase(uveDX), buffer(nullptr), sound(nullptr) {
  if (this->uveDX->uveSound->isReady()) {
    auto fileStream = this->uveDX->uveFileManager->openFile(filename, nullptr);

    char wavFileHeader[44];

    fileStream.read(wavFileHeader, sizeof(wavFileHeader));

    unsigned int fileSize =
        *reinterpret_cast<unsigned int *>(&wavFileHeader[40]);

    this->uveDX->log(
        std::format("Loading sound file [{}], {} bytes.", filename, fileSize));

    auto wavFile = new char[sizeof(wavFileHeader) + fileSize];

    std::memcpy(wavFile, wavFileHeader, sizeof(wavFileHeader));

    fileStream.read(wavFile + sizeof(wavFileHeader), fileSize);

    this->buffer =
        new sf::SoundBuffer{wavFile, sizeof(wavFileHeader) + fileSize};

    this->sound = new sf::Sound{*this->buffer};

    delete[] wavFile;

    // throw std::runtime_error("Sound::Sound(): Cannot create sound");
  } else {
    this->uveDX->log("Warning: audio device not ready, skipping sound load");
  }
}

Sound::~Sound() {
  if (this->sound)
    delete this->sound;

  if (this->buffer)
    delete this->buffer;
}

void Sound::update() {}

void Sound::play() { this->sound->play(); }

void Sound::stop() { this->sound->stop(); }

bool Sound::isPlaying() {
  if (!this->uveDX->soundEnabled)
    return false;

  return this->sound->getStatus() == sf::SoundSource::Status::Playing;
}

void Sound::setPan(int pan) {
  this->sound->setPan(static_cast<float>(pan) / 10000.0f);
}
} // namespace UveDX
