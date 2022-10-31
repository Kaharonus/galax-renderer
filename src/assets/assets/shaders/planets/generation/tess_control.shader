#version 430 core

// define the number of CPs in the output patch
layout (vertices = 3) out;


in vec3 vPosition[];

out vec3 tcPosition[];


// attributes of the output CPs

void main(){
    float tessLevel = 2.0;
    // Set the control points of the output patch

    tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];

    // Calculate the tessellation levels
    if (gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = tessLevel;
        gl_TessLevelOuter[1] = tessLevel;
        gl_TessLevelOuter[2] = tessLevel;
        if(tessLevel > 1){
            gl_TessLevelInner[0] = tessLevel + 1;
        }else{
            gl_TessLevelInner[0] = tessLevel;
        }
    }

}