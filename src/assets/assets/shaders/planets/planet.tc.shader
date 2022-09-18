#version 430 core

// define the number of CPs in the output patch
layout (vertices = 1) out;


// attributes of the input CPs
in vec3 vPosition[];
in vec3 vNormal[];
in vec3 vNormalSmooth[];

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


    vec3 NormalSmooth200;
    vec3 NormalSmooth020;
    vec3 NormalSmooth002;
    vec3 NormalSmooth110;
    vec3 NormalSmooth011;
    vec3 NormalSmooth101;

    vec3 Normal;
    vec3 WorldPos[3];
};



out patch OutputPatch tcData;

// attributes of the output CPs

vec3 ProjectToPlane(vec3 Point, vec3 PlanePoint, vec3 PlaneNormal){
    vec3 v = Point - PlanePoint;
    float Len = dot(v, PlaneNormal);
    vec3 d = Len * PlaneNormal;
    return (Point - d);
}

vec3 getH(int i, int j){
    vec3 A = vNormal[i] + vNormal[j];
    vec3 B = tcData.WorldPos[j] - tcData.WorldPos[i];
    float v = 2.0 * (dot(B, A) / dot(B, B));
    return (A/2) - ((v/2.0) * B);
}

vec3 getHSmooth(int i, int j){
    vec3 A = vNormalSmooth[i] + vNormalSmooth[j];
    vec3 B = tcData.WorldPos[j] - tcData.WorldPos[i];
    float v = 2.0 * (dot(B, A) / dot(B, B));
    return (A/2) - ((v/2.0) * B);
}



void CalcPositions(){

    // The original vertices stay the same
    tcData.WorldPos030 = vPosition[0];
    tcData.WorldPos003 = vPosition[1];
    tcData.WorldPos300 = vPosition[2];

    // Edges are names according to the opposing vertex
    vec3 Edge300 = tcData.WorldPos003 - tcData.WorldPos030;
    vec3 Edge030 = tcData.WorldPos300 - tcData.WorldPos003;
    vec3 Edge003 = tcData.WorldPos030 - tcData.WorldPos300;

    // Generate two midpoints on each edge
    tcData.WorldPos021 = tcData.WorldPos030 + Edge300 / 3.0;
    tcData.WorldPos012 = tcData.WorldPos030 + Edge300 * 2.0 / 3.0;
    tcData.WorldPos102 = tcData.WorldPos003 + Edge030 / 3.0;
    tcData.WorldPos201 = tcData.WorldPos003 + Edge030 * 2.0 / 3.0;
    tcData.WorldPos210 = tcData.WorldPos300 + Edge003 / 3.0;
    tcData.WorldPos120 = tcData.WorldPos300 + Edge003 * 2.0 / 3.0;

    // Project each midpoint on the plane defined by the nearest vertex and its normal
    tcData.WorldPos021 = ProjectToPlane(tcData.WorldPos021, tcData.WorldPos030, vNormal[0]);
    tcData.WorldPos012 = ProjectToPlane(tcData.WorldPos012, tcData.WorldPos003, vNormal[1]);
    tcData.WorldPos102 = ProjectToPlane(tcData.WorldPos102, tcData.WorldPos003, vNormal[1]);
    tcData.WorldPos201 = ProjectToPlane(tcData.WorldPos201, tcData.WorldPos300, vNormal[2]);
    tcData.WorldPos210 = ProjectToPlane(tcData.WorldPos210, tcData.WorldPos300, vNormal[2]);
    tcData.WorldPos120 = ProjectToPlane(tcData.WorldPos120, tcData.WorldPos030, vNormal[0]);

    // Handle the center
    vec3 Center = (tcData.WorldPos003 + tcData.WorldPos030 + tcData.WorldPos300) / 3.0;
    tcData.WorldPos111 = (tcData.WorldPos021 + tcData.WorldPos012 + tcData.WorldPos102 +
    tcData.WorldPos201 + tcData.WorldPos210 + tcData.WorldPos120) / 6.0;
    tcData.WorldPos111 += (tcData.WorldPos111 - Center) / 2.0;

}

void CalcNormals(){
    tcData.NormalSmooth200 =  vNormalSmooth[0];
    tcData.NormalSmooth020 =  vNormalSmooth[1];
    tcData.NormalSmooth002 =  vNormalSmooth[2];
    tcData.NormalSmooth110 = normalize(getHSmooth(0, 1));
    tcData.NormalSmooth011 = normalize(getHSmooth(1, 2));
    tcData.NormalSmooth101 = normalize(getHSmooth(2, 0));
}


void main(){
    float tessLevel = 2.0;
    // Set the control points of the output patch
    for (int i = 0; i < 3; i++) {
        tcData.WorldPos[i] = vPosition[i];
    }
    tcData.Normal = vNormal[0];


    CalcPositions();
    CalcNormals();
    // Calculate the tessellation levels
    gl_TessLevelOuter[0] = tessLevel;
    gl_TessLevelOuter[1] = tessLevel;
    gl_TessLevelOuter[2] = tessLevel;
    gl_TessLevelInner[0] = tessLevel;

}