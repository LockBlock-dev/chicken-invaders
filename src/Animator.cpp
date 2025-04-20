#include "Animator.hpp"
#include "Sound.hpp"
#include "UveDX.hpp"
#include <cstdint>

Animator::Animator(UveDX::UveDX *uveDX, const std::string &filename,
                   unsigned int frameDataBufferSize)
    : UveDX::UveBase(uveDX), field_14(0), isFinished(false), offsetX(0),
      offsetY(0), framerate(0), sounds({}), fileStream(),
      frameDataBuffer(nullptr), width(0), height(0),
      frameDataBufferSize(frameDataBufferSize) {
  if (filename != "")
    this->loadFromFile(filename);
}

Animator::~Animator() {
  for (auto sound : this->sounds)
    if (sound)
      delete sound;
}

void Animator::update() {}

void Animator::loadFromFile(const std::string &filename) {
  this->fileStream = this->uveDX->uveFileManager->openFile(filename, nullptr);

  this->fileStream.read(reinterpret_cast<char *>(&this->width),
                        sizeof(this->width));
  this->fileStream.read(reinterpret_cast<char *>(&this->height),
                        sizeof(this->height));
  this->fileStream.read(reinterpret_cast<char *>(&this->framerate),
                        sizeof(this->framerate));

  if (!this->frameDataBufferSize)
    this->frameDataBufferSize =
        4 * this->height + (this->height + 1) * (this->width + 1);

  // this->frameDataBuffer = static_cast<char *>(std::std::malloc(2 *
  // this->frameDataBufferSize));
  this->frameDataBuffer = new char[2 * this->frameDataBufferSize];

  this->frameDataBufferSize *= 2;
}

void Animator::process() {
  if (!this->fileStream)
    return;

  unsigned int interactionType = 0;

  while (true) {
    this->fileStream.read(reinterpret_cast<char *>(&interactionType),
                          sizeof(interactionType));

    switch (interactionType) {
    case 0: {
      delete[] this->frameDataBuffer;
      this->isFinished = true;
      this->fileStream.close();
      break;
    }
    case 1: {
      unsigned int frameSize = 0;
      this->fileStream.read(reinterpret_cast<char *>(&frameSize),
                            sizeof(frameSize));

      if (frameSize >= this->frameDataBufferSize)
        this->uveDX->onError("Animator::process()",
                             "Frame size bigger than buffer!", frameSize);

      this->fileStream.read(this->frameDataBuffer, frameSize);

      // auto frameDataBufferCopy = this->frameDataBuffer;

      if (this->uveDX->pixelFormat == 565)
        this->convertRGB555toRGB565(frameDataBuffer);

      // ToDo draw
      // ToDo (0x00415968)
      /*
      unsigned int surfaceRowPitch = this->width * 2;
      char *lpSurface = reinterpret_cast<char *>(0);
      lpSurface = lpSurface + 2 * this->offsetX + 2 * surfaceRowPitch *
      this->offsetY; char *pixelPointer; char *rowStart; unsigned int rowIndex =
      0; for (pixelPointer = lpSurface;;pixelPointer = &rowStart[rowIndex]) {
          uint16_t pixelCount = 0;

          while (true) {
              pixelCount = *reinterpret_cast<uint16_t *>(frameDataBufferCopy);

              frameDataBufferCopy += 2;

              if (pixelCount <= 0x8000)
                  break;

              lpSurface += 2 * surfaceRowPitch * (0x10000 - pixelCount);
              pixelPointer = lpSurface;
          }

          if (pixelCount == 0x8000)
              break;

          rowStart = &pixelPointer[2 * pixelCount];

          auto bytesToCopy = *reinterpret_cast<uint16_t *>(frameDataBufferCopy);

          frameDataBufferCopy += 2;

          // std::memcpy(rowStart, frameDataBufferCopy, 2 * bytesToCopy);

          rowIndex = 2 * bytesToCopy;
          frameDataBufferCopy += bytesToCopy;
      }
      */

      this->uveDX->backSurface->blitWithColor();

      break;
    }
    case 2: {
      unsigned int sleepTime = 0;
      this->fileStream.read(reinterpret_cast<char *>(&sleepTime),
                            sizeof(sleepTime));

      this->uveDX->waitForEscapeKey(sleepTime);

      continue;
    }
    case 3: {
      char filename[20];

      this->fileStream.read(filename, sizeof(filename));

      this->fileStream.seekg(256 - 20, std::ios::cur);

      this->sounds.push_back(
          new UveDX::Sound(this->uveDX, std::string{filename}));

      continue;
    }
    case 4: {
      unsigned int idx = 0;

      this->fileStream.read(reinterpret_cast<char *>(&idx), sizeof(idx));
      this->fileStream.seekg(4, std::ios::cur);

      auto sound = this->sounds.at(idx);

      if (this->uveDX->soundEnabled) {
        sound->play();
      }
      continue;
    }
    case 5: {
      unsigned int idx = 0;

      this->fileStream.read(reinterpret_cast<char *>(&idx), sizeof(idx));

      auto sound = this->sounds.at(idx);

      if (this->uveDX->soundEnabled) {
        sound->stop();
      }
      continue;
    }
    default:
      continue;
    }

    break;
  }
}

void Animator::convertRGB555toRGB565(char *frameDataBuf) {
  uint16_t frameType = 0;
  uint16_t repeatCount = 0;

  while (true) {
    do {
      frameType = *reinterpret_cast<uint16_t *>(frameDataBuf);

      frameDataBuf += 2;
    } while (frameType > 0x8000u);

    if (frameType == 0x8000)
      break;

    repeatCount = *reinterpret_cast<uint16_t *>(frameDataBuf);

    frameDataBuf += 2;

    for (size_t i = 0; i < repeatCount; ++i) {
      uint16_t *buf = reinterpret_cast<uint16_t *>(frameDataBuf);

      uint16_t redGreenMasked =
          *buf & 0xFFE0; // Mask for red and green (top 11 bits)
      uint16_t blueMasked =
          *buf & 0x001F; // Mask for blue (5 least significant bits)

      *buf = (2 * redGreenMasked) | blueMasked;

      frameDataBuf += 2;
    }
  }
}
