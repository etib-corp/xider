# How XIDER Works

XIDER is the glue layer that turns the lower-level libraries into a runnable
application.

## Startup Flow

1. The platform code creates an `evan::Engine`.
2. That engine is passed into `xider::XIDER`.
3. The XIDER renderer is configured with the engine.
4. The application loop runs through Guillaume and delegates rendering to Evan.

## Runtime Responsibilities

- `xider::EventHandler` receives platform events.
- `xider::Renderer` forwards draw work to Evan.
- `xider::XIDER` manages the application and scene lifetime.
- The `Main` and `Settings` scenes define the visible application state.

## Practical Rule

XIDER should stay thin. Rendering, UI behavior, and shared support logic belong
in Evan, Guillaume, and Utility.