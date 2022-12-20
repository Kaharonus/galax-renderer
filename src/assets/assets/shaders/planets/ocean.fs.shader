#version 430 core


in vec3 gsPosition;
in vec3 gsNormal;
in float gsNoise;
in float gsPositionNoise;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec3 gEmission;
layout (location = 4) out vec4 gMetadata;


uniform sampler2D palette;

uniform int objectId;
uniform float colorOffset;

vec3 getColor(){
    float x = abs(gsPositionNoise * 0.5);
    float y = gsNoise;
    vec3 t = texture(palette, vec2(x, y)).xyz;
    return t;
}


void main(){
    gAlbedo = vec4(getColor(), 1);
    gPosition = gsPosition;
    gNormal = normalize(gsNormal);
    gEmission = vec3(0);
    gMetadata = vec4(objectId,colorOffset,0,0);
}