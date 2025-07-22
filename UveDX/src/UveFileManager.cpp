#include "UveDX/UveFileManager.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <iosfwd>
#include <print>
#include <string>
#include <vector>

#include "UveDX/UveDX.hpp"

namespace UveDX {
struct AssetEntry {
  std::string name;
  std::string path;
  uint32_t offset;
  uint32_t size;
};

UveFileManager::UveFileManager(UveDX* uveDX)
    : UveBase(uveDX),
      loaded(false),
      filename(""),
      bypassAssetsList(false),
      list({}) {
  this->loadAssetsList("");
}

UveFileManager::~UveFileManager() {
  for (auto assetFileHeader : this->list)
    delete assetFileHeader;
}

void UveFileManager::update() {}

void UveFileManager::loadAssetsList(const std::string& path) {
  this->filename = path;

  if (path == "") {
    this->loaded = false;
    return;
  }

  this->checkFile(this->filename);

  std::fstream s{this->filename, std::ios::binary | std::ios::in};

  if (!s.is_open())
    throw std::runtime_error(
        "UveFileManager::loadAssetsList: Cannot open file"
    );

  unsigned int count = 0;

  s.read(reinterpret_cast<char*>(&count), sizeof(count));

  for (size_t i = 0; i < count; i++) {
    AssetFileHeader* assetFileHeader = new AssetFileHeader{};

    s.read(reinterpret_cast<char*>(assetFileHeader), sizeof(AssetFileHeader));

    this->list.push_back(assetFileHeader);
  }

  this->loaded = true;
}

std::fstream UveFileManager::openFile(
    const std::string& path,
    unsigned int* outFileSize
) {
  std::fstream s;

  this->checkFile(path);

  if (this->loaded) {
    s.open(this->filename, std::ios::binary | std::ios::in);

    for (auto assetFileHeader : this->list) {
      if (assetFileHeader->filename == path) {
        if (outFileSize)
          *outFileSize = assetFileHeader->size;

        s.seekg(assetFileHeader->offset, std::ios::beg);

        break;
      }
    }
  } else {
    s.open(path, std::ios::binary | std::ios::in);

    if (outFileSize)
      *outFileSize =
          static_cast<unsigned int>(std::filesystem::file_size(path));
  }

  if (this->bypassAssetsList) {
    AssetFileHeader* header = new AssetFileHeader{};

    std::strncpy(header->filename, path.c_str(), sizeof(header->filename) - 1);

    header->filename[sizeof(header->filename) - 1] = '\0';
    header->size = static_cast<unsigned int>(std::filesystem::file_size(path));
    header->offset = 0;

    this->loaded = true;

    if (!this->checkFileExists(path))
      this->list.push_back(header);

    this->loaded = false;
  }

  return s;
}

void UveFileManager::checkFile(const std::string& path) {
  if (!this->checkFileExists(path))
    this->uveDX->onError(
        "UveFileManager::checkFile()", std::format("Cannot open file {}", path)
    );
}

bool UveFileManager::checkFileExists(const std::string& path) {
  if (this->loaded) {
    for (auto assetFileHeader : this->list) {
      if (assetFileHeader->filename == path)
        return true;
    }

    return false;
  }

  return this->verifyFileExists(path);
}

bool UveFileManager::verifyFileExists(const std::string& path) {
  return std::filesystem::exists(path);
}

void UveFileManager::createWADFile(
    const std::string& assetsFolderPath,
    const std::string& outputFilePath
) {
  std::filesystem::path path{assetsFolderPath};

  if (!std::filesystem::is_directory(path))
    std::println(
        "UveFileManager::createWADFile: {} is not a directory!",
        assetsFolderPath
    );

  std::filesystem::path outputPath{outputFilePath};

  if (outputPath.extension() != ".dat")
    outputPath.replace_extension(".dat");

  std::ofstream s{outputPath, std::ios::binary | std::ios::out};
  std::vector<AssetEntry> assetEntries{};

  uint32_t zero = 0;
  AssetFileHeader dummyAssetFileHeader = {};
  s.write(reinterpret_cast<char*>(&zero), sizeof(uint32_t));

  for (auto const& entry :
       std::filesystem::recursive_directory_iterator(path)) {
    if (!entry.is_regular_file())
      continue;

    auto& fullPath = entry.path();
    auto relativePath = std::filesystem::relative(fullPath, path);

    AssetEntry assetEntry{
        relativePath.string(),
        fullPath.string(),
        0,
        static_cast<uint32_t>(entry.file_size()),
    };

    assetEntries.push_back(assetEntry);

    s.write(
        reinterpret_cast<char*>(&dummyAssetFileHeader), sizeof(AssetFileHeader)
    );
  }

  for (auto& entry : assetEntries) {
    std::ifstream entryFile{entry.path, std::ios::binary | std::ios::in};
    std::vector<char> buffer(entry.size);

    entryFile.read(buffer.data(), entry.size);

    entry.offset = s.tellp();

    s.write(buffer.data(), entry.size);
  }

  s.seekp(0, std::ios::beg);

  uint32_t count = assetEntries.size();
  s.write(reinterpret_cast<char*>(&count), sizeof(uint32_t));

  for (auto const& entry : assetEntries) {
    AssetFileHeader header{
        "\0",
        entry.offset,
        entry.size,
    };

    std::strncpy(
        header.filename, entry.name.c_str(), sizeof(header.filename) - 1
    );

    header.filename[sizeof(header.filename) - 1] = '\0';

    s.write(reinterpret_cast<char*>(&header), sizeof(AssetFileHeader));
  }
}
}  // namespace UveDX
