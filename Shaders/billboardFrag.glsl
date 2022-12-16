#version 430

in vec2 ex_TexCoor;

out vec4 FragColor;
uniform sampler2D gSampler;


void main()
{
    //FragColor = vec4(1.f, 1.f, 1.f, 1.f);

    vec4 texColor = texture2D(gSampler, ex_TexCoor);
    //vec4 texColor = vec4(1.f, 1.f, 1.f, 1.f);
    if (texColor.a < 0.1)
        discard;
    FragColor = texColor;
}