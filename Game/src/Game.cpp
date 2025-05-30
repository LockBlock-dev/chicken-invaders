#include "Game.hpp"

#include <cmath>

#include "Background.hpp"
#include "DelaySprite.hpp"
#include "DelayText.hpp"
#include "IntroChicken.hpp"
#include "UveDX/Sprite.hpp"
#include "UveDX/UveTimer.hpp"
#include "constants.hpp"
#include "trigonometry.hpp"

Game::Game()
    : uveDX(nullptr),
      fontsList(nullptr),
      surfacesAndChainsList(nullptr),
      soundsList(nullptr),
      isLoaded(false),
      isPainted(false),
      isInitialized(false),
      surface_salmonsky(nullptr),
      surface_chain_chickenb(nullptr),
      surface_chain_chicken_attack(nullptr),
      surface_chain_fighter(nullptr),
      surface_chain_sparc(nullptr),
      surface_chain_crystal(nullptr),
      surface_chain_smoke(nullptr),
      surface_chain_missile(nullptr),
      surface_chain_exhaust(nullptr),
      surface_chain_shield(nullptr),
      surface_chain_egg(nullptr),
      surface_chain_chicken_leg(nullptr),
      surface_chain_gift_box(nullptr),
      surface_chain_asteroid_big_rock(nullptr),
      surface_chain_asteroid_small_rock(nullptr),
      surface_chain_asteroid_big_fire(nullptr),
      surface_chain_asteroid_small_fire(nullptr),
      surface_chain_feather(nullptr),
      surface_earth(nullptr),
      surface_heart(nullptr),
      surface_chain_burger(nullptr),
      surface_chain_talkbubble(nullptr),
      surface_logo_small(nullptr),
      surface_chain_chicken_boss(nullptr),
      sound_2001(nullptr),
      sound_space(nullptr),
      sound_bonewah(nullptr),
      sound_fanfar1(nullptr),
      sound_fx11(nullptr),
      sound_fx110(nullptr),
      sound_rdfx31(nullptr),
      sound_rdfx31_boss(nullptr),
      sound_tr3_239(nullptr),
      sound_cannonfire(nullptr),
      sound_fx113(nullptr),
      sound_chomp(nullptr),
      sound_gpolice_12(nullptr),
      sound_harley1(nullptr),
      gameController(nullptr),
      messenger(nullptr),
      // original game uses BGR 0xFFDC00
      color_lightBlue(0x00DCFFFF),
      color_white(sf::Color::White),
      font_alphabet(nullptr),
      font_alphabet_small(nullptr),
      font_3x7(nullptr),
      shouldQuit(false) {}

Game::~Game() {
  if (this->uveDX) {
    delete this->soundsList;
    delete this->surfacesAndChainsList;
    delete this->fontsList;
    delete this->uveDX;
  }
}

void Game::init() {
  if (!this->uveDX) {
    this->uveDX = new UveDX::UveDX{
        false, constants::screenWidth, constants::screenHeight, 16
    };

    this->uveDX->timer.setInterval(38);

    UveDX::UveSound::create(this->uveDX);
    UveDX::UveInput::create(
        this->uveDX, UveDX::UveInput::AcquireDeviceFlag::ACQUIRE_KEYBOARD_FLAG
    );

    this->uveDX->uveFileManager->loadAssetsList("ChickenInvaders.dat");

    this->fontsList = new UveDX::UveListOwner{this->uveDX};
    this->surfacesAndChainsList = new UveDX::UveListOwner{this->uveDX};
    this->soundsList = new UveDX::UveListOwner{this->uveDX};

    this->isInitialized = true;
  }
}

void Game::paint() {
  if (!this->isInitialized || this->isPainted)
    return;

  this->isPainted = true;

  this->uveDX->log("Entering Form::Paint()");

  if (!this->isLoaded) {
#ifdef WITH_DISCORD
    this->discordRichPresence = new DiscordRichPresence{this->uveDX};

    if (this->discordRichPresence->isInitialized())
      this->uveDX->add(this->discordRichPresence);
#endif

    this->loadGameResources();

    this->isLoaded = true;

    this->playIntro();

    this->uveDX->clear();

    this->uveDX->yOffset = 0;

    this->uveDX->uveInput->waitForAllKeysRelease();

    this->gameController = new GameController{this->uveDX};
    this->uveDX->add(this->gameController);

    this->messenger = new Messenger{this->uveDX};
    this->uveDX->add(this->messenger);
  }

  while (!this->shouldQuit) {
    this->uveDX->update();
  }

  this->uveDX->backSurface->blitWithColor();

  this->font_alphabet_small->blitText(
      320, 180, "did you know...", UveDX::Font::TextAlignment::Center
  );
  this->font_alphabet_small->blitText(
      320, 210, "you can compare yourself against",
      UveDX::Font::TextAlignment::Center
  );
  this->font_alphabet_small->blitText(
      320, 230, "people from all over the world",
      UveDX::Font::TextAlignment::Center
  );
  this->font_alphabet_small->blitText(
      320, 250, "by uploading your high scores",
      UveDX::Font::TextAlignment::Center
  );
  this->font_alphabet_small->blitText(
      320, 270, "to the chicken invaders internet high score tables!",
      UveDX::Font::TextAlignment::Center
  );

  this->uveDX->frame();

  this->uveDX->waitForEscapeKey(10000);

  this->uveDX->backSurface->blitWithColor();

  this->font_alphabet_small->blitText(
      320, 180,
      "the chickens are back and they're taking over the solar system!",
      UveDX::Font::TextAlignment::Center
  );
  this->font_alphabet_small->blitText(
      320, 200, "continue your adventure in", UveDX::Font::TextAlignment::Center
  );

  this->uveDX->backSurface->loadBMP("gfx/ci2.bmp", 0xFFFF8000, 230);

  this->font_alphabet_small->blitText(
      320, 270, "the next wave!", UveDX::Font::TextAlignment::Center
  );

  this->uveDX->frame();

  this->uveDX->waitForEscapeKey(10000);

  this->uveDX->backSurface->blitWithColor();

  this->font_alphabet_small->blitText(
      320, 190, "visit", UveDX::Font::TextAlignment::Center
  );
  this->font_alphabet->blitText(
      320, 220, "www.interactionstudios.com", UveDX::Font::TextAlignment::Center
  );
  this->font_alphabet_small->blitText(
      320, 265, "for more information", UveDX::Font::TextAlignment::Center
  );

  this->uveDX->frame();

  this->uveDX->waitForEscapeKey(60000);
}

void Game::loadGameResources() {
  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 4, 200, 20);
  this->uveDX->frame();

  for (size_t i = 0; i < 256; ++i) {
    double angle =
        HALF_PI_APPROX - static_cast<double>(i) * TWO_PI * INVERSE_OF_256;

    global::dsin.at(i) = -std::sin(angle);
    global::dcos.at(i) = std::cos(angle);
  }

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 8, 200, 20);
  this->uveDX->frame();

  this->surface_salmonsky = new UveDX::Surface{
      this->uveDX,
      "gfx/salmonsky.bmp",
  };
  this->surfacesAndChainsList->add(this->surface_salmonsky);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 12, 200, 20);
  this->uveDX->frame();

  this->surface_salmonsky->setAnchorPoint(SurfaceAnchorType::TopLeftDefault);

  this->surface_chain_chickenb = new UveDX::SurfaceChain{
      this->uveDX,
      "gfx/chickenb",
  };
  this->surfacesAndChainsList->add(this->surface_chain_chickenb);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 16, 200, 20);
  this->uveDX->frame();

  this->surface_chain_chickenb->scaleAllSurfaces(0.75);

  this->surface_chain_chicken_attack = new UveDX::SurfaceChain{
      this->uveDX,
      "gfx/chicken-attack",
  };
  this->surfacesAndChainsList->add(this->surface_chain_chicken_attack);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 20, 200, 20);
  this->uveDX->frame();

  this->surface_chain_chicken_attack->scaleAllSurfaces(0.75);

  this->surface_chain_fighter = new UveDX::SurfaceChain{
      this->uveDX,
      "gfx/fighter",
  };
  this->surfacesAndChainsList->add(this->surface_chain_fighter);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 24, 200, 20);
  this->uveDX->frame();

  this->surface_chain_fighter->scaleAllSurfaces(0.5);

  this->surface_chain_sparc = new UveDX::SurfaceChain{
      this->uveDX,
      "gfx/sparc",
  };
  this->surfacesAndChainsList->add(this->surface_chain_sparc);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 28, 200, 20);
  this->uveDX->frame();

  this->surface_chain_crystal = new UveDX::SurfaceChain{
      this->uveDX,
      "gfx/crystal",
  };
  this->surfacesAndChainsList->add(this->surface_chain_crystal);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 32, 200, 20);
  this->uveDX->frame();

  this->surface_chain_crystal->scaleAllSurfaces(0.25);

  this->surface_chain_smoke = new UveDX::SurfaceChain{
      this->uveDX,
      "gfx/smoke",
  };
  this->surfacesAndChainsList->add(this->surface_chain_smoke);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 36, 200, 20);
  this->uveDX->frame();

  this->surface_chain_missile = new UveDX::SurfaceChain{
      this->uveDX,
      "gfx/missile",
  };
  this->surfacesAndChainsList->add(this->surface_chain_missile);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 40, 200, 20);
  this->uveDX->frame();

  this->surface_chain_exhaust = new UveDX::SurfaceChain{
      this->uveDX,
      "gfx/exhaust",
  };
  this->surfacesAndChainsList->add(this->surface_chain_exhaust);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 44, 200, 20);
  this->uveDX->frame();

  this->surface_chain_exhaust->applyAnchorPointToAllSurfaces(
      SurfaceAnchorType::TopCenter
  );

  this->surface_chain_shield = new UveDX::SurfaceChain{
      this->uveDX,
      "gfx/shield",
  };
  this->surfacesAndChainsList->add(this->surface_chain_shield);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 48, 200, 20);
  this->uveDX->frame();

  this->surface_chain_egg = new UveDX::SurfaceChain{
      this->uveDX,
      "gfx/egg",
  };
  this->surfacesAndChainsList->add(this->surface_chain_egg);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 52, 200, 20);
  this->uveDX->frame();

  this->surface_chain_egg->linkSurfaces(
      UveDX::SurfaceChain::SurfaceLinkType::Self, 1, -1
  );

  this->surface_chain_chicken_leg = new UveDX::SurfaceChain{
      this->uveDX,
      "gfx/chickenleg",
  };
  this->surfacesAndChainsList->add(this->surface_chain_chicken_leg);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 56, 200, 20);
  this->uveDX->frame();

  this->surface_chain_gift_box = new UveDX::SurfaceChain{
      this->uveDX,
      "gfx/giftbox",
  };
  this->surfacesAndChainsList->add(this->surface_chain_gift_box);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 60, 200, 20);
  this->uveDX->frame();

  this->surface_chain_gift_box->linkSurfaces(
      UveDX::SurfaceChain::SurfaceLinkType::Loop, 0, -1
  );

  this->surface_chain_asteroid_big_rock = new UveDX::SurfaceChain{
      this->uveDX,
      "gfx/asteroid-big-rock",
  };
  this->surfacesAndChainsList->add(this->surface_chain_asteroid_big_rock);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 64, 200, 20);
  this->uveDX->frame();

  this->surface_chain_asteroid_big_rock->linkSurfaces(
      UveDX::SurfaceChain::SurfaceLinkType::Loop, 0, -1
  );

  this->surface_chain_asteroid_small_rock = new UveDX::SurfaceChain{
      this->uveDX,
      "gfx/asteroid-small-rock",
  };
  this->surfacesAndChainsList->add(this->surface_chain_asteroid_small_rock);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 68, 200, 20);
  this->uveDX->frame();

  this->surface_chain_asteroid_small_rock->linkSurfaces(
      UveDX::SurfaceChain::SurfaceLinkType::Loop, 0, -1
  );

  this->surface_chain_asteroid_big_fire = new UveDX::SurfaceChain{
      this->uveDX,
      "gfx/asteroid-big-fire",
  };
  this->surfacesAndChainsList->add(this->surface_chain_asteroid_big_fire);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 72, 200, 20);
  this->uveDX->frame();

  this->surface_chain_asteroid_big_fire->linkSurfaces(
      UveDX::SurfaceChain::SurfaceLinkType::Loop, 0, -1
  );

  this->surface_chain_asteroid_small_fire = new UveDX::SurfaceChain{
      this->uveDX,
      "gfx/asteroid-small-fire",
  };
  this->surfacesAndChainsList->add(this->surface_chain_asteroid_small_fire);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 76, 200, 20);
  this->uveDX->frame();

  this->surface_chain_asteroid_small_fire->linkSurfaces(
      UveDX::SurfaceChain::SurfaceLinkType::Loop, 0, -1
  );

  this->surface_chain_feather = new UveDX::SurfaceChain{
      this->uveDX,
      "gfx/feather",
  };
  this->surfacesAndChainsList->add(this->surface_chain_feather);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 80, 200, 20);
  this->uveDX->frame();

  this->surface_earth = new UveDX::Surface{
      this->uveDX,
      "gfx/earth_0.bmp",
  };
  this->surfacesAndChainsList->add(this->surface_earth);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 84, 200, 20);
  this->uveDX->frame();

  this->surface_heart = new UveDX::Surface{
      this->uveDX,
      "gfx/heart.bmp",
  };
  this->surfacesAndChainsList->add(this->surface_heart);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 88, 200, 20);
  this->uveDX->frame();

  this->surface_chain_burger = new UveDX::SurfaceChain{
      this->uveDX,
      "gfx/burger",
  };
  this->surfacesAndChainsList->add(this->surface_chain_burger);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 92, 200, 20);
  this->uveDX->frame();

  this->surface_chain_burger->linkSurfaces(
      UveDX::SurfaceChain::SurfaceLinkType::Loop, 0, -1
  );
  this->surface_chain_burger->getSurf(0)->setAnimationDelay(10);
  this->surface_chain_burger->getSurf(1)->setAnimationDelay(10);

  this->surface_chain_talkbubble = new UveDX::SurfaceChain{
      this->uveDX,
      "gfx/talkbubble",
  };
  this->surfacesAndChainsList->add(this->surface_chain_talkbubble);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 96, 200, 20);
  this->uveDX->frame();

  this->surface_logo_small = new UveDX::Surface{
      this->uveDX,
      "gfx/logo-small_0.bmp",
  };
  this->surfacesAndChainsList->add(this->surface_logo_small);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 100, 200, 20);
  this->uveDX->frame();

  this->surface_chain_chicken_boss = new UveDX::SurfaceChain{
      this->uveDX,
      "gfx/chicken-boss",
  };
  this->surfacesAndChainsList->add(this->surface_chain_chicken_boss);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 104, 200, 20);
  this->uveDX->frame();

  this->surface_chain_chicken_boss->scaleAllSurfaces(0.75);

  this->sound_2001 = new UveDX::Sound{
      this->uveDX,
      "sfx/2001.wav",
  };
  this->soundsList->add(this->sound_2001);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 108, 200, 20);
  this->uveDX->frame();

  this->sound_space = new UveDX::Sound{
      this->uveDX,
      "sfx/space.wav",
  };
  this->soundsList->add(this->sound_space);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 112, 200, 20);
  this->uveDX->frame();

  this->sound_bonewah = new UveDX::Sound{
      this->uveDX,
      "sfx/bonewah.wav",
  };
  this->soundsList->add(this->sound_bonewah);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 116, 200, 20);
  this->uveDX->frame();

  this->sound_fanfar1 = new UveDX::Sound{
      this->uveDX,
      "sfx/fanfar1.wav",
  };
  this->soundsList->add(this->sound_fanfar1);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 120, 200, 20);
  this->uveDX->frame();

  this->sound_fx11 = new UveDX::Sound{
      this->uveDX,
      "sfx/fx11.wav",
  };
  this->soundsList->add(this->sound_fx11);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 124, 200, 20);
  this->uveDX->frame();

  this->sound_fx110 = new UveDX::Sound{
      this->uveDX,
      "sfx/fx110.wav",
  };
  this->soundsList->add(this->sound_fx110);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 128, 200, 20);
  this->uveDX->frame();

  this->sound_rdfx31 = new UveDX::Sound{
      this->uveDX,
      "sfx/rdfx31.wav",
  };
  this->soundsList->add(this->sound_rdfx31);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 132, 200, 20);
  this->uveDX->frame();

  this->sound_rdfx31_boss = new UveDX::Sound{
      this->uveDX,
      "sfx/rdfx31-boss.wav",
  };
  this->soundsList->add(this->sound_rdfx31_boss);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 136, 200, 20);
  this->uveDX->frame();

  this->sound_tr3_239 = new UveDX::Sound{
      this->uveDX,
      "sfx/tr3_239.wav",
  };
  this->soundsList->add(this->sound_tr3_239);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 140, 200, 20);
  this->uveDX->frame();

  this->sound_cannonfire = new UveDX::Sound{
      this->uveDX,
      "sfx/cannonfire.wav",
  };
  this->soundsList->add(this->sound_cannonfire);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 144, 200, 20);
  this->uveDX->frame();

  this->sound_fx113 = new UveDX::Sound{
      this->uveDX,
      "sfx/fx113.wav",
  };
  this->soundsList->add(this->sound_fx113);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 148, 200, 20);
  this->uveDX->frame();

  this->sound_chomp = new UveDX::Sound{
      this->uveDX,
      "sfx/chomp.wav",
  };
  this->soundsList->add(this->sound_chomp);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 152, 200, 20);
  this->uveDX->frame();

  this->sound_gpolice_12 = new UveDX::Sound{
      this->uveDX,
      "sfx/gpolice_12.wav",
  };
  this->soundsList->add(this->sound_gpolice_12);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 156, 200, 20);
  this->uveDX->frame();

  this->sound_harley1 = new UveDX::Sound{
      this->uveDX,
      "sfx/harley1.wav",
  };
  this->soundsList->add(this->sound_harley1);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 160, 200, 20);
  this->uveDX->frame();

  this->font_alphabet = new UveDX::Font{
      this->uveDX,
      "gfx/alphabet",
  };
  this->fontsList->add(this->font_alphabet);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 164, 200, 20);
  this->uveDX->frame();

  this->font_alphabet_small = new UveDX::Font{
      this->uveDX,
      "gfx/alphabet-s",
  };
  this->fontsList->add(this->font_alphabet_small);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 168, 200, 20);
  this->uveDX->frame();

  this->font_alphabet_small->setSpaceWidth(6);

  this->font_3x7 = new UveDX::Font{this->uveDX, "gfx/font3x7"};
  this->fontsList->add(this->font_3x7);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 172, 200, 20);
  this->uveDX->frame();

  this->font_3x7->setSpaceWidth(2);

  this->uveDX->backSurface->blitWithColor();
  this->drawProgressBar(218, 230, 176, 200, 20);
  this->uveDX->frame();

  for (unsigned int i = 44; i < 50; ++i) {
    this->uveDX->backSurface->blitWithColor();
    this->drawProgressBar(218, 230, 4 * i, 200, 20);
    this->uveDX->frame();
  }
}

void Game::playIntro() {
  this->uveDX->backSurface->blitWithColor();

  this->uveDX->loadAnimations("gfx/interaction.uan", 0, 0);

  if (this->sound_2001->uveDX->soundEnabled)
    this->sound_2001->play();

  this->uveDX->backSurface->blitWithColor();
  this->uveDX->backSurface->loadBMP("gfx/presents.bmp", 0xFFFF8000, 0xFFFF8000);
  this->uveDX->frame();
  this->uveDX->waitForEscapeKey(2000);

  this->uveDX->backSurface->blitWithColor();
  this->uveDX->backSurface->loadBMP("gfx/uveDX.bmp", 0xFFFF8000, 0xFFFF8000);
  this->uveDX->frame();
  this->uveDX->waitForEscapeKey(2000);

  this->uveDX->backSurface->blitWithColor();
  this->uveDX->backSurface->loadBMP("gfx/SStudio.bmp", 0xFFFF8000, 0xFFFF8000);
  this->uveDX->frame();
  this->uveDX->waitForEscapeKey(2000);

  this->uveDX->backSurface->blitWithColor();
  this->uveDX->backSurface->loadBMP(
      "gfx/particleflux.bmp", 0xFFFF8000, 0xFFFF8000
  );
  this->uveDX->frame();
  this->uveDX->waitForEscapeKey(2000);

  this->uveDX->backSurface->blitWithColor();
  this->uveDX->backSurface->loadBMP("gfx/dolby.bmp", 0xFFFF8000, 0xFFFF8000);
  this->uveDX->frame();
  this->uveDX->waitForEscapeKey(2000);

  this->uveDX->backSurface->blitWithColor();
  this->uveDX->backSurface->loadBMP("gfx/logo.bmp", 0xFFFF8000, 0xFFFF8000);
  this->uveDX->frame();
  this->uveDX->waitForEscapeKey(5000);

  auto background = new Background{this->uveDX};
  this->uveDX->add(background);
  background->setTargetScrollSpeed(1);

  this->uveDX->add(
      new DelaySprite{this->uveDX, 320, 240, this->surface_earth, 0, 905}
  );

  this->uveDX->add(new DelayText{this->uveDX, 200, "a long time ago", 100, 120}
  );

  this->uveDX->add(
      new DelayText{this->uveDX, 240, "in a galaxy far away...", 100, 120}
  );

  this->uveDX->add(new DelayText{this->uveDX, 160, "the", 300, 130});

  this->uveDX->add(new DelayText{this->uveDX, 200, "chicken invasion", 300, 130}
  );

  this->uveDX->add(new DelayText{this->uveDX, 240, "begins!", 300, 130});

  int i = -380;

  while (true) {
    this->uveDX->yOffset = i;

    this->uveDX->update();

    if (this->uveDX->uveInput->isKeyPressed(sf::Keyboard::Scan::Escape))
      break;

    if (++i > 0) {
      for (unsigned int j = 0; j < 200; ++j)
        this->uveDX->add(new IntroChicken{this->uveDX});

      int j = 0;

      while (true) {
        this->uveDX->update();

        if (this->uveDX->uveInput->isKeyPressed(sf::Keyboard::Scan::Escape))
          break;

        if (++j >= 300) {
          this->uveDX->add(
              new DelayText{this->uveDX, 200, "meanwhile...", 0, 40}
          );

          this->uveDX->add(new DelayText{
              this->uveDX, 200, "a couple of light years away", 50, 80
          });

          this->uveDX->add(new DelayText{
              this->uveDX, 200, "our hero is faced with", 150, 100
          });

          this->uveDX->add(new DelayText{
              this->uveDX, 240, "problems of his own...", 150, 100
          });

          int k = 0;

          while (true) {
            this->uveDX->yOffset += 2;

            this->uveDX->update();

            if (this->uveDX->uveInput->isKeyPressed(sf::Keyboard::Scan::Escape))
              break;

            if (++k >= 230) {
              background->setTargetScrollSpeed(4);

              this->uveDX->add(new UveDX::Sprite{
                  this->uveDX, 340, 240, this->surface_chain_burger->getSurf(0)
              });
              this->uveDX->add(new UveDX::Sprite{
                  this->uveDX, 250, 300, this->surface_chain_fighter->getSurf(4)
              });

              int m = -480;

              while (true) {
                this->uveDX->yOffset = m;

                this->uveDX->update();

                if (this->uveDX->uveInput->isKeyPressed(
                        sf::Keyboard::Scan::Escape
                    ))
                  break;

                m += 8;

                if (m > 0) {
                  this->uveDX->yOffset = 0;

                  background->setTargetScrollSpeed(0);

                  int n = 0;

                  while (true) {
                    this->uveDX->update();

                    if (this->uveDX->uveInput->isKeyPressed(
                            sf::Keyboard::Scan::Escape
                        ))
                      break;

                    if (++n >= 50) {
                      this->uveDX->add(new DelaySprite{
                          this->uveDX, 200, 230,
                          this->surface_chain_talkbubble->getSurf(0), 0, 80
                      });
                      this->uveDX->add(new DelaySprite{
                          this->uveDX, 380, 350,
                          this->surface_chain_talkbubble->getSurf(1), 100, 80
                      });
                      this->uveDX->add(new DelaySprite{
                          this->uveDX, 200, 230,
                          this->surface_chain_talkbubble->getSurf(2), 200, 40
                      });

                      for (unsigned int o = 0; o < 280; ++o) {
                        this->uveDX->update();

                        if (this->uveDX->uveInput->isKeyPressed(
                                sf::Keyboard::Scan::Escape
                            ))
                          break;
                      }

                      return;
                    }
                  }

                  return;
                }
              }

              return;
            }
          }

          return;
        }
      }

      return;
    }
  }

  return;
}

void Game::playSound(UveDX::Sound* sound, int x) {
  if (!sound->uveDX->soundEnabled)
    return;

  sound->setPan((10000 * (x - this->uveDX->xOffset) - 3200000) / 1280);

  sound->play();
}

void Game::drawProgressBar(
    int startX,
    int startY,
    unsigned int progressWidth,
    unsigned int totalWidth,
    unsigned int barHeight
) {
  sf::IntRect rect{
      {startX - 2, startY - 2},
      {static_cast<int>(totalWidth + 4), static_cast<int>(barHeight + 4)},
  };

  this->uveDX->backSurface->blitWithColor(&rect, this->color_lightBlue);

  rect.size = {rect.size.x - 2, rect.size.y - 2};
  rect.position = {rect.position.x + 1, rect.position.y + 1};

  this->uveDX->backSurface->blitWithColor(&rect, sf::Color::Black);

  rect.size = {static_cast<int>(progressWidth), rect.size.y - 2};
  rect.position = {rect.position.x + 1, rect.position.y + 1};

  this->uveDX->backSurface->blitWithColor(&rect, this->color_white);
}

bool Game::handlePauseScreen() {
  this->renderPauseOverlay();

  this->uveDX->backSurface->blitWithColor();

  this->font_alphabet_small->blitText(
      320, 235, "press esc to go back or any other key to quit",
      UveDX::Font::TextAlignment::Center
  );

  this->uveDX->frame();

  this->uveDX->uveInput->waitForAllKeysRelease();

  this->uveDX->uveInput->waitForKeyPress();

  return !this->uveDX->uveInput->isKeyPressed(sf::Keyboard::Scancode::Escape);
}

void Game::renderPauseOverlay() {
  UveDX::UveTimer timer{this->uveDX->timer.getInterval()};

  //   this->window.clear(sf::Color::Black);

  sf::IntRect rect{
      {0, 0},
      {static_cast<int>(this->uveDX->getWidth()),
       static_cast<int>(this->uveDX->getHeight())},
  };

  for (int i = 0; 245 - i > 1; i = (i + 245) / 2) {
    rect.position.y = 0;
    rect.size.y = i;

    this->uveDX->backSurface->blitWithColor(&rect);

    rect.position.y = static_cast<int>(this->uveDX->getHeight()) - i;
    rect.size.y = static_cast<int>(this->uveDX->getHeight()) - rect.position.y;

    this->uveDX->backSurface->blitWithColor(&rect);

    this->uveDX->frame();

    timer.waitForTimeInterval();
  }
}
