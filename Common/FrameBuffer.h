/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright � 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once

class Texture;

class FrameBuffer
{
public:
	FrameBuffer(int width_, int height_);
	~FrameBuffer();
	void Bind();
	void UnBind();
	void SavePNG();
	void ReadValue();
	Texture* texture;

private:
	unsigned frameBuffer, rbo;
	int width, height;
};