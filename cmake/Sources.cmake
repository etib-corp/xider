# Explicit list of XIDER source files.
#
# This list is intentionally explicit (no file(GLOB)) so that the installed
# target is reproducible across machines and packaging is deterministic.
# When adding or removing a source file, update this list.

set(XIDER_SOURCES
    sources/scenes/showcase.cpp
    sources/xider.cpp
)
