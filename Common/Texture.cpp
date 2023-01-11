/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for texture information.
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "Texture.h"


#include <vector>

#define STB_IMAGE_IMPLEMENTATION

#include "stb/stb_image.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>

#include "stb/stb_image_write.h"

Texture::Texture(GLenum textureTarget, const std::string fileName)
{
	target = textureTarget;
	file = fileName;
}

void Texture::LoadEmptyTexture(int width, int height)
{
	imageBPP = 4;
	imageWidth = width;
	imageHeight = height;
	glGenTextures(1, &textureObj);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

bool Texture::Load()
{
	stbi_set_flip_vertically_on_load(1);

	unsigned char* imageData = stbi_load(file.c_str(), &imageWidth, &imageHeight, &imageBPP, 0);

	if(imageData)
	{
		glGenTextures(1, &textureObj);
		glBindTexture(target, textureObj);

		switch(imageBPP)
		{
		case 1:
			glTexImage2D(target, 0, GL_RED, imageWidth, imageHeight, 0, GL_RED, GL_UNSIGNED_BYTE, imageData);
			break;
		case 3:
			glTexImage2D(target, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			break;
		case 4:
			glTexImage2D(target, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
			break;
		default:
			break;
		}

		glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(target, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindTexture(target, 0);

		stbi_image_free(imageData);

		return true;
	}
	std::cout << "File does not exist" << file.c_str() << std::endl;
	return false;
}

void Texture::Bind(unsigned slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(target, textureObj);
}

void Texture::Bind(unsigned shaderId, unsigned slot, const std::string samplerName)
{
	const unsigned location = glGetUniformLocation(shaderId, samplerName.c_str());
	glUseProgram(shaderId);
	glUniform1i(static_cast<GLint>(location), static_cast<GLint>(slot));

	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textureObj);
}

void Texture::SaveImg()
{
	std::vector<unsigned char> outBuffer;
	outBuffer.resize(imageBPP * imageWidth * imageHeight);

	glBindTexture(target, textureObj);
	GLenum format = GL_RGB;

	if (imageBPP == 4)
		format = GL_RGBA;
	
	glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, outBuffer.data());

	stbi_flip_vertically_on_write(true);
	//stbi_write_jpg("test.jpg", imageWidth, imageHeight, imageBPP, outBuffer.data(), 100);
	stbi_write_png("test.png", imageWidth, imageHeight, imageBPP, outBuffer.data(), 0);
}

GLuint Texture::GetTextureObj()
{
	return textureObj;
}


unsigned char* Image::Load_Image(std::string path, int& w, int& h, bool isFlip)
{
	if (!color_datas.empty())
	{
		color_datas.clear();
	}
	stbi_set_flip_vertically_on_load(isFlip);

	unsigned char* data = stbi_load(path.c_str(), &image_w, &image_h, &image_channel, color_channel);
	if (data)
	{
		pixel_size = image_w * image_h * color_channel;

		for (size_t i = 0; i < pixel_size; i += 4)
		{
			color_data.r = data[i];
			color_data.g = data[i + 1];
			color_data.b = data[i + 2];
			color_data.a = data[i + 3];
			color_datas.push_back(color_data);
		}
		stbi_image_free(data);
		w = image_w;
		h = image_h;

		return &color_datas[0].r;
	}
	else
	{
		stbi_image_free(data);
		return nullptr;
	}
}