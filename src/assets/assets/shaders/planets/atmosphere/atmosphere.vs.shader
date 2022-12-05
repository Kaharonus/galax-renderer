#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 0) in vec3 aNormal;

out vec3 vPosition;
out vec3 viewspacePosition;
out vec3 viewspaceCamera;
out vec3 modelPosition;
out vec3 lightDirection;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;



uniform int lightCount;

struct Light{
    vec3 position;
    float intensity;
    vec3 color;
    float falloff;
};

layout (std430) buffer lightData{
    Light data[];
} lights;

void main() {
    mat4 viewModel = view * model;
    Light sun = lights.data[0];
    vec3 sunDirection = model[3].xyz - sun.position;

    viewspacePosition = (view * model * vec4(aPos, 1)).xyz;
    viewspaceCamera = viewModel[3].xyz;
    lightDirection = normalize(viewModel * vec4(sunDirection, 1)).xyz;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
