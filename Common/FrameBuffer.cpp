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
    // create a color attachment texture
    //glGenTextures(1, &textureColorBuffer);
    //glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    texture = new Texture(GL_TEXTURE_2D, "");
    texture->LoadEmptyTexture(width, height);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetTextureObj(), 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

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

    //Place here


    
    delete[] pBuffer;
}

unsigned FrameBuffer::GetFrameBufferId()
{
    return frameBuffer;
}

