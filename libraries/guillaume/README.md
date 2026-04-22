# Guillaume

Guillaume is the UI framework used by XIDER. It provides the application
shell, ECS-based UI composition, scene management, event dispatching, and
storage helpers for desktop and XR user interfaces.

## What It Covers

- Application lifecycle management.
- Entity-component-system based UI structures.
- Scenes, event handlers, and render systems.
- Local and session storage helpers.
- Theme and component metadata support.

## Build Notes

Guillaume is built as a C++20 library through CMake and links against Utility
and SQLite3.

## Documentation

- [Architecture](docs/ARCHITECTURE.MD)
- [How Guillaume Works](docs/HOW_GUILLAUME_WORKS.MD)
- [Technical Choices](docs/TECHNICAL_CHOICES.MD)
- [Code Conventions](docs/CODE_CONVENTIONS.MD)
- [Commit Conventions](docs/COMMIT_CONVENTIONS.MD)
