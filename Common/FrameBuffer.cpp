/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *  Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "FrameBuffer.h"

#include "Texture.h"
#include "GL/glew.h"

FrameBuffer::FrameBuffer(int width_, int height_)
{
    width = width_;
    height = height_;

    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    texture = new Texture(GL_TEXTURE_2D, "");
    texture->LoadEmptyTexture(width, height);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetTextureObj(), 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)

	glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &frameBuffer);
    glDeleteRenderbuffers(1, &rbo);

    delete texture;
}

void FrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

void FrameBuffer::UnBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::SavePNG()
{
    texture->SaveImg();
}

void FrameBuffer::ReadValue()
{
    unsigned char* pBuffer = new unsigned char[width * height * 4];
    glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pBuffer);
    
    delete[] pBuffer;
}

unsigned FrameBuffer::GetFrameBufferId()
{
    return frameBuffer;
}

