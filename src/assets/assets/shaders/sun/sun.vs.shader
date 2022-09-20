#version 430

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 vNormal;
out vec3 vPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transposeInverseModel;

void main() {
    vNormal = mat3(transposeInverseModel) * aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vPosition = gl_Position.xyz;
}
