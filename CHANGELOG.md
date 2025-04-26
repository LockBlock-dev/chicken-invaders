# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

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
