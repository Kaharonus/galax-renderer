#version 430 core

layout(triangles, equal_spacing, ccw) in;


out TE_OUT {
    vec4 position;
} te_out;



struct OutputPatch{
    vec3 WorldPos030;
    vec3 WorldPos021;
    vec3 WorldPos012;
    vec3 WorldPos003;
    vec3 WorldPos102;
    vec3 WorldPos201;
    vec3 WorldPos300;
    vec3 WorldPos210;
    vec3 WorldPos120;
    vec3 WorldPos111;

    vec3 WorldPos[3];
};


in patch OutputPatch tcData;

void main(){
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    float w = gl_TessCoord.z;

    float uPow3 = pow(u, 3);
    float vPow3 = pow(v, 3);
    float wPow3 = pow(w, 3);
    float uPow2 = pow(u, 2);
    float vPow2 = pow(v, 2);
    float wPow2 = pow(w, 2);

    vec3 tePosition = tcData.WorldPos300 * wPow3 +
    tcData.WorldPos030 * uPow3 +
    tcData.WorldPos003 * vPow3 +
    tcData.WorldPos210 * 3.0 * wPow2 * u +
    tcData.WorldPos120 * 3.0 * w * uPow2 +
    tcData.WorldPos201 * 3.0 * wPow2 * v +
    tcData.WorldPos021 * 3.0 * uPow2 * v +
    tcData.WorldPos102 * 3.0 * w * vPow2 +
    tcData.WorldPos012 * 3.0 * u * vPow2 +
    tcData.WorldPos111 * 6.0 * w * u * v;

    gl_Position = vec4(tePosition, 1.0);
    te_out.position = gl_Position;
}