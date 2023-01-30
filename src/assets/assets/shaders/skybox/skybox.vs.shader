#version 430 core

layout (location = 0) in vec3 aPos;

out vec3 texCoords;
out vec3 position;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 cameraPosition;

void main()
{
    texCoords = aPos;
    vec4 pos = projection * view * vec4((aPos * 500) + cameraPosition, 1.0);
    position = pos.xyz;
    gl_Position = pos;
}