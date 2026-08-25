# XIDER Architecture

XIDER is the application composition layer above Utility, Evan, and Guillaume.

## Layering

- Utility provides shared data types and helpers.
- Evan provides the rendering and platform engine.
- Guillaume provides the UI framework and application template.
- XIDER wires those pieces together into the final IDE application.

## Main Classes

- `xider::XIDER` owns the application shell and the shared engine pointer.
- `xider::Engine` adapts Evan to the Guillaume engine interface (rendering + events).
- `xider::scenes::Home` and `xider::scenes::Settings` define the app screens.

## Runtime Flow

1. The platform entry point creates the Evan engine.
2. XIDER receives that engine and configures the renderer.
3. Guillaume drives the application loop and scene management.
4. Evan performs the Vulkan rendering work on each frame.

## Design Goal

Keep platform entry points thin and move application-specific wiring into a
single reusable library.