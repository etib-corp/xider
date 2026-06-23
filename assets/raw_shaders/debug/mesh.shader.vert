#version 450

layout(binding = 0) uniform UBO {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inUV;

layout(location = 0) out vec3 fragColor;

void main()
{
    gl_Position =
        ubo.proj *
        ubo.view *
        ubo.model *
        vec4(inPosition, 1.0);

    vec3 debugPalette[3] = vec3[](
        vec3(0.1, 1.0, 0.2),
        vec3(0.0, 1.0, 0.8),
        vec3(0.6, 1.0, 0.2)
    );

    fragColor = debugPalette[gl_VertexIndex % 3];
}