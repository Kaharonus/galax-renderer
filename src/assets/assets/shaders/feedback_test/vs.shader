#version 430 core


layout(location = 0) in vec3 aPos;

out VS_OUT {
    vec3 position;
} vs_out;

void main() {
    gl_Position = vec4(aPos, 1);
    vs_out.position = aPos;
}

