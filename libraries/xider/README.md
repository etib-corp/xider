# XIDER

XIDER is the application layer that combines Utility, Evan, and Guillaume into
a single desktop and XR IDE shell.

## What It Does

- Owns the top-level application object.
- Bridges Guillaume scenes with Evan rendering.
- Wraps the engine and event handler used by the platform entry points.
- Provides the `Main` and `Settings` scenes used by the app.

## Build Notes

XIDER is built as a C++20 library through the workspace root CMake project.
It links against Utility, Evan, and Guillaume.

## Documentation

- [Architecture](docs/ARCHITECTURE.md)
- [How XIDER Works](docs/HOW_XIDER_WORKS.md)
