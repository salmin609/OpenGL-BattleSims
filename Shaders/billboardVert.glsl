/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#version 430

uniform mat4 projMat;
uniform mat4 viewMat;
uniform vec4 boPos;


mat4 identityMat = mat4(1.0, 0.0, 0.0, 0.0,  // 1. column
    0.0, 1.0, 0.0, 0.0,  // 2. column
    0.0, 0.0, 1.0, 0.0,  // 3. column
    0.0, 0.0, 0.0, 1.0); // 4. column

mat4 TranslationMatrix(vec4 translationFactor)
{
    mat4 result = identityMat;

    result[3][0] = translationFactor.x;
    result[3][1] = translationFactor.y;
    result[3][2] = translationFactor.z;

    return result;
}

void main()
{
    mat4 modelMat = TranslationMatrix(boPos);
    mat4 mvmat = viewMat * modelMat;
    
    mvmat[0][0] = mvmat[1][1] = mvmat[2][2] = 1.f;
    mvmat[0][1] = mvmat[0][2] = mvmat[1][2] = 0.0;
    mvmat[1][0] = mvmat[2][0] = mvmat[2][1] = 0.0;

    gl_Position = projMat * mvmat * vec4(1.f, 1.f, 1.f, 1.f);
}