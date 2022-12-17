#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 TexCoord0;
out vec3 Normal0;
out vec3 LocalPos0;
out vec3 fragPos;

uniform mat4 gWVP;
uniform int transformIndex;
uniform float timeTicks;
uniform float instancingOffset;
uniform mat4 model;


layout(std430, binding = 0) buffer boneDatas
{
    int bones[];
};
layout(std430, binding = 1) buffer weightDatas
{
    float weights[];
};
layout(std430, binding = 2) buffer indexStartDatas
{
    int indexStart[];
};
layout(std430, binding = 3) buffer indexEndDatas
{
    int indexEnd[];
};

layout(std430, binding = 4) buffer transformDatas{
    mat4 gBones[];
};

void main()
{
    int startIndex = indexStart[gl_VertexID];
    int endIndex = indexEnd[gl_VertexID];

    mat4 boneTransform = gBones[bones[startIndex] + transformIndex] * weights[startIndex];

    for (int i = startIndex + 1; i < endIndex; ++i)
    {
        int boneId = bones[i];
        float weight = weights[i];

        boneTransform += gBones[boneId + transformIndex] * weight;
    }

    vec4 posL = boneTransform * vec4(position, 1.0);
    posL.z += (gl_InstanceID * instancingOffset);

    gl_Position = gWVP * posL;
    fragPos = vec3(model * vec4(position, 1.0));

    TexCoord0 = texCoord;
    Normal0 = normal;
    LocalPos0 = position;
}