#version 430

layout(location = 0) in vec3 aPos;
layout(location = 3) in vec3 aNormalSmooth;

out vec3 vNormalSmooth;
out vec3 vPosition;

void main() {
    vNormalSmooth = aNormalSmooth;
    vPosition = aPos;
}
