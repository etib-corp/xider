# Getting Started

This tutorial walks you from an empty project to a working XIDER setup. It
assumes you have already [built the library](../README.md#quickstart).

## 1. Select a backend and platform

XIDER requires exactly one backend (`BUILD_FOR_OPENXR` or `BUILD_FOR_GLFW`) and
one platform (`BUILD_FOR_ANDROID`, `BUILD_FOR_LINUX`, `BUILD_FOR_WINDOWS`, or
`BUILD_FOR_MACOS`).

```sh
cmake -S . -B build -DBUILD_FOR_GLFW=ON -DBUILD_FOR_LINUX=ON
cmake --build build
```

## 2. Create a platform implementation

XIDER bridges Guillaume scenes with Evan rendering. Provide an `evan::IPlatform`
implementation for your target backend and platform.

## 3. Construct the application

The `xider::XIDER` class combines the Guillaume application shell with the Evan
graphics engine:

```cpp
#include <xider/xider.hpp>

// Construct XIDER with an Evan platform and a resource provider.
```

## 4. Run the main loop

XIDER drives the frame loop: it polls events, updates, renders, and presents.

## Next steps

- Read [How XIDER Works](HOW_XIDER_WORKS.md) for the frame lifecycle.
- Browse the [API reference](https://etib-corp.github.io/xider).
