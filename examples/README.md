# XIDER Examples

This directory contains runnable samples that demonstrate how to use the XIDER
framework.

## scene_objects

A minimal sample that demonstrates the CPU-side Guillaume `EntityRegistry`
object-management API (adding entities and querying by signature). It does not
require a live Vulkan device, so it builds and runs anywhere.

### Building

```sh
cmake -S . -B build -DBUILD_EXAMPLES=ON
cmake --build build --target xider_scene_objects
```

### Running

```sh
./build/examples/scene_objects/xider_scene_objects
```
