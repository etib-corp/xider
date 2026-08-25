# Building XIDER

This guide explains how to configure and build XIDER with CMake for each
platform.

## Build Matrix Rules

The root [CMakeLists.txt](../CMakeLists.txt) enforces these rules:

- Exactly one platform must be enabled:
  - `BUILD_FOR_ANDROID`
  - `BUILD_FOR_LINUX`
  - `BUILD_FOR_WINDOWS`
  - `BUILD_FOR_MACOS`
- Exactly one backend must be enabled:
  - `BUILD_FOR_OPENXR`
  - `BUILD_FOR_GLFW`

If multiple or no options are selected, CMake stops with an error.

## General Build Steps

1. Configure a build directory with CMake.
2. Build with `cmake --build`.

Template:

```bash
cmake -S . -B <build-dir> <options> -DBUILD_FOR_<platform>=ON -DBUILD_FOR_<backend>=ON
cmake --build <build-dir> -j
```

You can add `-DCMAKE_BUILD_TYPE=Debug` or `-DCMAKE_BUILD_TYPE=Release` during
configuration.

## Troubleshooting

- Error about invalid platform configuration:
  enable exactly one `BUILD_FOR_*` platform option.
- Error about invalid backend configuration:
  enable exactly one backend (`BUILD_FOR_OPENXR` or `BUILD_FOR_GLFW`).
- If you switch platform/backend combinations, use a different build
  directory (recommended) or clean the previous one before reconfiguring.
