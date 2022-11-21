#version 430 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D lightMap;
uniform sampler2D bloomResult;
uniform sampler2D outlineResult;


float luminance(vec3 v)
{
    return dot(v, vec3(0.2126, 0.7152, 0.0722));
}

vec3 change_luminance(vec3 c_in, float l_out)
{
    float l_in = luminance(c_in);
    return c_in * (l_out / l_in);
}

vec3 reinhard_extended_luminance(vec3 v, float max_white_l)
{
    float l_old = luminance(v);
    float numerator = l_old * (1.0f + (l_old / (max_white_l * max_white_l)));
    float l_new = numerator / (1.0f + l_old);
    return change_luminance(v, l_new);
}

void main(){
    float exposure = 1;
    const float gamma = 1;
    vec3 hdrColor = texture(lightMap, texCoords).rgb;
    vec3 bloom = texture(bloomResult, texCoords).rgb;
    vec3 outline = texture(outlineResult, texCoords).rgb;
    hdrColor += bloom;
    hdrColor += outline;
    vec3 mapped = reinhard_extended_luminance(hdrColor, 2);
    // exposure tone mapping
    mapped = vec3(1.0) - exp(-mapped * exposure);
    // gamma correction
    mapped = pow(mapped, vec3(1.0 / gamma));
    FragColor = vec4(mapped, 1.0);
}