#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 0) in vec3 aNormal;

out vec3 vPosition;
out vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transposeInverseModel;

void main() {
    vPosition = (model * vec4(aPos, 1)).xyz;
    vNormal = vec3(transposeInverseModel * vec4(aNormal, 1));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
