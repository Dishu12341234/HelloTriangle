#version 450
layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

struct ObjectData {
    mat4 model;
    uint tileIndex;
};

layout(std430, binding = 2) buffer ObjectBuffer {
    ObjectData objects[];
} objectBuffer;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_Position = ubo.proj * ubo.view * objectBuffer.objects[gl_InstanceIndex].model * vec4(inPosition, 1.0);
    fragColor = inColor;

    // Compute UV offset from tileIndex
    uint index = objectBuffer.objects[gl_InstanceIndex].tileIndex;
    float tilesPerRow = 20.0;
    float tileSize = 1.0 / tilesPerRow;

    float x = float(index % 20u);
    float y = float(index / 20u);

    vec2 tileOffset = vec2(x, y) * tileSize;

    fragTexCoord = tileOffset + inTexCoord * tileSize;
}
