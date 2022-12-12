#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 0) in vec3 aNormal;

out vec3 vPosition;
out vec3 viewspacePosition;
out vec3 viewspaceCamera;
out vec3 modelPosition;
out vec3 lightDirection;

out vec3 vNormal;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;


uniform vec3 cameraPosition;


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


mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
    oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
    oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
    0.0,                                0.0,                                0.0,                                1.0);
}

void main() {
    mat4 viewModel = view * model;
    Light sun = lights.data[0];
    vec3 sunDirection = model[3].xyz - sun.position;
    viewspacePosition = (view * model * vec4(aPos, 1)).xyz;
    viewspaceCamera = viewModel[3].xyz;
    lightDirection = normalize(viewModel * vec4(sunDirection, 1)).xyz;

    //Calculate normal between the center of the planet, the sun and the camera
    vec3 sunPos = sun.position;
    vec3 planetPos = model[3].xyz;
    vec3 cameraPos = cameraPosition;
    vec3 normal = normalize(cross(sunPos - planetPos, cameraPos - planetPos));

    //Find the angle of attack

    vec3 sunToPlanet = normalize(sunPos - planetPos);
    vec3 sunToCamera = normalize(sunPos - cameraPos);
    float angle = acos(dot(sunToPlanet, sunToCamera));
    mat4 rotation = rotationMatrix(normal, angle);
    lightDirection = normalize(viewModel * rotation * vec4(sunDirection, 0)).xyz;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
