#version 430

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 vPosition;
out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transposeInverseModel;
uniform vec3 scale;

void main() {
    mat4 modelView = view * model;

    // Column 0:
    modelView[0][0] = 0;
    modelView[0][1] = scale.x;
    modelView[0][2] = 0;

    // Column 1:
    modelView[1][0] = scale.y;
    modelView[1][1] = 0;
    modelView[1][2] = 0;

    // Column 2:
    modelView[2][0] = 0;
    modelView[2][1] = 0;
    modelView[2][2] = scale.z;


    gl_Position = projection * modelView * vec4(aPos, 1.0);
    uv = vec2(aPos.x + 1, aPos.y + 1) *0.5;
}
