#version 430

layout(location = 0) in vec3 aPos;
layout(location = 1) in float aNoise;

out VS_OUT {
    vec3 position;
    float noise;
} gs_in;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
    mat4 mvp = projection * view * model;
    gl_Position = vec4(aPos, 1);
    gs_in.position = gl_Position.xyz;
    gs_in.noise = aNoise;
}
