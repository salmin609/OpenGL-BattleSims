/*
 * Author		: Ryan Kim.
 * Date			: 2023-03-17
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#version 430

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 LocalPos0;
in vec3 fragPos;
out vec4 FragColor;

layout(binding = 0) uniform sampler2D gDiffuse;
layout(binding = 1) uniform sampler2D gSpecular;

void main()
{
	float ambientStrength = 0.5f;
	vec3 lightColor = vec3(1.5f, 1.5f, 1.5f);
	vec3 lightPos = fragPos + vec3(0.f, 10.f, 5.f);

	vec3 diffuseColor = texture2D(gDiffuse, TexCoord0.xy).rgb;

	vec3 ambient = ambientStrength * diffuseColor;

	vec3 norm = normalize(Normal0);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * diffuseColor;

	vec3 viewDir = vec3(0.f, 0.f, 1.f);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.f);
	vec3 specular = spec * texture(gSpecular, TexCoord0.xy).rgb;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0f);
}