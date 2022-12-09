#version 430 core


in vec3 gsPosition;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec3 gEmission;

uniform sampler2D palette;


void main(){
    gAlbedo = vec4(1);
    gPosition = gsPosition;
    gNormal = vec3(0);
    gEmission = vec3(1);
}