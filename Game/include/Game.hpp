#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "GameController.hpp"
#include "Messenger.hpp"
#include "UveDX/Font.hpp"
#include "UveDX/Sound.hpp"
#include "UveDX/Surface.hpp"
#include "UveDX/SurfaceChain.hpp"
#include "UveDX/UveDX.hpp"
#include "UveDX/UveListOwner.hpp"

#ifdef WITH_DISCORD

#include "DiscordRichPresence.hpp"

#endif

class Game {
 public:
  Game();
  ~Game();

  void init();
  void paint();
  void loadGameResources();
  void playIntro();
  void playSound(UveDX::Sound* sound, int x);
  void drawProgressBar(
      int startX,
      int startY,
      unsigned int progressWidth,
      unsigned int totalWidth,
      unsigned int barHeight
  );
  bool handlePauseScreen();
  void renderPauseOverlay();

  UveDX::UveDX* uveDX;
  UveDX::UveListOwner* fontsList;
  UveDX::UveListOwner* surfacesAndChainsList;
  UveDX::UveListOwner* soundsList;
  bool isLoaded;
  bool isPainted;
  bool isInitialized;
  UveDX::Surface* surface_salmonsky;
  UveDX::SurfaceChain* surface_chain_chickenb;
  UveDX::SurfaceChain* surface_chain_chicken_attack;
  UveDX::SurfaceChain* surface_chain_fighter;
  UveDX::SurfaceChain* surface_chain_sparc;
  UveDX::SurfaceChain* surface_chain_crystal;
  UveDX::SurfaceChain* surface_chain_smoke;
  UveDX::SurfaceChain* surface_chain_missile;
  UveDX::SurfaceChain* surface_chain_exhaust;
  UveDX::SurfaceChain* surface_chain_shield;
  UveDX::SurfaceChain* surface_chain_egg;
  UveDX::SurfaceChain* surface_chain_chicken_leg;
  UveDX::SurfaceChain* surface_chain_gift_box;
  UveDX::SurfaceChain* surface_chain_asteroid_big_rock;
  UveDX::SurfaceChain* surface_chain_asteroid_small_rock;
  UveDX::SurfaceChain* surface_chain_asteroid_big_fire;
  UveDX::SurfaceChain* surface_chain_asteroid_small_fire;
  UveDX::SurfaceChain* surface_chain_feather;
  UveDX::Surface* surface_earth;
  UveDX::Surface* surface_heart;
  UveDX::SurfaceChain* surface_chain_burger;
  UveDX::SurfaceChain* surface_chain_talkbubble;
  UveDX::Surface* surface_logo_small;
  UveDX::SurfaceChain* surface_chain_chicken_boss;
  UveDX::Sound* sound_2001;
  UveDX::Sound* sound_space;
  UveDX::Sound* sound_bonewah;
  UveDX::Sound* sound_fanfar1;
  UveDX::Sound* sound_fx11;
  UveDX::Sound* sound_fx110;
  UveDX::Sound* sound_rdfx31;
  UveDX::Sound* sound_rdfx31_boss;
  UveDX::Sound* sound_tr3_239;
  UveDX::Sound* sound_cannonfire;
  UveDX::Sound* sound_fx113;
  UveDX::Sound* sound_chomp;
  UveDX::Sound* sound_gpolice_12;
  UveDX::Sound* sound_harley1;
  GameController* gameController;
  Messenger* messenger;
  sf::Color color_lightBlue;
  sf::Color color_white;
  UveDX::Font* font_alphabet;
  UveDX::Font* font_alphabet_small;
  UveDX::Font* font_3x7;
  bool shouldQuit;

#ifdef WITH_DISCORD
  DiscordRichPresence* discordRichPresence;
#endif
};

namespace global {
inline std::unique_ptr<Game> game = nullptr;
}
