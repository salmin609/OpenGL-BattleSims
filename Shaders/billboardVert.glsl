#version 430

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

void main()
{
    mat4 mvmat = viewMat * modelMat;
    
    mvmat[0][0] = mvmat[1][1] = mvmat[2][2] = 1.f;
    mvmat[0][1] = mvmat[0][2] = mvmat[1][2] = 0.0;
    mvmat[1][0] = mvmat[2][0] = mvmat[2][1] = 0.0;

    gl_Position = projMat * mvmat * vec4(1.f, 1.f, 1.f, 1.f);
}