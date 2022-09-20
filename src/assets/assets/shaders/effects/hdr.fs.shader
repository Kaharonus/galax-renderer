#version 430 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D lightMap;

void main()
{
    vec3 hdrColor = texture(lightMap, texCoords).rgb;
    FragColor = vec4(hdrColor, 1.0);
}