#version 430

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 3) in vec3 aNormalSmooth;

out vec3 vNormalSmooth;
out vec3 vNormal;
out vec3 vPosition;


uniform float seed;
uniform mat4 model;



void main() {
    vNormal = aNormal;
    vNormalSmooth = aNormalSmooth;
    //vPosition = aPos;
    vPosition = vec3( vec4(aPos, 1.0));
    //vNoise = noise;

}
