#version 450
layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

struct ObjectData {
    mat4 model;
    uint tileIndex[6];
};

layout(push_constant) uniform PushConstantC1
{
    mat4 model;
} pc;

layout(std430, binding = 2) buffer ObjectBuffer {
    ObjectData objects[];
} objectBuffer;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in uint face;
layout(location = 3) in uint vertexIndex;


layout(location = 0) out vec3 fragColor;

void main() {
    gl_PointSize = 5.0; 
    mat4 scaledModel = pc.model;

    // Multiply only the translation (last column, xyz) by 2
    //scaledModel[3].xyz *= 2.0;

    gl_Position = ubo.proj * ubo.view * scaledModel * vec4(inPosition, 1.0);
    fragColor = inColor;
}