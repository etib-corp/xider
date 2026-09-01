# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

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
