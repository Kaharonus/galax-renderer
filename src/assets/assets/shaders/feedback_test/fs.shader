#version 430 core

in vec3 gsPosition;
in vec3 gsNormal;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec3 gEmission;

void main() {

    vec3 color = vec3(1);

    gAlbedo = vec4(color, 1.0);
    gPosition = gsPosition;
    gNormal = gsNormal;
    gEmission = color;
}
