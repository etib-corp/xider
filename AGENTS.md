# AI Agent Guide for XIDER

This guide helps AI assistants understand the XIDER codebase and contribute effectively to the project.

## Project Overview

**XIDER** (eXtensible Integrated Development Environment for Reality) is an IDE for XR (Extended Reality) application development. It's built with a modular architecture consisting of several key repositories:

### Core Libraries

1. **`libraries/utility`** - Shared utilities and helper functions
   - **Math**: Vectors, matrices, quaternions, trigonometric functions
   - **Graphics**: Colors, poses, textures, vertices, meshes, text rendering
   - **Events**: Mouse, keyboard, text input, XR hand interactions
   - **Logging**: Logger interfaces and implementations
   - **System I/O**: Asset management for desktop and Android
   - Used by all other libraries as the foundation layer

2. **`libraries/evan`** - Graphics and XR rendering engine
   - **Vulkan Rendering**: GPU shaders, materials, meshes, render objects
   - **Platform Abstraction**: Desktop (GLFW) and XR (OpenXR) backends
   - **Swapchain Management**: Frame buffer and image presentation
   - **Scene Management**: Renderable scene graphs
   - **Resource Management**: Asset loading and GPU resource handling

3. **`libraries/guillaume`** - UI framework (most active development)
   - ECS-based UI system
   - Scene management
   - Event handling
   - Application lifecycle

4. **`libraries/xider`** - Main IDE application
   - Integrates guillaume and evan
   - Platform-specific implementations in `platforms/`

### Platform Implementations

- `platforms/glfw/` - Desktop platforms (macOS, Windows, Linux)
- `platforms/openxr/` - XR platforms (Android, etc.)

## Library Deep Dives

### Utility Library

**Purpose**: Foundation layer providing shared data types and helpers across all libraries.

**Key Namespaces**:
- `utility::math` - Math primitives (vectors, matrices, quaternions)
- `utility::graphic` - Graphics types (colors, poses, meshes, text)
- `utility::event` - Event models (input, XR, system events)
- `utility::logging` - Logging infrastructure
- `utility::system_io` - Platform I/O and asset management

**File Structure**:
```
libraries/utility/
├── headers/utility/
│   ├── math/              # Math types
│   │   ├── vector.hpp
│   │   ├── matrix.hpp
│   │   ├── quaternion.hpp
│   │   └── trigonometric.hpp
│   ├── graphic/           # Graphics types
│   │   ├── color.hpp
│   │   ├── pose.hpp
│   │   ├── mesh.hpp
│   │   ├── texture.hpp
│   │   ├── view.hpp
│   │   ├── ray.hpp
│   │   └── text/          # Text rendering types
│   ├── event/             # Event definitions
│   │   ├── event.hpp
│   │   ├── mouse_motion_event.hpp
│   │   ├── keyboard_event.hpp
│   │   ├── hand_motion_event.hpp
│   │   └── text_input_event.hpp
│   ├── logging/           # Logging
│   └── system_io/         # Platform I/O
├── sources/
└── tests/
```

**Common Usage**:
```cpp
// Math types
utility::math::Vector3F position(1.0f, 2.0f, 3.0f);
utility::math::QuaternionF rotation;

// Graphics types
utility::graphic::ColorF color(1.0f, 0.0f, 0.0f, 1.0f);
utility::graphic::PoseF pose(position, rotation);
utility::graphic::RayF ray(origin, direction);

// Events
utility::event::MouseEvent mouseEvent;
utility::event::KeyboardEvent keyboardEvent;
utility::event::HandMotionEvent handEvent;

// Logging
auto &logger = getLogger();
logger.info() << "Message";
```

**Design Principles**:
- Keep generic and reusable
- No application-specific logic
- Minimal dependencies (GLM, stb, freetype, tinyobjloader)
- Header-heavy for template support

---

### Evan Library

**Purpose**: Rendering and runtime layer providing Vulkan graphics pipeline and platform abstractions.

**Key Components**:
- **Engine**: Main render loop coordinator
- **Renderer**: Vulkan command recording and execution
- **Platform**: Desktop (GLFW) or XR (OpenXR) abstraction
- **Device Backend**: Vulkan device management
- **Swapchain**: Frame buffer management
- **Resources**: Shaders, materials, meshes, textures

**File Structure**:
```
libraries/evan/
├── headers/evan/
│   ├── Engine.hpp              # Main engine class
│   ├── Renderer.hpp            # Vulkan renderer
│   ├── RenderObject.hpp        # Renderable object
│   ├── GPUShader.hpp           # Shader management
│   ├── GPUMaterial.hpp         # Material system
│   ├── GPUMesh.hpp             # Mesh management
│   ├── RessourceManager.hpp    # Resource handling
│   ├── ADeviceBackend.hpp      # Device abstraction
│   ├── ASwapchainContext.hpp   # Swapchain abstraction
│   ├── IPlatform.hpp           # Platform interface
│   ├── EvanPlatform.hpp        # Platform enum
│   ├── openxr/                 # OpenXR backend
│   │   ├── IXrPlatform.hpp
│   │   ├── XrDeviceBackend.hpp
│   │   ├── XrSwapchainContext.hpp
│   │   ├── actions/            # XR action management
│   │   └── platform/           # Platform-specific XR
│   └── glfw/                   # GLFW backend
│       ├── DesktopBackend.hpp
│       ├── IDesktopPlatform.hpp
│       └── platform/           # Platform-specific desktop
├── sources/
└── tests/
```

**Typical Startup Flow**:
```cpp
// 1. Create platform implementation
auto platform = std::make_unique<GLFWPlatform>();

// 2. Create engine with platform
evan::Engine engine(std::move(platform));

// 3. Initialize resources
engine.initialize();

// 4. Main loop
while (running) {
    engine.pollEvents();      // Gather input
    engine.update();          // Update state
    engine.render();          // Record and submit commands
    engine.present();         // Present frame
}
```

**Backend Selection** (CMake configuration):
- `BUILD_FOR_GLFW` - Desktop windowing and input
- `BUILD_FOR_OPENXR` - XR platform support
- Platform: `BUILD_FOR_ANDROID`, `BUILD_FOR_LINUX`, `BUILD_FOR_WINDOWS`, `BUILD_FOR_MACOS`

**Design Principles**:
- Low-level performance focus
- Clear separation: platform vs rendering vs scene logic
- Vulkan-centric with abstraction layers
- Frame ownership and synchronization handled internally

---

### Guillaume Library

**Purpose**: UI framework for XR and desktop applications using ECS architecture.

**Key Systems**:
- **Application**: Lifecycle management
- **Scene**: UI scene composition
- **ECS**: Entity-component-system for UI elements
- **Event System**: Event bus and handlers
- **Storage**: Local and session storage
- **Theme**: UI theming support

**File Structure**:
```
libraries/guillaume/
├── headers/guillaume/
│   ├── application.hpp       # Application entry point
│   ├── engine.hpp            # UI engine interface
│   ├── scene.hpp             # Scene management
│   ├── scene_manager.hpp     # Multi-scene support
│   ├── component_registry.hpp
│   ├── components/           # UI components
│   │   ├── focus.hpp
│   │   ├── transform.hpp
│   │   ├── bound.hpp
│   │   ├── color.hpp
│   │   ├── text.hpp
│   │   └── interaction.hpp
│   ├── systems/              # UI systems
│   │   ├── focus.hpp
│   │   ├── text_render.hpp
│   │   ├── rectangle_render.hpp
│   │   ├── keyboard_control.hpp
│   │   └── mouse_button.hpp
│   ├── entities/             # Pre-built UI entities
│   │   ├── button.hpp
│   │   ├── panel.hpp
│   │   ├── text.hpp
│   │   └── icon.hpp
│   ├── ecs/                  # ECS framework
│   └── event/                # Event handling
├── sources/
├── templates/                # Template implementations (.tpp)
└── tests/
```

**Design Principles**:
- ECS for flexible UI composition
- Event-driven architecture
- Platform-agnostic UI logic
- Builder/Director patterns for entity creation

---

### Entity-Component-System (ECS)

Guillaume uses ECS for UI composition:

```cpp
// Components - Data containers
class Focus : public ecs::Component {
    Handler _onFocusGained;
    Handler _onFocusLost;
};

// Systems - Logic that processes entities
class FocusSystem : public ecs::SystemFiller<components::Focus, ...> {
    void update(const ecs::Entity::Identifier &id) override;
};

// Entities - Composed of components
class Button : public ecs::ParentEntityFiller<...> {
    // Builder and Director patterns for entity creation
};
```

### System Phases

Systems execute in ordered phases:

- `ecs::Phase::Event` - Event processing systems
- `ecs::Phase::Render` - Rendering systems
- Custom phases as needed

### Event System

Event-driven architecture with EventBus:

```cpp
// Subscribe to events
class MouseMotion : public event::EventManager<MouseMotionEvent> {
    MouseMotion(EventBus &bus) : event::EventManager(bus) {}
};

// Publish events
eventBus.publish(std::move(event));
```

## File Organization

### Naming Conventions

- **Files**: snake_case (e.g., `focus_system.hpp`)
- **Classes**: PascalCase (e.g., `FocusSystem`)
- **Methods/Variables**: camelCase (e.g., `setFocusHandler`)
- **Members**: Prefix with `_` (e.g., `_focusedEntity`)
- **Namespaces**: `guillaume::systems`, `guillaume::components`, etc.

## Common Tasks for AI Agents

### Working with Utility Library

#### Adding a New Math Type

1. Create header in `libraries/utility/headers/utility/math/`
2. Implement using GLM as base when appropriate
3. Add to `utility/math/common.hpp` if needed
4. Add tests in `libraries/utility/tests/`

Example:
```cpp
// headers/utility/math/new_type.hpp
#pragma once
#include <glm/glm.hpp>

namespace utility::math {

class NewType {
    // Implementation
};

} // namespace utility::math
```

#### Adding a New Event Type

1. Create header in `libraries/utility/headers/utility/event/`
2. Inherit from `utility::event::Event` base class
3. Follow existing event patterns (mouse, keyboard, hand)
4. Add to event system consumers as needed

Example:
```cpp
#include "utility/event/event.hpp"

namespace utility::event {

class CustomEvent : public Event {
public:
    CustomEvent() = default;
    ~CustomEvent() override = default;

    // Event-specific data and methods
};

} // namespace utility::event
```

#### Adding Graphics Types

1. Create in `libraries/utility/headers/utility/graphic/`
2. Keep data-only (no rendering logic)
3. Use existing types (Vector, Color, Pose) as building blocks
4. Document with Doxygen

---

### Working with Evan Library

#### Adding a New Render Object

1. Create in `libraries/evan/headers/evan/`
2. Inherit from `RenderObject` or create new type
3. Implement Vulkan resource management
4. Add to `RessourceManager` if needed

#### Creating Platform-Specific Code

**For Desktop (GLFW)**:
1. Add to `libraries/evan/headers/evan/glfw/`
2. Implement `IDesktopPlatform` interface
3. Handle windowing, input, Vulkan surface

**For XR (OpenXR)**:
1. Add to `libraries/evan/headers/evan/openxr/`
2. Implement `IXrPlatform` interface
3. Handle XR session, spaces, input actions

#### Adding Shaders

1. Create GLSL files in `libraries/evan/shaders/`
2. Both vertex and fragment shaders required
3. Update shader compilation in `GPUShader.cpp`
4. Register in material system

#### Working with Vulkan Resources

```cpp
// Access Vulkan device
auto device = getDevice();

// Create buffer
vk::BufferCreateInfo bufferInfo;
bufferInfo.setSize(size);
bufferInfo.setUsage(vk::BufferUsageFlagBits::eVertexBuffer);

// Manage lifecycle with RAII
vk::UniqueBuffer buffer = device.createBufferUnique(bufferInfo);
```

---

### Working with Guillaume Library

### Adding a New Component

1. Create header in `libraries/guillaume/headers/guillaume/components/`
2. Create source in `libraries/guillaume/sources/components/`
3. Add to `ComponentRegistry` in `component_registry.hpp`
4. Add tests in `libraries/guillaume/tests/`

Example: See `Focus` component implementation

### Adding a New System

1. Create header in `libraries/guillaume/headers/guillaume/systems/`
2. Create source in `libraries/guillaume/sources/systems/`
3. Register in `Application::registerCoreSystems()` in `application.tpp`
4. Add tests in `libraries/guillaume/tests/`

Example: See `Focus` system implementation

### Adding a New Entity

1. Create in `libraries/guillaume/headers/guillaume/entities/`
2. Implement Builder and Director patterns
3. Add to `Scene` constructor's `EntityBuilderManagerFiller`
4. Add to `Scene` constructor's `EntityDirectorManagerFiller`

### Writing Tests

Use Google Test framework:

```cpp
#include <gtest/gtest.h>
#include "systems/test_focus.hpp"  // Test fixture

TEST_F(TestFocus, TestName) {
    // Arrange
    auto entity = createFocusableEntity();

    // Act
    _focusSystem->setFocus(entity, ray);

    // Assert
    EXPECT_TRUE(_focusSystem->getFocusedEntity().has_value());
}
```

### Building and Testing

```bash
# Configure
cmake -B build -DBUILD_TESTING=ON

# Build
cmake --build build

# Run tests
cd build && ctest --verbose
```

## Code Style Guidelines

### Class Layout

```cpp
class Example {
public:
    // Types first
    using Handler = std::function<void()>;

    // Constructors
    Example();

    // Public methods
    void update();

    // Public members
    int publicValue = 0;

protected:
    // Protected types
    enum class State { Idle, Active };

    // Protected methods
    void refresh();

    // Protected members
    int _sharedValue = 0;

private:
    // Private methods
    void compute();

    // Private members
    int _value = 0;
};
```

### Documentation

Use Doxygen comments:

```cpp
/**
 * @brief Brief description
 *
 * Detailed description if needed.
 *
 * @param param Description of parameter
 * @return Description of return value
 *
 * @see RelatedClass
 */
void method(int param);
```

### Commit Messages

Follow Conventional Commits:

```
feat(systems): add Focus system for UI entity focus management

- Add Focus system to manage focus state
- Update Focus component with gained/lost handlers
- Add comprehensive tests

Fixes: #49
```

**Types**: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`, `optim`

## Important Design Decisions

### Why ECS for UI?

- Flexible composition over inheritance
- Easy to add new behaviors via components
- Systems can process entities uniformly
- Good for XR where UI needs vary widely

### Why Separate Libraries?

- `utility`: Platform-agnostic helpers
- `evan`: Graphics/XR concerns isolated
- `guillaume`: UI framework independent of rendering
- `xider`: Application-specific logic

### Event System Design

- EventBus for decoupled communication
- EventManager for typed event handling
- EventSubscriber for manual event consumption
- Events are moved (not copied) for performance

### Vulkan-Centric Rendering (Evan)

- Single graphics API for consistency across platforms
- Explicit control over GPU resources
- Better performance for XR applications
- Abstraction layers to simplify common tasks

### Platform Abstraction Strategy

- Interfaces (`IPlatform`, `IDesktopPlatform`, `IXrPlatform`)
- Backend implementations (GLFW, OpenXR)
- Platform-specific code isolated in subdirectories
- Build-time selection via CMake options

## Common Pitfalls

### Guillaume Pitfalls

1. **Template Implementations**: Must be in `.tpp` files, not `.cpp`
2. **Component Registration**: Forgot to add to `ComponentRegistry` = runtime errors
3. **System Phases**: Wrong phase = systems execute in wrong order
4. **Event Ownership**: Events are `unique_ptr` - don't use after moving
5. **Entity Lifecycle**: Entities owned by `EntityRegistry`, use identifiers

### Evan Pitfalls

1. **Vulkan Validation Layers**: Enable during development, disable in release
2. **Swapchain Recreation**: Handle window resize and surface changes
3. **Synchronization**: Proper fence/semaphore usage for frame timing
4. **Resource Lifetime**: Ensure Vulkan objects destroyed in correct order
5. **Platform Selection**: Exactly one backend (GLFW/OpenXR) and one platform

### Utility Pitfalls

1. **Generic Design**: Don't add application-specific logic
2. **Header Dependencies**: Keep includes minimal to avoid circular dependencies
3. **Math Types**: Use GLM conventions for consistency
4. **Event Types**: Keep events data-only, no business logic

### Cross-Library Pitfalls

1. **Dependency Direction**: utility ← evan/guillaume ← xider (never reverse)
2. **Include Paths**: Use quoted includes for project headers
3. **Build Order**: CMake handles this, but be aware of dependencies
4. **Shared State**: Avoid global state, use dependency injection

## Testing Strategy

- Unit tests for components and systems
- Google Test framework
- Test fixtures for common setup
- Mock events for system testing
- Run tests on every PR via GitHub Actions

## Resources

### Project Documentation

- [Project Architecture](docs/PROJECT_ARCHITECTURE.MD)
- [Code Conventions](docs/CODE_CONVENTIONS.md)
- [Commit Conventions](docs/COMMIT_CONVENTIONS.md)
- [Contributing Guide](CONTRIBUTING.md)

### Library Documentation

**Utility**:
- [Architecture](libraries/utility/docs/ARCHITECTURE.md)
- [How Utility Works](libraries/utility/docs/HOW_GUILLAUME_WORKS.md)
- [Technical Choices](libraries/utility/docs/TECHNICAL_CHOICES.md)

**Evan**:
- [Architecture](libraries/evan/docs/ARCHITECTURE.md)
- [How Evan Works](libraries/evan/docs/HOW_EVAN_WORKS.md)
- [Technical Choices](libraries/evan/docs/TECHNICAL_CHOICES.md)

**Guillaume**:
- [Architecture](libraries/guillaume/docs/ARCHITECTURE.md)
- [How Guillaume Works](libraries/guillaume/docs/HOW_GUILLAUME_WORKS.md)
- [Technical Choices](libraries/guillaume/docs/TECHNICAL_CHOICES.md)

### External Resources

- [Vulkan Specification](https://www.khronos.org/vulkan/)
- [OpenXR Specification](https://www.khronos.org/openxr/)
- [GLFW Documentation](https://www.glfw.org/documentation.html)
- [GLM Documentation](https://glm.g-truc.net/)
- [Google Test](https://google.github.io/googletest/)

## Contact

For questions or issues:

- Clément: <clement-antoine.xavier@epitech.eu>
- Léo: <leo.sautron@epitech.eu>
- Julien: <julien.ferdinand@epitech.eu>
- Nathan: <nathan.maillot@epitech.eu>

## Quick Reference

### Key Files to Know

#### Guillaume Files

| File | Purpose |
|------|---------|  
| `libraries/guillaume/headers/guillaume/application.hpp` | Application entry point |
| `libraries/guillaume/headers/guillaume/scene.hpp` | Scene management |
| `libraries/guillaume/headers/guillaume/ecs/system.hpp` | System base class |
| `libraries/guillaume/headers/guillaume/ecs/component.hpp` | Component base class |
| `libraries/guillaume/headers/guillaume/event/event_bus.hpp` | Event system |

#### Evan Files

| File | Purpose |
|------|---------|  
| `libraries/evan/headers/evan/Engine.hpp` | Main engine class |
| `libraries/evan/headers/evan/Renderer.hpp` | Vulkan renderer |
| `libraries/evan/headers/evan/IPlatform.hpp` | Platform interface |
| `libraries/evan/headers/evan/RenderObject.hpp` | Renderable object base |
| `libraries/evan/headers/evan/GPUShader.hpp` | Shader management |
| `libraries/evan/headers/evan/glfw/IDesktopPlatform.hpp` | Desktop platform interface |
| `libraries/evan/headers/evan/openxr/IXrPlatform.hpp` | XR platform interface |

#### Utility Files

| File | Purpose |
|------|---------|  
| `libraries/utility/headers/utility/math/vector.hpp` | Vector types |
| `libraries/utility/headers/utility/math/matrix.hpp` | Matrix types |
| `libraries/utility/headers/utility/math/quaternion.hpp` | Quaternion types |
| `libraries/utility/headers/utility/graphic/color.hpp` | Color types |
| `libraries/utility/headers/utility/graphic/pose.hpp` | Pose (position + orientation) |
| `libraries/utility/headers/utility/graphic/mesh.hpp` | Mesh data structures |
| `libraries/utility/headers/utility/event/event.hpp` | Event base class |
| `libraries/utility/headers/utility/event/mouse_motion_event.hpp` | Mouse motion event |
| `libraries/utility/headers/utility/event/keyboard_event.hpp` | Keyboard event |
| `libraries/utility/headers/utility/event/hand_motion_event.hpp` | XR hand motion event |

### Common Patterns

#### Guillaume Patterns

- **SystemFiller**: Template for systems requiring specific components
- **EntityFiller**: Template for entities with predefined components
- **EventManager**: Helper for event subscription in systems
- **Builder/Director**: Entity creation patterns

#### Evan Patterns

- **Platform Abstraction**: `IPlatform` → `DesktopBackend`/`XrDeviceBackend`
- **RAII Vulkan Resources**: `vk::UniqueBuffer`, `vk::UniqueImage`, etc.
- **Swapchain Management**: Context-based frame buffer handling
- **Action-Based Input**: OpenXR action sets for XR interactions

#### Utility Patterns

- **Value Types**: Math and graphic types are copyable value types
- **Event Hierarchy**: All events inherit from `utility::event::Event`
- **Singleton Helpers**: For global resources (logging, asset manager)
- **Type Aliases**: `Vector3F`, `ColorF`, `PoseF` for common types

### Build Commands

#### Basic Build

```bash
# Configure with tests
cmake -B build -DBUILD_TESTING=ON

# Build all
cmake --build build

# Run tests
cd build && ctest --verbose
```

#### Platform-Specific Builds (Evan)

```bash
# macOS Desktop
cmake -B build -DBUILD_FOR_GLFW=ON -DBUILD_FOR_MACOS=ON

# Linux Desktop
cmake -B build -DBUILD_FOR_GLFW=ON -DBUILD_FOR_LINUX=ON

# Windows Desktop
cmake -B build -DBUILD_FOR_GLFW=ON -DBUILD_FOR_WINDOWS=ON

# Android XR
cmake -B build -DBUILD_FOR_OPENXR=ON -DBUILD_FOR_ANDROID=ON
```

#### Build Options

```bash
# Enable documentation
-DBUILD_DOCS=ON

# Enable testing
-DBUILD_TESTING=ON

# Select backend (choose exactly one)
-DBUILD_FOR_GLFW=ON      # Desktop
-DBUILD_FOR_OPENXR=ON    # XR

# Select platform (choose exactly one)
-DBUILD_FOR_MACOS=ON
-DBUILD_FOR_LINUX=ON
-DBUILD_FOR_WINDOWS=ON
-DBUILD_FOR_ANDROID=ON
```

#### Running Specific Tests

```bash
# Guillaume tests
./build/libraries/guillaume/tests/test_guillaume --gtest_filter="TestFocus.*"

# Evan tests
./build/libraries/evan/tests/test_evan --gtest_filter="Renderer.*"

# Utility tests
./build/libraries/utility/tests/test_utility --gtest_filter="Vector.*"

# All tests with filter
ctest -R "Focus" --verbose
```

## Recent Changes

### Focus System (June 2026)

Recently implemented Focus system for managing UI entity focus:

- `Focus` component with gained/lost handlers
- `FocusSystem` for focus state management
- Ray-based intersection testing
- Integration with keyboard and text input systems

See commit `008b06b` for implementation details.
