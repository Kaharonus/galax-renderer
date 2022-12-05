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


void main() {
    mat4 viewModel = view * model;

    viewspacePosition = (view * model * vec4(aPos, 1)).xyz;
    viewspaceCamera = viewModel[3].xyz;
    lightDirection = normalize(viewModel * vec4(model[3].xyz, 1)).xyz;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
