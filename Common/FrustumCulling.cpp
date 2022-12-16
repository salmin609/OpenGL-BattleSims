/*
 *	File for GAM400 - OpenGL MAs project.
 *	Term			- 2022F
 *	Author			- Ryan Kim
 *  Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#include "FrustumCulling.h"

#include "Camera.hpp"

void Frustum::ResetFrustumPlans(const Camera& cam, float aspect, float fovY, float zNear, float zFar)
{
    const float halfVSide = zFar * tanf(fovY * .5f);
    const float halfHSide = halfVSide * aspect;
    const glm::vec3 frontMultFar = zFar * cam.Front;

    nearFace = { cam.Position + zNear * cam.Front, cam.Front };
    farFace = { cam.Position + frontMultFar, -cam.Front };
    rightFace = { cam.Position,
                    glm::cross(cam.Up,frontMultFar + cam.Right * halfHSide) };
    leftFace = { cam.Position,
                    glm::cross(frontMultFar - cam.Right * halfHSide, cam.Up) };
    topFace = { cam.Position,
                    glm::cross(cam.Right, frontMultFar - cam.Up * halfVSide) };
    bottomFace = { cam.Position,
                            glm::cross(frontMultFar + cam.Up * halfVSide, cam.Right) };

}
