#ifdef WITH_DISCORD

#include "DiscordRichPresence.hpp"

#include "UveDX.hpp"
#include "constants.hpp"

DiscordRichPresence::DiscordRichPresence(UveDX::UveDX* uveDX)
    : UveDX::UveBase(uveDX), core(), activity(), initialized(false) {
  auto result = discord::Core::Create(
      constants::DISCORD_CLIENT_ID, DiscordCreateFlags_NoRequireDiscord,
      &this->core
  );

  if (result != discord::Result::Ok && result != discord::Result::InternalError)
    this->uveDX->onError(
        "DiscordRichPresence::DiscordRichPresence",
        "Failed to initialize Discord Game SDK", static_cast<int>(result)
    );

  this->activity.SetDetails("In game");
  this->activity.GetAssets().SetLargeImage("game_logo");
  this->activity.GetAssets().SetLargeText("Chicken Invaders 1");

  if (result == discord::Result::Ok) {
    this->core->ActivityManager().UpdateActivity(
        this->activity,
        [this](discord::Result result) {
          if (result != discord::Result::Ok)
            this->uveDX->onError(
                "DiscordRichPresence::DiscordRichPresence",
                "Failed to update Discord activity", static_cast<int>(result)
            );
        }
    );

    this->initialized = true;
  }
}

void DiscordRichPresence::update() {
  if (!this->initialized)
    return;

  auto result = this->core->RunCallbacks();

  if (result != discord::Result::Ok)
    this->uveDX->onError(
        "DiscordRichPresence::update", "Failed to run callbacks",
        static_cast<int>(result)
    );
}

bool DiscordRichPresence::isInitialized() const {
  return this->initialized;
}

#endif
