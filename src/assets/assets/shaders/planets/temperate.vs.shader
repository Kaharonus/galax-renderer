#version 430

layout(location = 0) in vec3 aPos;

out vec3 vPosition;
void main() {
    vPosition = aPos;
}
