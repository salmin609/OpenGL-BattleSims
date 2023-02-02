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