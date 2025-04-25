# Chicken Invaders 1

A modernized and cross-platform version of the original [Chicken Invaders](https://www.interactionstudios.com/chickeninvaders.php) game.

This project concerns only the original 1999 version, not the remastered 2023 release.

## Table of Contents

-   [Disclaimer](#disclaimer)
-   [How to build](#how-to-build)
-   [Documentation](#documentation)
-   [Current state of the project](#current-state-of-the-project)
-   [Additional features](#additional-features)
-   [Acknowledgments](#acknowledgments)
-   [License](#license)

## Disclaimer

This repository contains a reimplementation of the original [Chicken Invaders](https://www.interactionstudios.com/chickeninvaders.php) game, created through reverse engineering for personal and non-commercial purposes. No original game assets are included, as they remain the intellectual property of InterAction Studios.

This project is entirely unofficial and is not affiliated with, endorsed by, or sponsored by InterAction Studios. I do not claim ownership of the Chicken Invaders name, brand, or franchise, nor any rights to the original game content or intellectual property owned by InterAction Studios. All trademarks and copyrights remain the property of their respective owners.

This reimplementation was created in accordance with applicable European Union laws that permit reverse engineering for the purposes of interoperability and personal use, provided it does not infringe on the original rights holders' commercial interests.

## How to build

You can find the build instructions in [`doc/BUILD.md`](./doc/BUILD.md).

## Documentation

You can find all the project documentation in the [`doc`](./doc/README.md) folder.

## Current state of the project

At the moment this is very much a work-in-progress. A lot of the code still carries over the decompiled variable names, which are not always meaningful. There are a few bugs, and the code could definitely be cleaner. That said, it is fully playable, and I will continue improving it over time.

See the [CHANGELOG](/CHANGELOG.md) for the latest updates.

## Additional features

-   Discord Rich Presence with the [GameSDK](https://discord.com/developers/docs/developer-tools/game-sdk).

## Acknowledgments

-   [SFML](https://www.sfml-dev.org/) for the multimedia library.
-   [tinyfiledialogs](http://tinyfiledialogs.sourceforge.net) for the cross-platform message box.

## License

This project is licensed under the AGPL 3.0 License. See the [LICENSE](./LICENSE) file for details.
