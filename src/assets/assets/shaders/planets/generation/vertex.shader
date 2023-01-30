#version 430 core


layout(location = 0) in vec3 aPos;

out vec3 vPosition;


void main() {
    gl_Position = vec4(aPos, 1);
    vPosition = gl_Position.xyz;
}

