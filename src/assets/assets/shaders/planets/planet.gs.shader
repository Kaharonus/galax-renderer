#version 430 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in TE_OUT {
    vec4 position;
    float noise;
} gs_in[];


out vec3 gsNormal;
out vec3 gsPosition;
out float gsNoise;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main(){
    vec3 a = gs_in[0].position.xyz;
    vec3 b = gs_in[1].position.xyz;
    vec3 c = gs_in[2].position.xyz;
    vec3 normal = normalize(cross(b-a, c-a));
    normal = mat3(transpose(inverse(model))) * normal;

    gl_Position = projection * view * model * gs_in[0].position;
    gsNoise = gs_in[0].noise;
    gsPosition = a;
    gsNormal = normal;
    EmitVertex();

    gl_Position = projection * view * model * gs_in[1].position;
    gsPosition = b;
    gsNoise = gs_in[1].noise;
    gsNormal = normal;
    EmitVertex();

    gl_Position = projection * view * model * gs_in[2].position;
    gsPosition = c;
    gsNoise = gs_in[2].noise;
    gsNormal = normal;
    EmitVertex();

    EndPrimitive();
}