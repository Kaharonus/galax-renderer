#version 430 core

layout(triangles, equal_spacing, ccw) in;

out vec3 tePosition;
in vec3 tcPosition[];


void main(){
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    float w = gl_TessCoord.z;

    vec3 p0 = gl_TessCoord.x * tcPosition[0];
    vec3 p1 = gl_TessCoord.y * tcPosition[1];
    vec3 p2 = gl_TessCoord.z * tcPosition[2];
    tePosition = normalize(p0 + p1 + p2);

    gl_Position = vec4(tePosition, 1.0);
}