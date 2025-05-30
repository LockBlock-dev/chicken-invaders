#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "UveDX/UveBase.hpp"

namespace UveDX {
struct AssetFileHeader {
  char filename[80];
  unsigned int offset;
  unsigned int size;
};

class UveFileManager : public UveBase {
 public:
  UveFileManager(UveDX* uveDX);
  ~UveFileManager();

  void update() override;
  void loadAssetsList(const std::string& path);
  std::fstream openFile(const std::string& path, unsigned int* outFileSize);
  void checkFile(const std::string& path);
  bool checkFileExists(const std::string& path);
  bool verifyFileExists(const std::string& path);

 private:
  bool loaded;
  std::string filename;
  bool bypassAssetsList;
  std::vector<AssetFileHeader*> list;
};
}  // namespace UveDX
