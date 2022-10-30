#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;


in VS_OUT {
    vec3 position;
} gs_in[];

out vec3 gsPosition;
out vec3 gsNormal;


uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


void main() {
    mat4 mvp = projection * view * model;
    vec4 v1 = (mvp * vec4(gs_in[0].position, 1.0));
    vec4 v2 = (mvp * vec4(gs_in[1].position, 1.0));
    vec4 v3 = (mvp * vec4(gs_in[2].position, 1.0));

    vec3 normal = normalize(cross(v2.xyz - v1.xyz, v3.xyz - v1.xyz));

    gl_Position = v1;
    gsPosition = v1.xyz;
    gsNormal = normal;
    EmitVertex();

    gl_Position = v2;
    gsPosition = v2.xyz;
    gsNormal = normal;
    EmitVertex();

    gl_Position = v3;
    gsPosition = v3.xyz;
    gsNormal = normal;
    EmitVertex();

    EndPrimitive();
}
