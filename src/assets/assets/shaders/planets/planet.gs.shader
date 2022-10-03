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
    vec3 a = (model * gs_in[0].position).xyz;
    vec3 b = (model * gs_in[1].position).xyz;
    vec3 c = (model * gs_in[2].position).xyz;
    vec3 normal = normalize(cross(b-a, c-a));

    gl_Position = projection * view * model * gs_in[0].position;
    gsNoise = gs_in[0].noise;
    gsPosition = gs_in[0].position.xyz;
    gsNormal = normal;
    EmitVertex();

    gl_Position = projection * view * model * gs_in[1].position;
    gsPosition = gs_in[1].position.xyz;
    gsNoise = gs_in[1].noise;
    gsNormal = normal;
    EmitVertex();

    gl_Position = projection * view * model * gs_in[2].position;
    gsPosition = gs_in[2].position.xyz;
    gsNoise = gs_in[2].noise;
    gsNormal = normal;
    EmitVertex();

    EndPrimitive();
}