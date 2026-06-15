---
name: openxr-spec
description: Provides specialized knowledge and workflows for working with the OpenXR API for virtual reality (VR) and augmented reality (AR) applications.
---

# OpenXR Specification Skill

This skill provides specialized assistance for working with the OpenXR API for VR/AR development.

## When to Use This Skill

Use this skill when the user:

- Asks about OpenXR API functions, structures, or enums
- Needs help understanding OpenXR specification details
- Wants to implement XR features (session management, rendering, input)
- Needs guidance on XR runtime compatibility
- Asks about OpenXR extensions or layers
- Wants best practices for XR development
- Needs help with coordinate systems, spaces, or tracking

## OpenXR Specification Reference

**Official Specification:** https://registry.khronos.org/OpenXR/specs/1.1/html/xrspec.html

**Key Resources:**
- OpenXR Specification: https://registry.khronos.org/OpenXR/specs/1.1/html/xrspec.html
- OpenXR Registry: https://github.com/KhronosGroup/OpenXR-SDK-Source
- OpenXR Loader: https://github.com/KhronosGroup/OpenXR-SDK
- OpenXR Samples: https://github.com/KhronosGroup/OpenXR-SDK-Source/tree/main/src/tests
- OpenXR Tools: https://github.com/KhronosGroup/OpenXR-Tools

## Common OpenXR Tasks

### 1. Specification Lookups

When users ask about specific OpenXR functionality:

1. **Identify the API element**: function, structure, enum, or extension
2. **Check the specification** for:
   - Parameter requirements and constraints
   - Return values and error codes
   - Threading and lifecycle requirements
   - Extension dependencies
3. **Provide relevant section references** when applicable

Example response structure:
```
According to the OpenXR specification [section link]:

**Function:** `xrCreateSession`

**Parameters:**
- instance: A valid XR instance handle
- createInfo: Pointer to session creation info
- session: Pointer to store the session handle

**Return Codes:**
- XR_SUCCESS: Session created successfully
- XR_ERROR_INSTANCE_LOST: Instance is no longer valid
- XR_ERROR_SESSION_RUNNING: Session is already running

**Example:**
[code example]
```

### 2. Common OpenXR Patterns

#### Instance Creation
```cpp
// Create OpenXR instance
XrInstanceCreateInfo instanceCreateInfo{XR_TYPE_INSTANCE_CREATE_INFO};
instanceCreateInfo.applicationInfo = {
    .applicationVersion = 1,
    .engineVersion = 1,
    .apiVersion = XR_API_VERSION_1_0,
    .applicationName = "My XR App",
    .engineName = "My Engine"
};

// Add required extensions
const char* extensions[] = {
    XR_KHR_VULKAN_ENABLE_EXTENSION_NAME,
    XR_EXT_DEBUG_UTILS_EXTENSION_NAME
};
instanceCreateInfo.enabledExtensionCount = 2;
instanceCreateInfo.enabledExtensionNames = extensions;

XrInstance instance;
XrResult result = xrCreateInstance(&instanceCreateInfo, &instance);
```

#### Session and Space Creation
```cpp
// Create XR session (platform-specific)
XrSessionCreateInfo sessionCreateInfo{XR_TYPE_SESSION_CREATE_INFO};
sessionCreateInfo.next = &graphicsBinding; // Vulkan/D3D/OpenGL binding
sessionCreateInfo.systemId = systemId;

XrSession session;
xrCreateSession(instance, &sessionCreateInfo, &session);

// Create reference spaces
XrReferenceSpaceCreateInfo spaceCreateInfo{XR_TYPE_REFERENCE_SPACE_CREATE_INFO};
spaceCreateInfo.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_VIEW;
spaceCreateInfo.poseInReferenceSpace = {0, 0, 0, 1, 0, 0, 0};

XrSpace viewSpace;
xrCreateReferenceSpace(session, &spaceCreateInfo, &viewSpace);
```

#### Main XR Loop
```cpp
XrSessionBeginInfo sessionBeginInfo{XR_TYPE_SESSION_BEGIN_INFO};
sessionBeginInfo.primaryViewConfigurationType = 
    XR_PRIMARY_VIEW_CONFIGURATION_TYPE_MAIN_STEREO;
xrBeginSession(session, &sessionBeginInfo);

// Main loop
while (running) {
    // Poll events
    XrEventDataBuffer eventBuffer{XR_TYPE_EVENT_DATA_BUFFER};
    XrResult eventResult;
    do {
        eventResult = xrPollEvent(instance, &eventBuffer);
        if (eventResult == XR_SUCCESS) {
            // Handle event based on type
        }
    } while (eventResult == XR_SUCCESS);
    
    // Wait for frame
    XrFrameWaitInfo waitInfo{XR_TYPE_FRAME_WAIT_INFO};
    XrFrameState frameState{XR_TYPE_FRAME_STATE};
    xrWaitFrame(session, &waitInfo, &frameState);
    
    // Begin frame
    XrFrameBeginInfo beginInfo{XR_TYPE_FRAME_BEGIN_INFO};
    xrBeginFrame(session, &beginInfo);
    
    // Render views
    if (frameState.shouldRender) {
        XrViewLocateInfo viewLocateInfo{XR_TYPE_VIEW_LOCATE_INFO};
        viewLocateInfo.viewConfigurationType = 
            XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
        viewLocateInfo.displayTime = frameState.predictedDisplayTime;
        viewLocateInfo.space = viewSpace;
        
        XrViewState viewState{XR_TYPE_VIEW_STATE};
        uint32_t viewCapacityInput = 2;
        uint32_t viewCountOutput;
        XrView views[2] = {{XR_TYPE_VIEW}, {XR_TYPE_VIEW}};
        
        xrLocateViews(session, &viewLocateInfo, &viewState,
                     viewCapacityInput, &viewCountOutput, views);
        
        // Render to each view...
    }
    
    // End frame
    XrFrameEndInfo endInfo{XR_TYPE_FRAME_END_INFO};
    endInfo.displayTime = frameState.predictedDisplayTime;
    endInfo.environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
    endInfo.layerCount = 1;
    endInfo.layers = (const XrCompositionLayerBaseHeader* const*)&layer;
    
    xrEndFrame(session, &endInfo);
}

// End session
xrEndSession(session);
```

### 3. Input and Interaction

#### Action Set Creation
```cpp
// Create action set
XrActionSetCreateInfo actionSetInfo{XR_TYPE_ACTION_SET_CREATE_INFO};
strcpy(actionSetInfo.actionSetName, "gameplay");
strcpy(actionSetInfo.localizedActionSetName, "Gameplay");
actionSetInfo.priority = 0;

XrActionSet actionSet;
xrCreateActionSet(instance, &actionSetInfo, &actionSet);

// Create actions
XrActionCreateInfo actionInfo{XR_TYPE_ACTION_CREATE_INFO};
actionInfo.actionType = XR_ACTION_TYPE_BOOLEAN_INPUT;
strcpy(actionInfo.actionName, "click");
strcpy(actionInfo.localizedActionName, "Click");

XrAction clickAction;
xrCreateAction(actionSet, &actionInfo, &clickAction);

// Create action bindings
XrActionSuggestedBinding bindings[] = {
    {
        .action = clickAction,
        .binding = xrStringToPath(instance, "/user/hand/left/input/trigger")
    },
    {
        .action = clickAction,
        .binding = xrStringToPath(instance, "/user/hand/right/input/trigger")
    }
};

// Suggest bindings
XrInteractionProfileSuggestedBinding suggestedBindings{
    XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING
};
suggestedBindings.interactionProfile = xrStringToPath(
    instance, "/interaction_profiles/khr/simple_controller");
suggestedBindings.suggestedBindings = bindings;
suggestedBindings.countSuggestedBindings = 2;

xrSuggestInteractionProfileBindings(instance, &suggestedBindings);

// Attach action sets to session
XrSessionActionSetsAttachInfo attachInfo{
    XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO
};
attachInfo.countActionSets = 1;
attachInfo.actionSets = &actionSet;

xrAttachSessionActionSets(session, &attachInfo);
```

#### Polling Input State
```cpp
// Sync actions
XrActiveActionSet activeActionSet{actionSet, XR_NULL_PATH};
XrActionsSyncInfo syncInfo{XR_TYPE_ACTIONS_SYNC_INFO};
syncInfo.countActiveActionSets = 1;
syncInfo.activeActionSets = &activeActionSet;

xrSyncActions(session, &syncInfo);

// Get action state
XrActionStateGetInfo getInfo{XR_TYPE_ACTION_STATE_GET_INFO};
getInfo.action = clickAction;

XrActionStateBoolean clickState{XR_TYPE_ACTION_STATE_BOOLEAN};
xrGetActionStateBoolean(session, &getInfo, &clickState);

if (clickState.isActive && clickState.currentState) {
    // Trigger was pressed
}
```

### 4. Extension Handling

```cpp
// Check for extension support
uint32_t extensionCount;
xrEnumerateInstanceExtensionProperties(nullptr, 0, &extensionCount, nullptr);
std::vector<XrExtensionProperties> extensions(extensionCount);
for (auto& ext : extensions) {
    ext.type = XR_TYPE_EXTENSION_PROPERTIES;
}
xrEnumerateInstanceExtensionProperties(nullptr, extensionCount, 
                                        &extensionCount, extensions.data());

// Check if extension is available
bool hasExtension = false;
for (const auto& ext : extensions) {
    if (strcmp(ext.extensionName, XR_KHR_VULKAN_ENABLE_EXTENSION_NAME) == 0) {
        hasExtension = true;
        break;
    }
}

// Enable extension if available
if (hasExtension) {
    // Add to enabled extensions list
}
```

## OpenXR Version-Specific Guidance

### OpenXR 1.0
- Base specification
- Core session management
- Basic input system
- Standard view configurations

### OpenXR 1.1
- Hand tracking extension promoted to core
- Eye tracking support
- Face tracking support
- Improved composition layers
- Enhanced debugging capabilities

### Common Extensions

| Extension | Purpose | Status |
|-----------|---------|--------|
| XR_KHR_vulkan_enable | Vulkan graphics binding | Core in 1.0 |
| XR_KHR_D3D11_enable | D3D11 graphics binding | Core in 1.0 |
| XR_KHR_opengl_enable | OpenGL graphics binding | Core in 1.0 |
| XR_EXT_hand_tracking | Hand tracking | Core in 1.1 |
| XR_EXT_eye_gaze_interaction | Eye tracking | Extension |
| XR_KHR_composition_layer_depth | Depth layers | Extension |
| XR_KHR_composition_layer_cylinder | 360° video | Extension |
| XR_KHR_composition_layer_equirect | Equirectangular | Extension |
| XR_EXT_local_floor | Floor reference space | Extension |

## Common OpenXR Workflows

### 1. Application Initialization
1. Create instance with required extensions
2. Get system ID (xrGetSystem)
3. Query system properties
4. Create session with graphics binding
5. Create reference spaces
6. Setup input action sets
7. Begin session

### 2. Frame Loop
1. Poll events (xrPollEvent)
2. Wait for frame (xrWaitFrame)
3. Begin frame (xrBeginFrame)
4. Locate views (xrLocateViews)
5. Render to swapchain images
6. End frame (xrEndFrame)

### 3. Shutdown
1. End session (xrEndSession)
2. Destroy spaces
3. Destroy session
4. Destroy instance

## Coordinate Systems and Spaces

### Reference Space Types
- **XR_REFERENCE_SPACE_TYPE_VIEW**: Head-relative space
- **XR_REFERENCE_SPACE_TYPE_LOCAL**: Local floor-locked space
- **XR_REFERENCE_SPACE_TYPE_STAGE**: Room-scale tracked space
- **XR_REFERENCE_SPACE_TYPE_LOCAL_FLOOR**: Floor-relative (extension)

### Pose Structure
```cpp
typedef struct XrPosef {
    XrQuaternionf orientation;
    XrVector3f position;
} XrPosef;
```

### Space Relations
```cpp
XrSpaceLocation location{XR_TYPE_SPACE_LOCATION};
XrResult result = xrLocateSpace(actionSpace, baseSpace, time, &location);

if (XR_SUCCEEDED(result) && 
    (location.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) &&
    (location.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT)) {
    // Use location.pose
}
```

## Best Practices

### Performance
- Minimize xrPollEvent calls (once per frame is sufficient)
- Use predicted display time for rendering
- Batch state queries together
- Reuse spaces when possible
- Monitor frame timing with xrWaitFrame

### Compatibility
- Always check for extension support
- Provide fallbacks for missing features
- Test on multiple runtimes (SteamVR, Oculus, Windows Mixed Reality)
- Handle different form factors (head-mounted, handheld)

### Debugging
- Enable XR_EXT_debug_utils when available
- Log all XrResult codes
- Use runtime-specific debugging tools
- Validate action bindings at runtime

### User Experience
- Handle session loss gracefully (XR_EVENT_SESSION_LOSS_PENDING)
- Support visible space changes (XR_EVENT_VISIBLE_SPACE_CHANGED)
- Implement proper focus handling
- Provide recentering functionality

## Error Handling Pattern

```cpp
#define XR_CHECK(x)                                                     \
    do {                                                                \
        XrResult err = x;                                               \
        if (XR_FAILED(err)) {                                           \
            char errStr[XR_MAX_RESULT_STRING_SIZE];                     \
            xrResultToString(instance, err, errStr);                    \
            fprintf(stderr, "OpenXR error: %s:%d, %s\n",                \
                    __FILE__, __LINE__, errStr);                        \
            /* Handle error appropriately */                            \
        }                                                               \
    } while (0)
```

## Common Error Codes

| Code | Meaning |
|------|---------|
| XR_SUCCESS | Operation succeeded |
| XR_ERROR_INITIALIZATION_FAILED | Failed to initialize |
| XR_ERROR_INSTANCE_LOST | Instance is invalid |
| XR_ERROR_SESSION_RUNNING | Session is already running |
| XR_ERROR_SESSION_NOT_RUNNING | Session is not running |
| XR_ERROR_PATH_INVALID | Path is not valid |
| XR_ERROR_ACTIONSET_NOT_ATTACHED | Action set not attached |
| XR_ERROR_LIMITS_EXCEEDED | Resource limits exceeded |

## Related Skills

- `vulkan-spec`: For Vulkan graphics API integration
- `graphics-debugging`: For graphics debugging tools
- `spatial-math`: For 3D math and transformations

## Updates

The OpenXR specification is regularly updated. Always refer to the latest version at:
https://registry.khronos.org/OpenXR/specs/1.1/html/xrspec.html

## Platform-Specific Notes

### Windows (OpenXR Runtime)
- Windows Mixed Reality provides built-in OpenXR runtime
- SteamVR provides OpenXR runtime option
- Oculus provides OpenXR runtime for Quest (via Link)

### Linux
- Monado is the open-source OpenXR runtime
- SteamVR provides OpenXR support

### Android
- Various vendor runtimes (Qualcomm, ARM)
- Requires platform-specific session creation
