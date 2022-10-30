#version 430 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in TE_OUT {
    vec4 position;
} gs_in[];


out vec3 gsNormal;
out float noiseY;
out vec3 gsPosition;
out float gsNoise;

out vec3 feedbackPosition;
out vec3 feedbackNormal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


struct Noise{
    float roughness;
    float strength;
};


//Simplex noise implementation by Ian McEwan, Ashima Arts
vec4 permute(vec4 x){return mod(((x*34.0)+1.0)*x, 289.0);}
vec4 taylorInvSqrt(vec4 r){return 1.79284291400159 - 0.85373472095314 * r;}

float snoise(vec3 v){
    const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
    const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

    // First corner
    vec3 i  = floor(v + dot(v, C.yyy) );
    vec3 x0 =   v - i + dot(i, C.xxx) ;

    // Other corners
    vec3 g = step(x0.yzx, x0.xyz);
    vec3 l = 1.0 - g;
    vec3 i1 = min( g.xyz, l.zxy );
    vec3 i2 = max( g.xyz, l.zxy );

    //  x0 = x0 - 0. + 0.0 * C
    vec3 x1 = x0 - i1 + 1.0 * C.xxx;
    vec3 x2 = x0 - i2 + 2.0 * C.xxx;
    vec3 x3 = x0 - 1. + 3.0 * C.xxx;

    // Permutations
    i = mod(i, 289.0 );
    vec4 p = permute( permute( permute(
    i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
    + i.y + vec4(0.0, i1.y, i2.y, 1.0 ))
    + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

    // Gradients
    // ( N*N points uniformly over a square, mapped onto an octahedron.)
    float n_ = 1.0/7.0; // N=7
    vec3  ns = n_ * D.wyz - D.xzx;

    vec4 j = p - 49.0 * floor(p * ns.z *ns.z);  //  mod(p,N*N)

    vec4 x_ = floor(j * ns.z);
    vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

    vec4 x = x_ *ns.x + ns.yyyy;
    vec4 y = y_ *ns.x + ns.yyyy;
    vec4 h = 1.0 - abs(x) - abs(y);

    vec4 b0 = vec4( x.xy, y.xy );
    vec4 b1 = vec4( x.zw, y.zw );

    vec4 s0 = floor(b0)*2.0 + 1.0;
    vec4 s1 = floor(b1)*2.0 + 1.0;
    vec4 sh = -step(h, vec4(0.0));

    vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
    vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

    vec3 p0 = vec3(a0.xy,h.x);
    vec3 p1 = vec3(a0.zw,h.y);
    vec3 p2 = vec3(a1.xy,h.z);
    vec3 p3 = vec3(a1.zw,h.w);

    //Normalise gradients
    vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
    p0 *= norm.x;
    p1 *= norm.y;
    p2 *= norm.z;
    p3 *= norm.w;

    // Mix final noise value
    vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
    m = m * m;
    return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1),
    dot(p2,x2), dot(p3,x3)));
}

float noise(vec3 position, float roughness, float strength){
    float seed = 5;
    float noise = (strength * snoise((position + seed) * roughness));
    return (noise);
}

float evaluateNoise(vec3 position){
    //Noise settings[] = {{1, .05}};
    Noise settings[] = {{0.1, .06} , {1, 0.04}, {3, 0.025}, {10, 0.01}, {20, 0.005}, {50, 0.005},  {100, 0.001}, {200, 0.001}};
    //Noise settings[] = {{1, .1} , {5, 0.01}, {10, 0.01}, {20, 0.01}, {60, 0.005}, {1000, 0.001}};
    float noiseValue = 0;
    for(int i = 0; i < settings.length(); i++){
        Noise n = settings[i];
        noiseValue += noise(position, n.roughness, n.strength);
    }
    //return 0;
    return max(0.009, noiseValue);
    return noiseValue;
    //return clamp(noiseValue, 0.0, 0.0001);
}



void main(){
    float multiplier = 10;

    vec3 v1 = gs_in[0].position.xyz;
    vec3 v2 = gs_in[1].position.xyz;
    vec3 v3 = gs_in[2].position.xyz;

    float noiseA = evaluateNoise(v1);
    float noiseB = evaluateNoise(v2);
    float noiseC = evaluateNoise(v3);

    vec3 o1 = v1 * noiseA;
    vec3 o2 = v2 * noiseB;
    vec3 o3 = v3 * noiseC;

    v1 += o1;
    v2 += o2;
    v3 += o3;
    mat3 mod3 = mat3(model);

    vec3 a = mod3 * v1;
    vec3 b = mod3 * v2;
    vec3 c = mod3 * v3;


    vec3 normal = normalize(cross(b-a, c-a));

    gl_Position = projection * view * model * vec4(v1, 1);
    gsPosition = (model * vec4(v1, 1)).xyz;
    gsNoise = noiseA * multiplier;
    noiseY = v1.y;
    gsNormal = normal;
    EmitVertex();

    gl_Position = projection * view * model * vec4(v2, 1);
    gsPosition = (model * vec4(v2, 1)).xyz;
    noiseY = v2.y;
    gsNoise = noiseB * multiplier;
    gsNormal = normal;
    EmitVertex();

    gl_Position = projection * view * model * vec4(v3, 1);
    gsPosition = (model * vec4(v3, 1)).xyz;
    noiseY = v2.y;
    gsNoise = noiseC * multiplier;
    gsNormal = normal;
    EmitVertex();

    EndPrimitive();
}