#version 430 core

out vec3 FragColor;

in vec2 texCoords;

uniform sampler2D gMetadata;

void main(){
    vec2 texelSize = (1. / textureSize(gMetadata, 0));
    vec4 current = texture(gMetadata, texCoords);
    vec4 firstSample = texture(gMetadata, texCoords + texelSize);
    vec4 secondSample = texture(gMetadata, texCoords + texelSize * vec2(1,-1));
    if(current.r != firstSample.r || current.r != secondSample.r){
        FragColor = vec3(1,0,0) * ((secondSample.g + firstSample.g + current.g));
    }
}