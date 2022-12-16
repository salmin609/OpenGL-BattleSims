/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include <glm/mat4x4.hpp>
#include <vector>

class Shader;
class Texture;


class SkyBox
{
public:
	SkyBox();
	~SkyBox();
	void Draw(glm::mat4 ndcMat, glm::mat4 camMat);
	void Draw(glm::mat4& ndcMat, glm::mat4& camMat);
	unsigned textureId;
private:
	unsigned skyboxVao;
	unsigned skyboxVbo;
	Shader* skyboxShader;

	//Texture* textures;
	std::vector<Texture*> textures;
};