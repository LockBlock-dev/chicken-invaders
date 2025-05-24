# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.4.1] - 2025-05-24

### Fixed

-   CMake SFML

## [0.4.0] - 2025-05-24

### Added

-   Missing `TileLayer` entity in UveDX (used in Island Wars 1)
-   Missing `UveDX::bounds` member (used by TileLayer)
-   Missing `UveDX::setBackSurfaceClipRegion` method

### Changed

-   Renamed `Game::sub_40AAD8` to `Game::renderPauseOverlay`

### Fixed

-   Debug timings

### Removed

-   SFML FPS limit

## [0.3.1] - 2025-05-11

### Added

-   More "magic numbers" as constants

### Changed

-   Switched `Explosion` and `Smoke` particles from dynamic C-style arrays to `std::array` of `Particle`

### Fixed

-   `UveDX::showError` string

## [0.3.0] - 2025-05-11

### Added

-   `random_range` function to replace `random % max + min`
-   `AsteroidType` enum

### Changed

-   Reverse engineer more members and variables

### Fixed

-   Missiles angle computation

## [0.2.2] - 2025-05-02

### Changed

-   Reverse engineer the missing members from `Font`, `Surface` and `Sprite`
-   Use `std::clamp` instead of manual clamping
-   Various code simplification

## [0.2.1] - 2025-04-26

### Fixed

-   Make MSVC compile

## [0.2.0] - 2025-04-25

### Added

-   clang format
-   All compilation warnings

### Changed

-   Rename `bounding_box` to `BoundaryBouncer`
-   Switch `GameController` C-style arrays to `std::array`
-   Remove `extern` in favor of `inline`

### Fixed

-   Fix pseudo writing in `GameController::saveScoresFile`
-   Close button now terminates the application

## [0.1.0] - 2025-04-20

### Added

-   First working version of the game
