/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#version 430

in vec2 ex_TexCoor;

out vec4 FragColor;
uniform sampler2D gSampler;

void main()
{
    vec4 texColor = texture2D(gSampler, ex_TexCoor);
    if (texColor.a < 0.1)
        discard;
    FragColor = texColor;
}