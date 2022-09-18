#version 430 core

out vec4 fragColor;
in vec2 texCoords;

layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gAlbedo;

void main() {

    vec3 color = texture(gAlbedo, texCoords).rgb;
    vec3 vPos = texture(gPosition, texCoords).rgb;
    vec3 vNormal = texture(gNormal, texCoords).rgb;

    vec3 lightPos = vec3(0, 1.5, -3);
    vec3 ambientColor = vec3(0.4, 0.4, 0.4);
    vec3 diffuseColor = vec3(0.9, 0.9, 0.9);
    float ambientIntensity = 0.1;
    float diffuseIntensity = 1.0;

    vec3 ambient = ambientColor * ambientIntensity;

    vec3 norm = vNormal;
    //    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - vPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor * diffuseIntensity;

    fragColor = vec4((ambient + diffuse) * color, 1.0);
}