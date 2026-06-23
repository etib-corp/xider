#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(binding = 4) uniform sampler2D fontAtlas;

void main()
{
    float a = texture(fontAtlas, fragUV).r;
    float alpha = smoothstep(0.08, 0.35, a);

    if (alpha < 0.01)
        discard;

    outColor = vec4(fragColor, alpha);
}