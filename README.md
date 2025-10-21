# XIDER

XIDER is a library for 3D rendering and input handling for applications on XR devices and traditional platforms. It is built on top of OpenXR and Vulkan, providing a high-level interface for developers to create immersive experiences.

## Key Features

- **3D Primitives**: Built-in support for rendering common 3D shapes like cubes, spheres, and text.
- **Event Handling**: Comprehensive event system for handling user input and interactions.

## Quick Start

```cpp
int main() {
    return 0;
}
```

## Documentation

- **[How XIDER Works](docs/HOW_XIDER_WORKS.md)** - Comprehensive guide to concepts, lifecycle, and usage patterns
- **[Architecture Documentation](docs/ARCHITECTURE.md)** - Detailed overview of the system's design and components
- **[Technical Choices Documentation](docs/TECHNICAL_CHOICES.md)** - Insights into design decisions and technologies

## Building

XIDER builds as a static library along with examples and tests:

```bash
mkdir build && cd build
cmake ..
make
```

### Running Tests

To run the test suite:

```bash
./tests/test_xider
```
