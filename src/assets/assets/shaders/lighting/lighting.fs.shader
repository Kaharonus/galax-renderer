#version 430 core

out vec4 lightMap;
out vec4 bloomMap;
in vec2 texCoords;

layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gAlbedo;
layout(binding = 2) uniform sampler2D gEmission;


void main() {
    vec3 color = texture(gAlbedo, texCoords).rgb;
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
    vec3 vPos = texture(gPosition, texCoords).rgb;
    vec3 vNormal = texture(gNormal, texCoords).rgb;


    vec3 lightPos = vec3(0, 1.5, -3);
    vec3 ambientColor = vec3(0.4, 0.4, 0.4);
    vec3 diffuseColor = vec3(0.9, 0.9, 0.9);
    float ambientIntensity = 0.1;
    float diffuseIntensity = 1.0;

    vec3 ambient = ambientColor * ambientIntensity;

    vec3 norm = vNormal;
    vec3 lightDir = normalize(lightPos - vPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor * diffuseIntensity;

    lightMap = vec4((ambient + diffuse) * color, 1.0);

    //Calculate the bloom map

}