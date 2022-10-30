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
uniform mat4 projection;
uniform mat4 view;


void main() {

    float v1Y = gs_in[0].position.y;
    float v2Y = gs_in[1].position.y;
    float v3Y = gs_in[2].position.y;

    mat4 mvp = projection * view * model;
    vec4 v1 = (mvp * vec4(gs_in[0].position, 1.0));
    vec4 v2 = (mvp * vec4(gs_in[1].position, 1.0));
    vec4 v3 = (mvp * vec4(gs_in[2].position, 1.0));

    vec3 normal = normalize(cross(v2.xyz - v1.xyz, v3.xyz - v1.xyz));

    gl_Position = v1;
    gsPosition = v1.xyz;
    gsNormal = normal;
    gsNoise = gs_in[0].noise;
    gsPositionNoise = v1Y;
    EmitVertex();

    gl_Position = v2;
    gsPosition = v2.xyz;
    gsNormal = normal;
    gsNoise = gs_in[1].noise;
    gsPositionNoise = v2Y;
    EmitVertex();

    gl_Position = v3;
    gsPosition = v3.xyz;
    gsNormal = normal;
    gsNoise = gs_in[2].noise;
    gsPositionNoise = v3Y;
    EmitVertex();

    EndPrimitive();
}
