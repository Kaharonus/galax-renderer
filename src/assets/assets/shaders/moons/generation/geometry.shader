#version 430 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;



in vec3 tePosition[];

uniform float inputSeed;

out vec3 gsPosition;
out float gsNoise;

//uniform float inputSeed;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

struct Noise{
    float roughness;
    float strength;
};


//Simplex noise implementation by Ian McEwan, Ashima Arts

float hash( in vec3 p )
{
    return fract(sin(p.x*15.32758341+p.y*39.786792357+p.z*59.4583127+7.5312) * 43758.236237153)-.5;
}

vec3 hash3( in vec3 p )
{
    return vec3(hash(p),hash(p+1.5),hash(p+2.5));
}


float voronoi3(in vec3 p)
{
    vec3 ip = floor(p);
    vec3 fp = fract(p);
    float v = .8;//cell variability <1.
    float va = 0.0;
    float wt = 0.0;
    for (int i=-1; i<=1; i++)
    for (int j=-1; j<=1; j++)
    for (int k=-1; k<=1; k++)
    {
        vec3 o = vec3(i, j, k)-.5;
        vec3 h = hash3((ip - o));
        vec3 pp = fp +o  -h;
        float d = dot(pp, pp);
        float w = exp(-d*4.5);
        wt +=w;

    }
    return pow(wt-.7,4.);
}



//concentric waves variant
float gavoronoi3b(in vec3 p)
{
    vec3 ip = floor(p);
    vec3 fp = fract(p);
    float f = 2.*3.14;
    float v = .8;
    float va = 0.0;
    float wt = 0.0;
    for (int i=-1; i<=1; i++)
    for (int j=-1; j<=1; j++)
    for (int k=-1; k<=1; k++)
    {
        vec3 o = vec3(i, j, k)-.5;
        vec3 pp = fp +o  - v*hash3(ip - o);
        float d = length(pp);
        float w = exp(-d*4.);
        wt +=w;
        va +=sin(sqrt(d)*f)*w;
    }
    return va/wt;
}



float fbmabs( vec3 p ) {

    float f=1.2;

    float r = 0.0;
    for(int i = 0;i<5;i++){
        r += abs(gavoronoi3b( p*f ))/f;
        f *=2.3;
    }
    return r/2.;
}

float fbm( vec3 p ) {

    float f=1.;

    float r = 0.0;
    for(int i = 0;i<3;i++){
        r += voronoi3( p*f )/f;
        f *=2.;
    }
    return r/4.;
}

float map(vec3 p){
    return fbmabs(p);
}

float noise(vec3 position, float roughness, float strength){
    float inputSeed = 5.;
    float noise = (strength * map((position + inputSeed) * roughness));
    return noise;
}

float evaluateNoise(vec3 position){
    Noise settings[] = {{0.75, .05} , {2, 0.1}, {3, 0.025}, {10, 0.01}};
    float noiseValue = 0;
    for(int i = 0; i < settings.length(); i++){
        Noise n = settings[i];
        noiseValue += noise(position, n.roughness, n.strength);
    }
    return -noiseValue;
}



void main(){
    float multiplier = 10;

    vec3 v1 = tePosition[0].xyz;
    vec3 v2 = tePosition[1].xyz;
    vec3 v3 = tePosition[2].xyz;

    float noiseA = evaluateNoise(v1);
    float noiseB = evaluateNoise(v2);
    float noiseC = evaluateNoise(v3);

    vec3 o1 = v1 * noiseA;
    vec3 o2 = v2 * noiseB;
    vec3 o3 = v3 * noiseC;

    v1 += o1;
    v2 += o2;
    v3 += o3;

    gl_Position = projection * view * model * vec4(v1, 1);
    gsPosition = v1;
    gsNoise = noiseA * multiplier;
    EmitVertex();

    gl_Position = projection * view * model *  vec4(v2, 1);
    gsPosition = v2;
    gsNoise = noiseB * multiplier;
    EmitVertex();

    gl_Position =  projection * view * model *  vec4(v3, 1);
    gsPosition = v3;
    gsNoise = noiseC * multiplier;
    EmitVertex();

    EndPrimitive();
}