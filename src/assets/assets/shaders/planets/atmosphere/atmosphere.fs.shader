#version 430 core

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 emission;

in vec3 vPosition;
in vec3 vNormal;

uniform int lightCount;

struct Light{
    vec3 position;
    float intensity;
    vec3 color;
    float falloff;
};

layout (std430) buffer lightData{
    Light data[];
} lights;

vec3 CalcPointLight(Light light, vec3 fragPos, vec3 fragNormal){
    vec3 lightPos = light.position;
    vec3 diffuseColor = light.color * light.intensity;

    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(fragNormal, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor;
    return diffuse;
}



void main() {
    vec3 baseColor = vec3(135, 206, 235) / 255;

    vec3 diffuse = vec3(0);
    for(int i = 0; i < lightCount; i++){
        Light light = lights.data[i];
        diffuse += CalcPointLight(light, vPosition, vNormal);
    }

    color = vec4(baseColor * diffuse, 0.25);
    emission = color * 1;
}
