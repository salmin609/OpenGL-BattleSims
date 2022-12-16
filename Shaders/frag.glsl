#version 430

const int MAX_POINT_LIGHTS = 2;
const int MAX_SPOT_LIGHTS = 2;

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 LocalPos0;

out vec4 FragColor;

struct Material
{
    vec3 AmbientColor;
    vec3 DiffuseColor;
    vec3 SpecularColor;
};

uniform Material gMaterial;
//uniform sampler2D gSampler;
uniform sampler2D gSampler;
//layout(binding = 1) uniform sampler2D gSpecular;
uniform vec3 gCameraLocalPos;
uniform int gDisplayBoneIndex;

uniform int displayTexture;

void main()
{
    vec3 Normal = normalize(Normal0);
    bool found = false;

    //Means None
    if (displayTexture == 0)
    {
        FragColor = vec4(1.f, 1.f, 1.f, 1.f);
    }
    else
    {
        FragColor = texture2D(gSampler, TexCoord0.xy);
    }

}