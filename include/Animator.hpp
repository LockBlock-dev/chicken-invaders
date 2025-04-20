#pragma once

#include "Sound.hpp"
#include "UveBase.hpp"
#include <fstream>
#include <string>
#include <vector>

class Animator : public UveDX::UveBase {
public:
  Animator(UveDX::UveDX *uveDX, const std::string &filename,
           unsigned int frameDataBufferSize);
  ~Animator();

  void update() override;
  void loadFromFile(const std::string &filename);
  void process();
  void convertRGB555toRGB565(char *frameDataBuffer);

  int field_14;
  bool isFinished;
  int offsetX;
  int offsetY;
  unsigned int framerate;
  std::vector<UveDX::Sound *> sounds;
  std::fstream fileStream;
  char *frameDataBuffer;
  char *frameDataBufferClone;
  unsigned int width;
  unsigned int height;
  unsigned int frameDataBufferSize;
};
