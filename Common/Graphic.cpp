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

#include "BillBoardObject.h"
#include "BillboardAnimatingDatas.h"
#include "BillboardManager.h"
#include "FrustumCulling.h"
#include "Skybox.h"
#include "Floor.hpp"
#include "Line.h"
#include "MultipleAnimationObject.h"
#include "ModelKinds.hpp"
#include "assimp/anim.h"

Graphic::Graphic(int w, int h) : windowWidth(w), windowHeight(h), deltaTime(0.f), lastFrame(0.f)
{
	shader = new Shader("../Shaders/vert.glsl", "../Shaders/frag.glsl");
	floorShader = new Shader("../Shaders/floorVertex.glsl", "../Shaders/floorFragment.glsl");
	billboardShader = new Shader("../Shaders/billboardVert.glsl", "../Shaders/billboardFrag.glsl", 
		"../Shaders/billboardGeometry.glsl");
	interpolationComputeShader = new Shader("../Shaders/computeShader.glsl");
	lineShader = new Shader("../Shaders/lineVert.glsl", "../Shaders/lineFrag.glsl");

	floor = new Floor(floorShader);
	floor->pos = glm::vec3(660.f, 0.f, 670.f);
	floor->scale = glm::vec3(20000.f, 1.f, 20000.f);
	floor->rot = glm::vec3(0.f, 1.f, 0.f);

	objPaths = ObjPaths();

	boManager = new BillboardManager(shader, interpolationComputeShader, windowWidth, windowHeight, objPaths);
	skybox = new SkyBox();
	PopulateObjsPos();

	cam = new Camera(glm::vec3(-47.5701f, 56.8972f, -76.2187f), 
		glm::vec3(0.f, 1.f, 0.f),
		50.8f, -14.0999f);

	floorLine = new Line(lineShader);



	/*BillboardAnimatingDatas* data = boManager->boDatas[0];
	currentCam = boManager->GetBoObjCamera(static_cast<int>(CamVectorOrder::LeftBack));
	objs.push_back(data->obj);*/

	currentCam = cam;

	frustum = new Frustum();
	frustum->ResetFrustumPlans(*currentCam, static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
		glm::radians(currentCam->Zoom), 0.1f, 1000.f);

	PopulateObjs(3000, 0);
	//PopulateObjs(3000, 1);
	//PopulateObjs(3000, 2);
	//PopulateObjs(3000, 3);
	//PopulateObjs(3000, 4);

	
}

Graphic::~Graphic()
{
	for (const auto bo : bos)
		delete bo;

	delete boManager;
	delete skybox;
	delete frustum;
	delete cam;
	//delete currentCam;
	delete shader;
	delete interpolationComputeShader;
	delete lineShader;
	delete billboardShader;
	delete floorLine;
}

void Graphic::PopulateObjsPos()
{
	glm::vec3 pos = populateLastPosition;

	for (int i = 0; i < 200000; ++i)
	{
		if (i % 200 == 0)
		{
			pos.x = 0.f;
			pos.z += 20.f;
		}

		pos.x += 20.f;

		objsPos.push_back(pos);
	}
	//PopulateObjs(200, int(objKind::SWAT_RifleAimIdle));
}

void Graphic::PopulateObjs(int num, int obj)
{
	BillboardAnimatingDatas* data = boManager->boDatas[obj];

	const int animationCount = static_cast<int>(data->obj->animationModels.size());
	data->inUse = true;
	
	for (int i = 0; i < num; ++i)
	{
		const int animationIndex = rand() % animationCount;
		const int timeDiffSlot = rand() % data->diffTimeAnimCount;

		const glm::vec3& pos = objsPos[posOffset + i];

		bos.push_back(new BillBoardObject(billboardShader,
			pos, data->frameBuffers[timeDiffSlot][animationIndex], currentCam));
	}

	posOffset += num;

	totalRenderingAmount += num;
}

void Graphic::DeleteObjs(int num)
{
	for (int i = posOffset - 1; i >= posOffset - num; --i)
	{
		delete bos[i];
	}

	posOffset -= num;
	if (posOffset < 0)
		posOffset = 0;

	bos.resize(posOffset);

	totalRenderingAmount -= num;
}


void Graphic::Draw()
{
	glClearColor(0.29f, 0.31f, 0.34f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, windowWidth, windowHeight);

	const glm::mat4 projMat = glm::perspective(glm::radians(currentCam->Zoom), (float)windowWidth / (float)windowHeight,
		0.1f, 10000.f);

	const glm::mat4 viewMat = currentCam->GetViewMatrix();
	const glm::mat4 projViewMat = projMat * viewMat;

	boManager->GenBillboard(projMat);

	frustum->ResetFrustumPlans(*currentCam, static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
		glm::radians(currentCam->Zoom), 0.1f, 2000.f);

	for (const auto& bo : bos)
	{
		bo->Render(projMat, viewMat, frustum);
	}

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
			//AnimationModel* animationModel = obj->animationModel;
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
	//currentCam->Yaw = 0.f;
	//currentCam->Pitch = 0.f;

	*cam = Camera(currentCam->Position,
		currentCam->Up,
		0.f, 0.f);
}
