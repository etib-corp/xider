# Technological Watch

## Objective

Actively explore, test, and integrate emerging technologies with a critical approach.

## Expected Deliverables

- Watch notes
- Comparative analysis sheets
- Proof of Concept (POC)
- Changelog
- Contributions on GitHub/forums

## What to Avoid

- Reading without testing
- Integrating without documentation
- Blindly following trends

## Key Performance Indicators (KPIs)

| **KPI** | **Description** |
| --- | --- |
| **Number of technologies tested** | Track how many emerging technologies have been evaluated |
| **Number of POCs created** | Monitor the number of proof-of-concept implementations |
| **Frequency of updates to watch documentation** | Ensure regular updates to technological watch notes |

## KPI Snapshot (2026-04-09)

| **KPI** | **Current Value** | **Evidence** |
| --- | --- | --- |
| Number of technologies tested | 9 | Vulkan, OpenXR, TinyObjLoader, Assimp, MicroUI, Android XR stack, Skia, React Native Skia, C++ POC base template |
| Number of POCs created | 6 | `android-xr-poc`, `tinyobjloader-poc`, `VulkanBase`, `assimp-poc`, `microui`, `cpp_poc_template` |
| Watch documentation update frequency | 1 structured update | This document update + asset-backed notes |

## Technologies Watched

- Vulkan
- OpenXR
- SDL3
- GLFW
- Dear ImGui
- Godot Engine
- Open Source UI libraries
- XR development tools and frameworks

## Watch Notes

### 1) XR and Rendering Foundations

- **Android XR POC** (`etib-corp/android-xr-poc`)
  - Focus: practical XR setup and Android integration.
  - Insight: useful entry point to validate device/toolchain constraints early.
  - Asset: ![Android XR repository snapshot](../assets/technological_watch/Android%20XR%20POC.png)

- **Vulkan Base** (`etib-corp/VulkanBase`)
  - Focus: long-lived Vulkan baseline, architecture and rendering pipeline evolution.
  - Insight: stable base project for testing low-level renderer changes and profiling.
  - Asset: ![Vulkan base repository snapshot](../assets/technological_watch/Vulkan%20Base%20Repository.png)

- **OpenXR + Vulkan external reference** (`janhsimon/openxr-vulkan-example`)
  - Focus: integration example from watch list.
  - Insight: good cross-check for OpenXR/Vulkan interoperability patterns.
  - Asset: ![GitHub watch list snapshot](../assets/technological_watch/GithHub%20Watch%20List.png)

### 2) Asset Loading and Runtime Dependencies

- **TinyObjLoader POC** (`etib-corp/tinyobjloader-poc`)
  - Focus: lightweight OBJ loading workflow.
  - Insight: minimal dependency path is useful for rapid prototyping and debugging.
  - Asset: ![TinyObjLoader repository snapshot](../assets/technological_watch/TinyOBJLoader%20POC.png)

- **Assimp POC** (`etib-corp/assimp-poc`)
  - Focus: richer model import ecosystem.
  - Insight: broader format support vs TinyObjLoader, at the cost of complexity.
  - Asset: ![Assimp repository snapshot](../assets/technological_watch/ASSIMP%20POC.png)

### 3) UI and Tooling Exploration

- **MicroUI POC** (`etib-corp/microui`)
  - Focus: lightweight immediate mode UI approach.
  - Insight: suitable for in-engine debugging interfaces where low overhead matters.
  - Asset: ![MicroUI repository snapshot](../assets/technological_watch/MICRO%20UI%20POC.png)

- **Skia and React Native Skia (watch list)**
  - Focus: modern 2D rendering/UI ecosystems.
  - Insight: promising for tool interfaces and non-core rendering surfaces.
  - Asset: ![GitHub watch list snapshot](../assets/technological_watch/GithHub%20Watch%20List.png)

## POCs and Repositories Tracked

| **Repository** | **Type** | **Role in Watch** | **Evidence** |
| --- | --- | --- | --- |
| `etib-corp/android-xr-poc` | Public fork | XR integration experimentation | ![Android XR POC](../assets/technological_watch/Android%20XR%20POC.png) |
| `etib-corp/tinyobjloader-poc` | Private | Lightweight asset loading tests | ![TinyObjLoader POC](../assets/technological_watch/TinyOBJLoader%20POC.png) |
| `etib-corp/VulkanBase` | Public fork | Vulkan architecture and performance base | ![VulkanBase repository](../assets/technological_watch/Vulkan%20Base%20Repository.png) |
| `etib-corp/assimp-poc` | Private | Multi-format import validation | ![Assimp POC](../assets/technological_watch/ASSIMP%20POC.png) |
| `etib-corp/microui` | Public fork | Immediate mode UI exploration | ![MicroUI POC](../assets/technological_watch/MICRO%20UI%20POC.png) |
| `etib-corp/cpp_poc_template` | Private template | Reusable C++ POC bootstrap | ![C++ POC template](../assets/technological_watch/POC%20Template%20Repository.png) |

## Contributions and External Monitoring

- **Community tracking source**: Vulkan Discord `#news` channel for ecosystem updates.
  - Noted references include ARM and LunarG communications.
  - Asset: ![Vulkan Discord news snapshot](../assets/technological_watch/Vulkan%20News%20Discord.png)
- **GitHub watchlist curation**: dedicated `XIDER` star list to centralize monitored repositories.
  - Asset: ![GitHub watch list snapshot](../assets/technological_watch/GithHub%20Watch%20List.png)

## Changelog

- **2026-04-09**
  - Added asset-backed watch notes for XR, rendering, model loading, and UI technologies.
  - Added first comparative analysis table from current POCs.
  - Added repository tracking table with direct evidence links.
  - Added community monitoring references (Discord + GitHub star watch list).
