---
name: vulkan-spec
description: Provides specialized knowledge and workflows for working with the Vulkan graphics API, including specification lookups, best practices, and common patterns.
---

# Vulkan Specification Skill

This skill provides specialized assistance for working with the Vulkan graphics and compute API.

## When to Use This Skill

Use this skill when the user:

- Asks about Vulkan API functions, structures, or enums
- Needs help understanding Vulkan specification details
- Wants to implement Vulkan features (rendering, compute, ray tracing)
- Needs validation layer guidance or debugging help
- Asks about Vulkan extensions or features
- Wants best practices for Vulkan development
- Needs help with synchronization, memory management, or resource creation

## Vulkan Specification Reference

**Official Specification:** https://registry.khronos.org/vulkan/specs/latest/html/vkspec.html

**Key Resources:**
- Vulkan Specification: https://registry.khronos.org/vulkan/specs/latest/html/vkspec.html
- Vulkan Registry (XML): https://github.com/KhronosGroup/Vulkan-Docs
- Vulkan Validation Layers: https://github.com/KhronosGroup/Vulkan-ValidationLayers
- Vulkan Samples: https://github.com/KhronosGroup/Vulkan-Samples
- Vulkan Memory Allocator: https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator

## Common Vulkan Tasks

### 1. Specification Lookups

When users ask about specific Vulkan functionality:

1. **Identify the API element**: function, structure, enum, or feature
2. **Check the specification** for:
   - Valid usage requirements
   - Parameter constraints
   - Return values and error conditions
   - Threading and synchronization requirements
3. **Provide relevant VUIDs** (Valid Usage IDs) when applicable

Example response structure:
```
According to the Vulkan specification [section link]:

**Function:** `vkCreateBuffer`

**Valid Usage:**
- VUID-vkCreateBuffer-size-00911: size must be greater than 0
- VUID-vkCreateBuffer-flags-00915: flags must be a valid combination...

**Example:**
[code example]
```

### 2. Common Vulkan Patterns

#### Instance and Device Creation
```cpp
// Create Vulkan instance
VkInstanceCreateInfo createInfo{};
createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
// Add extensions, validation layers, etc.
vkCreateInstance(&createInfo, nullptr, &instance);

// Create logical device
VkDeviceCreateInfo deviceCreateInfo{};
deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
// Add queue families, features, extensions
vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device);
```

#### Synchronization Pattern
```cpp
// Semaphore for queue synchronization
VkSemaphoreCreateInfo semaphoreInfo{};
semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphore);

// Fence for CPU-GPU synchronization
VkFenceCreateInfo fenceInfo{};
fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
vkCreateFence(device, &fenceInfo, nullptr, &fence);
```

#### Memory Allocation Pattern
```cpp
// Query memory requirements
VkMemoryRequirements memRequirements;
vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

// Find suitable memory type
uint32_t memoryTypeIndex = findMemoryType(
    memRequirements.memoryTypeBits,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
);

// Allocate memory
VkMemoryAllocateInfo allocInfo{};
allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
allocInfo.allocationSize = memRequirements.size;
allocInfo.memoryTypeIndex = memoryTypeIndex;
vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory);
```

### 3. Validation and Debugging

**Enable Validation Layers:**
```cpp
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
debugCreateInfo.messageSeverity = 
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
debugCreateInfo.messageType = 
    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
debugCreateInfo.pfnUserCallback = debugCallback;
```

**Common Validation Errors:**
- **VUID-***: Valid Usage ID violations
- **UNASSIGNED-***: Unassigned validation messages
- **SYNC-***: Synchronization validation errors

### 4. Extension and Feature Queries

```cpp
// Check for extension support
uint32_t extensionCount = 0;
vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
std::vector<VkExtensionProperties> extensions(extensionCount);
vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

// Check for device features
VkPhysicalDeviceFeatures deviceFeatures;
vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

// Modern feature querying (Vulkan 1.1+)
VkPhysicalDeviceFeatures2 features2{};
features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
// Chain extension-specific feature structures
vkGetPhysicalDeviceFeatures2(physicalDevice, &features2);
```

## Vulkan Version-Specific Guidance

### Vulkan 1.0
- Base specification
- Core synchronization primitives
- Basic rendering and compute

### Vulkan 1.1
- Multi-device support
- Protected memory
- Shader draw parameters
- YCbCr conversion

### Vulkan 1.2
- Timeline semaphores
- Buffer device address
- Descriptor indexing
- Vulkan Memory Model

### Vulkan 1.3
- Dynamic rendering
- Synchronization 2
- Format features
- Maintenance extensions promoted to core

### Vulkan Extensions
- **KHR**: Khronos ratified, widely supported
- **EXT**: Vendor-neutral, may become KHR
- **Vendor-specific**: NV, AMD, EXT (vendor extensions)

## Common Vulkan Workflows

### 1. Basic Rendering Pipeline
1. Create instance and debug messenger
2. Enumerate physical devices
3. Create logical device and queues
4. Create swapchain
5. Create image views
6. Create render pass
7. Create graphics pipeline
8. Create framebuffers
9. Create command pools and buffers
10. Create synchronization objects
11. Record and submit commands
12. Present images

### 2. Compute Pipeline
1. Create instance and device
2. Create compute pipeline
3. Create storage buffers
4. Create descriptor set layout and pool
5. Allocate and update descriptor sets
6. Create command buffer
7. Bind pipeline and descriptors
8. Dispatch compute work
9. Synchronize and read results

### 3. Ray Tracing (VK_KHR_ray_tracing_pipeline)
1. Check ray tracing extension support
2. Create acceleration structures (BLAS, TLAS)
3. Create ray tracing pipeline
4. Create shader binding table
5. Dispatch rays

## Best Practices

### Performance
- Minimize pipeline state changes
- Use descriptor sets efficiently
- Batch command buffer recording
- Use appropriate memory types
- Implement proper synchronization (avoid over-synchronization)

### Debugging
- Always enable validation layers during development
- Use RenderDoc or Nsight Graphics for debugging
- Check return values of all Vulkan calls
- Use VK_EXT_debug_utils for object naming

### Portability
- Check for extension support before using
- Handle different GPU vendors appropriately
- Test on multiple hardware configurations
- Use Vulkan Portability subset for macOS

## Error Handling Pattern

```cpp
#define VK_CHECK(x)                                                     \
    do {                                                                \
        VkResult err = x;                                               \
        if (err) {                                                      \
            fprintf(stderr, "Vulkan error: %s:%d, code %d\n",           \
                    __FILE__, __LINE__, err);                           \
            abort();                                                    \
        }                                                               \
    } while (0)
```

## Related Skills

- `openxr-spec`: For OpenXR VR/AR development
- `glsl-shaders`: For shader development
- `graphics-debugging`: For graphics debugging tools

## Updates

The Vulkan specification is regularly updated. Always refer to the latest version at:
https://registry.khronos.org/vulkan/specs/latest/html/vkspec.html
