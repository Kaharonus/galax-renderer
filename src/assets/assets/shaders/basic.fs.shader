#version 430

flat in vec3 vPos;
flat in vec3 vNormal;
in float vNoise;
out vec4 fragColor;


vec3 getColor(){

    if(vNoise < 0.02){
        return vec3(0,0,1);
    }

    if(vNoise < 0.025){
        return vec3(1,.9,.17);

    }

    if(vNoise < 0.04){
        return vec3(0,0.5,0);
    }

    if(vNoise < 0.05){
        return vec3(0.4,0.2,0.1);
    }

    return vec3(0.5);
    /*if(vNoise < 0.02){
        return vec3(0,0,1);
    }

    if(vNoise < 0.03){
        return vec3(1,.9,.17);

    }

    if(vNoise < 0.073){
        return vec3(0,0.5,0);
    }

    if(vNoise < 0.09){
        return vec3(0.4,0.2,0.1);
    }

    return vec3(0.9);*/



}


void main(){
    vec3 lightPos = vec3(0, 0, 0);
    vec3 ambientColor = vec3(0.4, 0.4, 0.4);
    vec3 diffuseColor = vec3(0.9, 0.9, 0.9);
    float ambientIntensity = 0.1;
    float diffuseIntensity = 3.0;

    vec3 ambient = ambientColor * ambientIntensity;

    vec3 norm = vNormal;
    //    vec3 norm = normalize(vNormal);
    vec3 lightDir =normalize(lightPos - vPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor * diffuseIntensity;

    fragColor = vec4((ambient + diffuse) * getColor(), 1.0);

    //fragColor = vec4(vNormal,1.0);
    //fragColor = vec4(1.0) * vNoise;
}