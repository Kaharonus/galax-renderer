#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;


in VS_OUT {
    vec3 position;
    float noise;
} gs_in[];

out vec3 gsPosition;
out vec3 gsNormal;
out float gsNoise;
out float gsPositionNoise;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main() {

    float v1Y = gs_in[0].position.y;
    float v2Y = gs_in[1].position.y;
    float v3Y = gs_in[2].position.y;

    mat4 mvp = projection * view * model;
    mat3 mod3 = mat3(model);

    vec3 v1 = gs_in[0].position;
    vec3 v2 = gs_in[1].position;
    vec3 v3 = gs_in[2].position;

    vec3 a = mod3 * v1;
    vec3 b = mod3 * v2;
    vec3 c = mod3 * v3;


    vec3 normal = normalize(cross(b - a, c - a));

    gl_Position = mvp * vec4(v1,1);
    gsPosition = v1.xyz;
    gsNormal = normal;
    gsNoise = gs_in[0].noise;
    gsPositionNoise = v1Y;
    EmitVertex();

    gl_Position = mvp * vec4(v2,1);
    gsPosition = v2.xyz;
    gsNormal = normal;
    gsNoise = gs_in[1].noise;
    gsPositionNoise = v2Y;
    EmitVertex();

    gl_Position = mvp * vec4(v3,1);
    gsPosition = v3.xyz;
    gsNormal = normal;
    gsNoise = gs_in[2].noise;
    gsPositionNoise = v3Y;
    EmitVertex();

    EndPrimitive();
}
