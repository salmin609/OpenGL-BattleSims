#version 430

//layout(location = 0) in vec3 position;
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 tex;

//vec2 texCoords[] = {
//        vec2(1.f, 1.f),
//        vec2(1.f, 0.f),
//        vec2(0.f, 0.f),
//        vec2(0.f, 1.f)
//};

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;
uniform float scalePart;
out vec2 texCoord;

void main()
{
    //texCoord = texCoords[gl_VertexID];
    texCoord = tex;
    

    mat4 mvmat = viewMat * modelMat;
    
    mvmat[0][0] = mvmat[1][1] = mvmat[2][2] = scalePart;//size;
    mvmat[0][1] = mvmat[0][2] = mvmat[1][2] = 0.0;
    mvmat[1][0] = mvmat[2][0] = mvmat[2][1] = 0.0;

    //gl_Position = gWVP * vec4(position, 1.f);
    //vec4 gPos = gWVP * vec4(position, 1.f);
    //gPos /= gPos.w;

    gl_Position = projMat * mvmat * vec4(position, 0.f, 1.f);
}