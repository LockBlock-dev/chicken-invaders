#pragma once

#ifdef WITH_DISCORD

#include "UveBase.hpp"
#include <cstdint>
#include <discord.h>
#include <string>

class DiscordRichPresence : public UveDX::UveBase {
public:
  DiscordRichPresence(UveDX::UveDX *uveDX);

  void update() override;
  bool isInitialized() const;

private:
  discord::Core *core;
  discord::Activity activity;
  bool initialized;
};

#else

#include "UveBase.hpp"

class DiscordRichPresence : public UveDX::UveBase {
public:
  DiscordRichPresence(UveDX::UveDX *uveDX) : UveBase(uveDX) {}
  void update() override {}
  bool isInitialized() const { return false; }
};

#endif
