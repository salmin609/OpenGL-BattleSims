/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for texture information.
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */


#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <fstream>




class Texture
{
public:
	Texture(GLenum textureTarget, const std::string fileName);
	void LoadEmptyTexture(int width, int height);
	bool Load();
	void Bind(GLenum textureUnit);
	void SaveImg();
	GLuint GetTextureObj();
private:
	int imageWidth, imageHeight, imageBPP;
	GLenum target;
	std::string file;

	GLuint textureObj;
};

class Image
{
public:
	unsigned char* Load_Image(std::string path, int& w, int& h, bool isFlip);
	

	struct Color_Ub {
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	};
private:

	std::vector<Color_Ub> color_datas;
	Color_Ub color_data;
	unsigned color_channel = 4;
	int image_w;
	int image_h;
	int image_channel;
	int pixel_size;
};