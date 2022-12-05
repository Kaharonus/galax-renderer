#version 430

in vec2 texCoords;

out vec4 color;

uniform sampler2D lightMap;
uniform vec2 resolution;

#define FXAA_SPAN_MAX 8.0
#define FXAA_REDUCE_MUL   (1.0/FXAA_SPAN_MAX)
#define FXAA_REDUCE_MIN   (1.0/128.0)
#define FXAA_SUBPIX_SHIFT (1.0/4.0)


vec3 FxaaPixelShader( vec4 uv, sampler2D tex, vec2 rcpFrame) {

    vec3 rgbNW = texture(tex, uv.zw).xyz;
    vec3 rgbNE = texture(tex, uv.zw + vec2(1,0)*rcpFrame.xy).xyz;
    vec3 rgbSW = texture(tex, uv.zw + vec2(0,1)*rcpFrame.xy).xyz;
    vec3 rgbSE = texture(tex, uv.zw + vec2(1,1)*rcpFrame.xy).xyz;
    vec3 rgbM  = texture(tex, uv.xy, 0.0).xyz;

    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM,  luma);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    float dirReduce = max(
    (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),
    FXAA_REDUCE_MIN);
    float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);

    dir = min(vec2( FXAA_SPAN_MAX,  FXAA_SPAN_MAX),
    max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
    dir * rcpDirMin)) * rcpFrame.xy;

    vec3 rgbA = (1.0/2.0) * (
    texture(tex, uv.xy + dir * (1.0/3.0 - 0.5)).xyz +
    texture(tex, uv.xy + dir * (2.0/3.0 - 0.5)).xyz);
    vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (
    texture(tex, uv.xy + dir * (0.0/3.0 - 0.5)).xyz +
    texture(tex, uv.xy + dir * (3.0/3.0 - 0.5)).xyz);

    float lumaB = dot(rgbB, luma);

    if((lumaB < lumaMin) || (lumaB > lumaMax)) return rgbA;

    return rgbB;
}

void main() {

    vec2 rcpFrame = 1./resolution.xy;
    vec2 uv2 = gl_FragCoord.xy / resolution.xy;
    vec4 uv = vec4( uv2, uv2 - (rcpFrame * (0.5 + FXAA_SUBPIX_SHIFT)));
    color = vec4(FxaaPixelShader( uv, lightMap, 1./resolution.xy ), 1);
}
