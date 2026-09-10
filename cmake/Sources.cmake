# Explicit list of XIDER source files.
#
# This list is intentionally explicit (no file(GLOB)) so that the installed
# target is reproducible across machines and packaging is deterministic.
# When adding or removing a source file, update this list.

set(XIDER_SOURCES
    sources/engine.cpp
    sources/frame_updater.cpp
    sources/hud.cpp
    sources/preferences.cpp
    sources/scenes/demo_scene.cpp
    sources/scenes/home.cpp
    sources/scenes/settings.cpp
    sources/scenes/sound.cpp
    sources/scenes/viewer.cpp
    sources/xider.cpp
)
