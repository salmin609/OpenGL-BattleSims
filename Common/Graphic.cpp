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
#include "MultipleAnimationObject.h"
#include "Timer.hpp"

Graphic::Graphic(int w, int h) : windowWidth(w), windowHeight(h), deltaTime(0.f), lastFrame(0.f)
{
	shader = new Shader("../Shaders/vert.glsl", "../Shaders/frag.glsl");
	floorShader = new Shader("../Shaders/floorVertex.glsl", "../Shaders/floorFragment.glsl");
	billboardShader = new Shader("../Shaders/billboardVert.glsl", "../Shaders/billboardFrag.glsl", 
		"../Shaders/billboardGeometry.glsl");
	interpolationComputeShader = new Shader("../Shaders/computeShader.glsl");

	floor = new Floor(floorShader);
	floor->pos = glm::vec3(660.f, 0.f, 670.f);
	floor->scale = glm::vec3(20000.f, 1.f, 20000.f);
	floor->rot = glm::vec3(0.f, 1.f, 0.f);

	PopulateObjPaths();
	boManager = new BillboardManager(shader, interpolationComputeShader, windowWidth, windowHeight, objPaths);
	skybox = new SkyBox();
	PopulateObjsPos();

	cam = new Camera(glm::vec3(-47.5701f, 56.8972f, -76.2187f), 
		glm::vec3(0.26682f, 1.f, 0.0932965f),
		50.8f, -14.0999f);

	currentCam = cam;

	frustum = new Frustum();
	frustum->ResetFrustumPlans(*currentCam, static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
		glm::radians(currentCam->Zoom), 0.1f, 1000.f);


}

Graphic::~Graphic()
{
	for (const auto bo : bos)
		delete bo;

	delete boManager;
	delete skybox;
	delete frustum;
	delete currentCam;
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
	PopulateObjs(200, int(objKind::SWAT_RifleAimIdle));
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
			pos, data->frameBuffers[timeDiffSlot][animationIndex]));
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
	if(currentCam != cam)
		PrintCameraStatement(currentCam);

	glClearColor(0.29f, 0.31f, 0.34f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, windowWidth, windowHeight);

	const glm::mat4 projMat = glm::perspective(glm::radians(currentCam->Zoom), (float)windowWidth / (float)windowHeight,
		0.1f, 10000.f);

	const glm::mat4 viewMat = currentCam->GetViewMatrix();
	const glm::mat4 projViewMat = projMat * viewMat;

	boManager->GenBillboard(projMat);

	floor->Draw(projViewMat);

	frustum->ResetFrustumPlans(*currentCam, static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
		glm::radians(currentCam->Zoom), 0.1f, 2000.f);

	for (const auto& bo : bos)
	{
		bo->Render(projMat, viewMat, frustum);
	}

	skybox->Draw(projMat, viewMat);
	/*
	 * Traverse all objects and calculate time ticks and pass to Draw()
	 */
#if 0
	for (const auto& obj : objs)
	{
		//AnimationModel* animationModel = obj->animationModel;
		const aiAnimation* animation = obj->GetAiAnimation();
		const long long diff = std::chrono::duration_cast<std::chrono::milliseconds>(current - obj->GetAnimationStartTime()).count();
		const float animationT = static_cast<float>(diff) / 1000.f;

		const float timeInTicks = animationT * static_cast<float>(animation->mTicksPerSecond);
		const float animationTimeTicks = fmod(timeInTicks, static_cast<float>(animation->mDuration));
		obj->Draw(projViewMat, animationT, 0, obj->Interpolate(animationTimeTicks));
	}
#endif
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

void Graphic::ResetFrameBufferWidthHeight(int w, int h) const
{
	//boManager->ResetFrameBufferWidthHeight(w, h);
}

void Graphic::PopulateObjPaths()
{
	for(int i = 0; i < static_cast<int>(objKind::END); ++i)
	{
		std::string path = "../Models/";
		switch(static_cast<objKind>(i))
		{
		case objKind::SWAT_RifleAimIdle:
			path += "Swat_RifleAimingIdle";
			break;
		case objKind::SWAT_RifleCrouch:
			path += "Swat_RifleCrouchWalk";
			break;
		case objKind::SWAT_RifleIdle:
			path += "Swat_RifleIdle";
			break;
		case objKind::SWAT_RifleWalk:
			path += "Swat_RifleWalk";
			break;
		case objKind::SWAT_Run:
			path += "Swat_Run";
			break;
		case objKind::SWAT_SteppingBackward:
			path += "Swat_SteppingBackward";
			break;
		case objKind::SWAT_Strafing:
			path += "Swat_Strafing";
			break;
		case objKind::SWAT_TurnLeft45Degree:
			path += "Swat_TurnLeft45Degree";
			break;
		case objKind::SWAT_CrawlBackward:
			path += "Swat_CrawlBackward";
			break;
		case objKind::SWAT_Death:
			path += "Swat_Death";
			break;
		case objKind::SWAT_Reload:
			path += "Swat_Reload";
			break;
		case objKind::SWAT_Reload2:
			path += "Swat_Reload2";
			break;
		case objKind::SWAT_Smash:
			path += "Swat_Smash";
			break;

		case objKind::AMY_Excited:
			path += "Amy_Excited";
			break;
		case objKind::AMY_Floating:
			path += "Amy_Floating";
			break;
		case objKind::AMY_HappyIdle:
			path += "Amy_HappyIdle";
			break;
		case objKind::AMY_Jogging:
			path += "Amy_Jogging";
			break;
		case objKind::AMY_Singing: 
			path += "Amy_Singing";
			break;
		case objKind::AMY_StandingUp: 
			path += "Amy_StandingUp";
			break;
		case objKind::AMY_Surprised: 
			path += "Amy_Surprised";
			break;
		case objKind::AMY_TalkingOnPhone: 
			path += "Amy_TalkingOnPhone";
			break;
		case objKind::AMY_TwistDance: 
			path += "Amy_TwistDance";
			break;
		case objKind::KNIGHT_Attack1: 
			path += "Knight_Attack1";
			break;
		case objKind::KNIGHT_Block: 
			path += "Knight_Block";
			break;
		case objKind::KNIGHT_BlockIdle: 
			path += "Knight_BlockIdle";
			break;
		case objKind::KNIGHT_CrouchBlockIdle: 
			path += "Knight_CrouchBlockIdle";
			break;
		case objKind::KNIGHT_Death: 
			path += "Knight_Death";
			break;
		case objKind::KNIGHT_Idle: 
			path += "Knight_Idle";
			break;
		case objKind::KNIGHT_Idle2: 
			path += "Knight_Idle2";
			break;
		case objKind::KNIGHT_Impact: 
			path += "Knight_Impact";
			break;
		case objKind::KNIGHT_PowerUp:
			path += "Knight_PowerUp";
			break;
		case objKind::KNIGHT_Run: 
			path += "Knight_Run";
			break;
		case objKind::KNIGHT_Slash: 
			path += "Knight_Slash";
			break;

		case objKind::MICHELLE_BreakDance: 
			path += "Michelle_BreakDance";
			break;
		case objKind::MICHELLE_BreakDanceReady: 
			path += "Michelle_BreakDanceReady";
			break;
		case objKind::MICHELLE_Dancing: 
			path += "Michelle_Dancing";
			break;
		case objKind::MICHELLE_DancingTwerk: 
			path += "Michelle_DancingTwerk";
			break;
		case objKind::MICHELLE_Flair: 
			path += "Michelle_Flair";
			break;
		case objKind::MICHELLE_HipHopDance1: 
			path += "Michelle_HipHopDance1";
			break;
		case objKind::MICHELLE_HipHopDance2: 
			path += "Michelle_HipHopDance2";
			break;
		case objKind::MICHELLE_HipHopDance3: 
			path += "Michelle_HipHopDance3";
			break;
		case objKind::MICHELLE_HipHopDance4: 
			path += "Michelle_HipHopDance4";
			break;
		case objKind::MICHELLE_RumbaDance: 
			path += "Michelle_RumbaDance";
			break;
		case objKind::MICHELLE_Twist: 
			path += "Michelle_Twist";
			break;
		case objKind::MICHELLE_WaveHipHop: 
			path += "Michelle_WaveHipHop";
			break;

		case objKind::ADAM_ComboPunch: 
			path += "Adam_ComboPunch";
			break;
		case objKind::ADAM_ElbowPunch: 
			path += "Adam_ElbowPunch";
			break;
		case objKind::ADAM_Idle: 
			path += "Adam_Idle1";
			break;
		case objKind::ADAM_Idle2: 
			path += "Adam_Idle2";
			break;
		case objKind::ADAM_Jab: 
			path += "Adam_Jab";
			break;
		case objKind::ADAM_MMAKick: 
			path += "Adam_MMAKick";
			break;
		case objKind::ADAM_Victory: 
			path += "Adam_Victory";
			break;

		case objKind::END:
			break;
		default: ;
		}
		path += ".dae";
		objPaths.push_back(path);
	}
}
