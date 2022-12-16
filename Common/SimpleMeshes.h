/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *	Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once

namespace SimpleMesh
{
    
	
	inline static float* SimpleCube(int& size)
	{
		size = 108;
		float* result = new float[108]
		{
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};
		return result;
	};

	inline static float* SimpleBigCube(int& size)
	{
		size = 108;
		float* result = new float[108]
		{
			// positions          
			-30.0f,  30.0f, -30.0f,
			-30.0f, -30.0f, -30.0f,
			 30.0f, -30.0f, -30.0f,
			 30.0f, -30.0f, -30.0f,
			 30.0f,  30.0f, -30.0f,
			-30.0f,  30.0f, -30.0f,
			-30.0f, -30.0f,  30.0f,
			-30.0f, -30.0f, -30.0f,
			-30.0f,  30.0f, -30.0f,
			-30.0f,  30.0f, -30.0f,
			-30.0f,  30.0f,  30.0f,
			-30.0f, -30.0f,  30.0f,
			 30.0f, -30.0f, -30.0f,
			 30.0f, -30.0f,  30.0f,
			 30.0f,  30.0f,  30.0f,
			 30.0f,  30.0f,  30.0f,
			 30.0f,  30.0f, -30.0f,
			 30.0f, -30.0f, -30.0f,
			-30.0f, -30.0f,  30.0f,
			-30.0f,  30.0f,  30.0f,
			 30.0f,  30.0f,  30.0f,
			 30.0f,  30.0f,  30.0f,
			 30.0f, -30.0f,  30.0f,
			-30.0f, -30.0f,  30.0f,
			-30.0f,  30.0f, -30.0f,
			 30.0f,  30.0f, -30.0f,
			 30.0f,  30.0f,  30.0f,
			 30.0f,  30.0f,  30.0f,
			-30.0f,  30.0f,  30.0f,
			-30.0f,  30.0f, -30.0f,
			-30.0f, -30.0f, -30.0f,
			-30.0f, -30.0f,  30.0f,
			 30.0f, -30.0f, -30.0f,
			 30.0f, -30.0f, -30.0f,
			-30.0f, -30.0f,  30.0f,
			 30.0f, -30.0f,  30.0f
		};
		return result;
	};

	inline static float* SimpleRectangleWithTextureCoord(int& count)
	{
		count = 24;
		float* quadVertices = new float[24]
		{
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		return quadVertices;
	}

	
}
