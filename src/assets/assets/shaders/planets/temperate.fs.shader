#version 430

in vec3 tePosition;
in vec3 teNormal;
in float vNoise;
uniform sampler2D palette;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

vec3 getColor(){
    float x = abs(tePosition.y);
    float y = vNoise / 0.045;
    vec3 t = texture(palette, vec2(x, y)).xyz;
    return t;
}


void main(){


    gAlbedo = vec4(1.0);
    gPosition = tePosition;
    gNormal = teNormal;

    //fragColor = vec4(vNormal,1.0);
    //fragColor = vec4(1.0) * vNoise;
}