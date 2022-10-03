#version 430

in vec3 gsNormal;
in vec3 gsPosition;
in float gsNoise;
in float noiseY;
uniform sampler2D palette;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec3 gEmission;

vec3 getColor(){
    float x = abs(noiseY);
    float y = gsNoise * 2;
    vec3 t = texture(palette, vec2(x, y)).xyz;
    return t;
}


void main(){
    gAlbedo = vec4(getColor(), 1);
    gPosition = gsPosition;
    gNormal = gsNormal;
    gEmission = vec3(0);
}