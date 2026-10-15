# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- `Showcase` scene demonstrating the Guillaume Material Design 3 entity
  families (app bar, navigation bar, tabs, list, cards, chips, badges,
  switches, checkboxes, radio buttons, sliders, text fields, progress
  indicators, dividers, snackbars and dialogs).
- The `Showcase` scene is now the only application scene (the previous
  `Home`, `Settings` and `Sound` scenes were removed).
- Standard open-source documentation: `CHANGELOG.md`, `CODE_OF_CONDUCT.md`,
  `SECURITY.md`, and `AUTHORS.md`.
- Packaging & consumability: `install()`/`export()`, a CMake package config
  (`xiderConfig.cmake`) for `find_package(xider)`, and CPack rules.
- Benchmark harness (`BUILD_BENCHMARKS`) covering entity traversal and
  signature queries.
- CI hardening: `ctest` execution, ASan/UBSan and `clang-tidy` jobs, and
  coverage reporting.
- Documentation: full README, Getting Started tutorial, and versioning &
  support policy.
- A runnable `examples/scene_objects` sample.

### Changed

- Replaced `file(GLOB)` with explicit source lists for reproducible builds.
- Moved include paths onto the `xider` target (target-scoped includes).
- Removed the `xider::Engine` adapter; `evan::Engine` is now passed directly to
  Guillaume as a `utility::Engine` (the Guillaume engine interface was removed
  upstream).

## [1.0.0] - 2025-08-21

### Added

- Initial release of the XIDER IDE.
- Cross-platform support for Windows, macOS, Linux, and XR platforms.
- Extensible architecture with plugins and extensions.
- Integrated debugging tools.
- Visual scripting system.
- Asset management system.
- Application layer combining Utility, Evan, and Guillaume.
- `Home`, `Settings`, and `Sound` scenes.
