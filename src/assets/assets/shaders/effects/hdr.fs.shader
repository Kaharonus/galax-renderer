#version 430 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D lightMap;
uniform sampler2D bloomResult;

void main(){
    float exposure = 1.0;
    const float gamma = 1.0;
    vec3 hdrColor = texture(lightMap, texCoords).rgb;
    vec3 bloom = texture(bloomResult, texCoords).rgb;
    hdrColor += bloom;

    // exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // gamma correction
    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped, 1.0);
}