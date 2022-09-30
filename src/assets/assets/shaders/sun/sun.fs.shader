#version 430

in vec3 vNormal;
in vec3 vPosition;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec3 gEmission;

void main(){
    gAlbedo = vec4(0.97, 0.84, 0.09, 1);
    gPosition = vPosition;
    gNormal = vNormal;
    gEmission = vec3(5) * gAlbedo.rgb;
}