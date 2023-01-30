#version 430 core

in vec3 position;
in vec3 texCoords;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec3 gEmission;



layout(binding=0)uniform samplerCube skybox;

void main()
{
    vec3 color = texture(skybox, texCoords).rgb;
    gAlbedo = vec4(color, 1.0);
    gPosition = position;
    gNormal = vec3(0.0);
    gEmission = color;
}