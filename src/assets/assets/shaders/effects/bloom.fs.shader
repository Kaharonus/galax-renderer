#version 430 core

out vec4 bloom;

in vec2 texCoords;

uniform int currentCall;
uniform sampler2D bloomMap;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    vec2 tex_offset = 1.0 / textureSize(bloomMap, 0); // gets size of single texel
    vec3 result = texture(bloomMap, texCoords).rgb * weight[0]; // current fragment's contribution
    bool horizontal = bool(currentCall % 2);
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(bloomMap, texCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(bloomMap, texCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(bloomMap, texCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(bloomMap, texCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    bloom = vec4(result, 1.0);
}