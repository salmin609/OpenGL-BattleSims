/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for openGL calls.
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */


#include "Graphic.h"
#include <chrono>
#include "Shader.h"
#include "Camera.hpp"
#include <GLFW/glfw3.h>
#include "AnimationModel.h"
#include "Object.h"
#include <fstream>

#include "BillboardAnimatingDatas.h"
#include "BillboardManager.h"
#include "BillboardObjectManager.h"
#include "Skybox.h"
#include "Line.h"
#include "ModelKinds.hpp"
#include "assimp/anim.h"

Graphic::Graphic(int w, int h) : deltaTime(0.f), lastFrame(0.f), windowWidth(w), windowHeight(h)
{
	animShader = new Shader("../Shaders/AnimationVertex.glsl", "../Shaders/AnimationFragment.glsl");
	floorShader = new Shader("../Shaders/floorVertex.glsl", "../Shaders/floorFragment.glsl");
	billboardShader = new Shader("../Shaders/billboardVert.glsl", "../Shaders/billboardFrag.glsl", 
		"../Shaders/billboardGeometry.glsl");
	interpolationComputeShader = new Shader("../Shaders/SkinningComputeShader.glsl");
	lineShader = new Shader("../Shaders/lineVert.glsl", "../Shaders/lineFrag.glsl");
	bbCheckFrameBufferUsage = new Shader("../Shaders/BillboardObjectAngleCompute.glsl");
	bbMoving = new Shader("../Shaders/BillboardObjectMovingCompute.glsl");
	bbAttack = new Shader("../Shaders/BillboardAttackCompute.glsl");
	bbChangeAnimation = new Shader("../Shaders/BillboardObjectAnimationStateCompute.glsl");


	cam = new Camera(glm::vec3(-47.5701f, 56.8972f, -76.2187f),
		glm::vec3(0.f, 1.f, 0.f),
		50.8f, -14.0999f);
	currentCam = cam;

	objPaths = ObjPaths();
	boManager = new BillboardManager(animShader, interpolationComputeShader, windowWidth, windowHeight, objPaths);
	boObjsManager = new BillboardObjectManager(billboardShader, boManager, 
		bbCheckFrameBufferUsage, currentCam, bbMoving, bbAttack,
		bbChangeAnimation);
	skybox = new SkyBox();

	floorLine = new Line(lineShader);
	cam->fov = (float)windowWidth / (float)windowHeight;
	cam->fovY = glm::radians(cam->Zoom);

	//BillboardAnimatingDatas* data = boManager->boDatas[0];

	//objs.push_back(reinterpret_cast<Object*>(data->obj));
	//currentCam = boManager->GetBoObjCamera(0);


	//frustum = new Frustum();
	//frustum->ResetFrustumPlans(*currentCam, fov,
	//	glm::radians(currentCam->Zoom), boObjsManager->zNear, 
	//	boObjsManager->zFar);
}

Graphic::~Graphic()
{
	delete boObjsManager;
	delete boManager;
	delete skybox;
	//delete frustum;
	delete cam;
	delete animShader;
	delete interpolationComputeShader;
	delete lineShader;
	delete billboardShader;
	delete floorLine;
	delete floorShader;
	delete bbCheckFrameBufferUsage;
	delete bbMoving;
	delete bbChangeAnimation;
}

void Graphic::Draw()
{
	glClearColor(0.29f, 0.31f, 0.34f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, windowWidth, windowHeight);

	const glm::mat4 projMat = glm::perspective(glm::radians(currentCam->Zoom), 
		static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
		currentCam->zNear, currentCam->zFar);

	const glm::mat4 viewMat = currentCam->GetViewMatrix();
	const glm::mat4 projViewMat = projMat * viewMat;

	//Decide whether billboard object is in frustum or not.
	//If it, get angle index from that
	boObjsManager->SetFrameBufferIndexFromAngle();

	//Move those billboard objects to desired direction. which is to nearest enemy.
	boObjsManager->Move(deltaTime);

	//Check if object is attacking animation, if it, += dt to timer buffer.
	//if timer over some certain number, change to death state.
	boObjsManager->Attack(deltaTime);

	//Update animation's status whether Ready or Playing
	//boManager->CheckAnimationPlayingStatus();

	//Set angle index from framebuffers[]. if index >= 0, which means
	//billboard object is in frustum, near of camera position.

	//Request to change animation state
	boObjsManager->ChangeAnimationOfHerds();

	boManager->GenBillboard(projMat);
	boObjsManager->Render(projMat, viewMat);

	skybox->Draw(projMat, viewMat);
	floorLine->Draw(projViewMat);
	/*
	 * Traverse all objects and calculate time ticks and pass to Draw()
	 */
	if(!objs.empty())
	{
		if (currentCam != cam)
			PrintCameraStatement(currentCam);

		const std::chrono::system_clock::time_point current = std::chrono::system_clock::now();

		for (const auto& obj : objs)
		{
			const aiAnimation* animation = obj->GetAiAnimation();
			const long long diff = std::chrono::duration_cast<std::chrono::milliseconds>(current - obj->GetAnimationStartTime()).count();
			const float animationT = static_cast<float>(diff) / 1000.f;

			const float timeInTicks = animationT * static_cast<float>(animation->mTicksPerSecond);
			const float animationTimeTicks = fmod(timeInTicks, static_cast<float>(animation->mDuration));
			obj->Draw(projViewMat, obj->Interpolate(animationTimeTicks));
		}
	}
}

void Graphic::ProcessInput()
{
	if (glfwGetKey(window, GLFW_KEY_W))
		currentCam->ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A))
		currentCam->ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D))
		currentCam->ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S))
		currentCam->ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camLock = !camLock;
}

void Graphic::PrintCameraStatement(Camera* cam_)
{
	const glm::vec3 camPos = cam_->Position;
	const glm::vec3 camUp = cam_->Up;

	std::cout << "glm::vec3(" << camPos.x << "f," << camPos.y << "f," << camPos.z << "f)," << std::endl;
	std::cout << "glm::vec3(" << camUp.x << "f," << camUp.y << "f," << camUp.z << "f)" << "," << std::endl <<
		cam_->Yaw << "f," << cam_->Pitch << "f" << std::endl;

	std::cout << std::endl;
}

void Graphic::SetWindowWidthHeight(int w, int h)
{
	windowWidth = w;
	windowHeight = h;
}

void Graphic::ResetCamAngle()
{
	*cam = Camera(currentCam->Position,
		currentCam->Up,
		0.f, 0.f);
}
