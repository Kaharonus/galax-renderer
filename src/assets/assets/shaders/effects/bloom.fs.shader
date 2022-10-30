#version 430 core

out vec3 bloomResult;

in vec2 texCoords;

uniform vec2 direction;
uniform sampler2D bloomMap;

vec4 blur13(vec2 resolution) {

    vec4 color = vec4(0.0);
    vec2 off1 = vec2(1.411764705882353) * direction;
    vec2 off2 = vec2(4.2941176470588234) * direction;
    vec2 off3 = vec2(6.176470588235294) * direction;
    color += texture(bloomMap, texCoords) * 0.1964825501511404;
    color += texture(bloomMap, texCoords + (off1 / resolution)) * 0.2969069646728344;
    color += texture(bloomMap, texCoords - (off1 / resolution)) * 0.2969069646728344;
    color += texture(bloomMap, texCoords + (off2 / resolution)) * 0.09447039785044732;
    color += texture(bloomMap, texCoords - (off2 / resolution)) * 0.09447039785044732;
    color += texture(bloomMap, texCoords + (off3 / resolution)) * 0.010381362401148057;
    color += texture(bloomMap, texCoords - (off3 / resolution)) * 0.010381362401148057;
    return color;
}



void main(){
    vec2 resolution = textureSize(bloomMap, 0);
    bloomResult = blur13(resolution).rgb;
}