#version 430

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 vsPosition;
out vec3 vsNormal;


uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
    mat4 mvp = projection * view * model;
    gl_Position = mvp * vec4(aPos, 1);
    vsPosition = gl_Position.xyz;
}
