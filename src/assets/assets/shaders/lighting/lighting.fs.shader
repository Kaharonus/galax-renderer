#version 430 core

out vec4 lightMap;
out vec4 bloomMap;
in vec2 texCoords;

layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gAlbedo;
layout(binding = 3) uniform sampler2D gEmission;

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


    vec3 vColor = texture(gAlbedo, texCoords).rgb;
    vec3 vEmission = texture(gEmission, texCoords).rgb;
    if(vEmission.x > 0 || vEmission.y > 0 || vEmission.z > 0) {
        lightMap = vec4(vEmission, 1.0);
        float brightness = dot(lightMap.xyz, vec3(0.2126, 0.7152, 0.0722));
        if(brightness > 1.0){
            bloomMap = vec4(lightMap.rgb, 1.0);
        }else{
            bloomMap = vec4(0);
        }
        return;
    }
    vec3 ambientColor = vec3(0.4, 0.4, 0.4);
    float ambientIntensity = 0.1;
    vec3 ambient = ambientColor * ambientIntensity;

    vec3 vPos = texture(gPosition, texCoords).rgb;
    vec3 vNormal = texture(gNormal, texCoords).rgb;

    vec3 diffuse = vec3(0.0);
    for(int i = 0; i < lightCount; i++){
        Light light = lights.data[i];
        diffuse += CalcPointLight(light, vPos, vNormal);
    }



    lightMap = vec4((ambient + diffuse) * vColor, 1.0);

    //Calculate the bloom map

}