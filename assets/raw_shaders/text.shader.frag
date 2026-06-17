#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(binding = 4) uniform sampler2D fontAtlas;

void main()
{
    float coverage = texture(fontAtlas, fragUV).r;

    float alpha = smoothstep(0.15, 0.85, coverage);

    outColor = vec4(fragColor, alpha);
}