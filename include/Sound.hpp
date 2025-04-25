#pragma once

#include <SFML/Audio.hpp>
#include <string>

#include "UveBase.hpp"

namespace UveDX {
class Sound : public UveBase {
 public:
  Sound(UveDX* uveDX, const std::string& filename);
  ~Sound();

  void update() override;
  void play();
  void stop();
  bool isPlaying();
  void setPan(int pan);

 private:
  sf::SoundBuffer* buffer;
  sf::Sound* sound;
};
}  // namespace UveDX
