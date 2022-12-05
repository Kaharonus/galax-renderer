#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 0) in vec3 aNormal;

out vec3 vNormal;
out vec3 vPosition;
out vec3 mvPosition;
out vec3 mvCamera;
out vec3 modelPosition;
out vec3 lightDir;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 transposeInverseModel;
uniform vec3 position;
uniform vec3 scale;
uniform vec3 rotation;

mat4 translate(vec3 t) {
    return mat4(1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    t.x, t.y, t.z, 1.0);
}

mat4 rotate(float angle, vec3 axis) {
    float c = cos(angle);
    float s = sin(angle);
    vec3 v = normalize(axis);
    vec3 t = (1.0 - c) * v;

    return mat4(c + t.x * v.x, t.x * v.y + s * v.z, t.x * v.z - s * v.y, 0.0,
    t.y * v.x - s * v.z, c + t.y * v.y, t.y * v.z + s * v.x, 0.0,
    t.z * v.x + s * v.y, t.z * v.y - s * v.x, c + t.z * v.z, 0.0,
    0.0, 0.0, 0.0, 1.0);
}

mat4 scaleV(vec3 s) {
    return mat4(s.x, 0.0, 0.0, 0.0,
    0.0, s.y, 0.0, 0.0,
    0.0, 0.0, s.z, 0.0,
    0.0, 0.0, 0.0, 1.0);
}






void main() {

    // Calculate model matrix in glsl

    mat4 model = mat4(1.0);
    mat4 translation = translate(position.xyz);
    mat4 rotation = rotate(0.0, vec3(0.0, 0.0, 1.0));
    mat4 scaling = scaleV(scale);
    model = translation * rotation * scaling;


    //Get position vector of the model matrix
    //vPosition = (model * vec4(aPos, 1)).xyz;
    mvPosition = (view * model * vec4(aPos, 1)).xyz;
    mvCamera = (view * model)[3].xyz;
    lightDir = -normalize((view * model)[2].xyz);
    vNormal = vec3(transposeInverseModel * vec4(aNormal, 1));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
