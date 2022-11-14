#version 430 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D gMetadata;

void main(){
    vec4 metadata = texture(gMetadata, texCoords);
    FragColor = vec4(metadata.r, metadata.g, metadata.b, 1.0);
}