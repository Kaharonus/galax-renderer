#version 430 core

layout(triangles, equal_spacing, ccw) in;

out vec3 tePosition;
out vec3 teNormal;

uniform mat4 view;
uniform mat4 projection;

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
    vec3 Normal[3];
    vec2 TexCoord[3];
};

in patch OutputPatch tcData;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}



vec3 getH(int i, int j){
    vec3 A = tcData.Normal[i] + tcData.Normal[j];
    vec3 B = tcData.WorldPos[j] - tcData.WorldPos[i];
    float v = 2.0 * (dot(B, A) / dot(B, B));
    return (A/2) - ((v/2.0) * B);
}

void main(){
    // Interpolate the attributes of the output vertex using the barycentric coordinates
    //TexCoord_FS_in = interpolate2D(patchData.TexCoord[0], patchData.TexCoord[1], patchData.TexCoord[2]);

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    float w = gl_TessCoord.z;

    float uPow3 = pow(u, 3);
    float vPow3 = pow(v, 3);
    float wPow3 = pow(w, 3);
    float uPow2 = pow(u, 2);
    float vPow2 = pow(v, 2);
    float wPow2 = pow(w, 2);

    tePosition = tcData.WorldPos300 * wPow3 +
    tcData.WorldPos030 * uPow3 +
    tcData.WorldPos003 * vPow3 +
    tcData.WorldPos210 * 3.0 * wPow2 * u +
    tcData.WorldPos120 * 3.0 * w * uPow2 +
    tcData.WorldPos201 * 3.0 * wPow2 * v +
    tcData.WorldPos021 * 3.0 * uPow2 * v +
    tcData.WorldPos102 * 3.0 * w * vPow2 +
    tcData.WorldPos012 * 3.0 * u * vPow2 +
    tcData.WorldPos111 * 6.0 * w * u * v;

    vec3 Normal200 =  tcData.Normal[0];
    vec3 Normal020 =  tcData.Normal[1];
    vec3 Normal002 =  tcData.Normal[2];
    vec3 Normal110 = normalize(getH(0, 1));
    vec3 Normal011 = normalize(getH(1, 2));
    vec3 Normal101 = normalize(getH(2, 0));

    teNormal =
    uPow2 * Normal200 +
    vPow2 * Normal020 +
    wPow2 * Normal002 +
    u * v * Normal110 +
    u * w * Normal101 +
    v * w * Normal011;
    teNormal = normalize(teNormal);
    gl_Position = projection * view * vec4(tePosition, 1.0);
    tePosition = (view * vec4(tePosition, 1.0)).xyz;
}