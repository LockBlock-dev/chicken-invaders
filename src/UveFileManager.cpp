#include "UveFileManager.hpp"
#include "UveDX.hpp"
#include <cstring>
#include <filesystem>

namespace UveDX {
std::streampos getFileSize(std::fstream &file) {
  std::streampos currentPos = file.tellg();

  file.seekg(0, std::ios::end);

  std::streampos fileSize = file.tellg();

  file.seekg(currentPos);

  return fileSize;
}

UveFileManager::UveFileManager(UveDX *uveDX)
    : UveBase(uveDX), loaded(false), filename(""), bypassAssetsList(false),
      list({}) {
  this->loadAssetsList("");
}

UveFileManager::~UveFileManager() {
  for (auto assetFileHeader : this->list)
    delete assetFileHeader;
}

void UveFileManager::update() {}

void UveFileManager::loadAssetsList(const std::string &path) {
  this->filename = path;

  if (path == "") {
    this->loaded = false;
    return;
  }

  this->checkFile(this->filename);

  std::fstream s{this->filename, s.binary | s.in};

  if (!s.is_open())
    throw std::runtime_error(
        "UveFileManager::loadAssetsList: Cannot open file");

  unsigned int count = 0;

  s.read(reinterpret_cast<char *>(&count), sizeof(count));

  for (size_t i = 0; i < count; i++) {
    AssetFileHeader *assetFileHeader = new AssetFileHeader{};

    s.read(reinterpret_cast<char *>(assetFileHeader), sizeof(AssetFileHeader));

    this->list.push_back(assetFileHeader);
  }

  this->loaded = true;
}

std::fstream UveFileManager::openFile(const std::string &path,
                                      unsigned int *outFileSize) {
  std::fstream s;

  this->checkFile(path);

  if (this->loaded) {
    s.open(this->filename, s.binary | s.in);

    for (auto assetFileHeader : this->list) {
      if (assetFileHeader->filename == path) {
        if (outFileSize)
          *outFileSize = assetFileHeader->size;

        s.seekg(assetFileHeader->offset, std::ios::beg);

        break;
      }
    }
  } else {
    s.open(filename, s.binary | s.in);

    if (outFileSize)
      *outFileSize = static_cast<unsigned int>(getFileSize(s));
  }

  if (this->bypassAssetsList) {
    AssetFileHeader *header = new AssetFileHeader;

    std::strncpy(header->filename, path.c_str(), sizeof(header->filename));

    header->size = static_cast<unsigned int>(getFileSize(s));
    header->offset = 0;

    this->loaded = true;

    if (!this->checkFileExists(path))
      this->list.push_back(header);

    this->loaded = false;
  }

  return s;
}

void UveFileManager::checkFile(const std::string &path) {
  if (!this->checkFileExists(path))
    this->uveDX->onError("UveFileManager::checkFile()",
                         std::format("Cannot open file {}", path));
}

bool UveFileManager::checkFileExists(const std::string &path) {
  if (this->loaded) {
    for (auto assetFileHeader : this->list) {
      if (assetFileHeader->filename == path)
        return true;
    }

    return false;
  }

  return this->verifyFileExists(path);
}

bool UveFileManager::verifyFileExists(const std::string &path) {
  return std::filesystem::exists(path);
}
} // namespace UveDX
