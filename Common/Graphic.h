/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for openGL calls.
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */


#pragma once
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/detail/type_vec.hpp>

class Texture;
class MaterialManager;
struct Frustum;
class BillboardManager;
class BillboardAnimatingDatas;
class FrameBuffer;
class BillBoardObject;
class Buffer;
class Floor;
class Shader;
struct aiScene;
class Camera;
class GLFWwindow;
class AnimationModel;
class Object;
class SkyBox;

enum class objKind
{
	SWAT_RifleAimIdle = 0,
	SWAT_RifleCrouch,
	SWAT_RifleIdle,
	SWAT_RifleWalk,
	SWAT_Run,
	SWAT_SteppingBackward,
	SWAT_Strafing,
	SWAT_TurnLeft45Degree,
	SWAT_CrawlBackward,
	SWAT_Death,
	SWAT_Reload,
	SWAT_Reload2,
	SWAT_Smash,

	AMY_Excited,
	AMY_Floating,
	AMY_HappyIdle,
	AMY_Jogging,
	AMY_Singing,
	AMY_StandingUp,
	AMY_Surprised,
	AMY_TalkingOnPhone,
	AMY_TwistDance,

	KNIGHT_Attack1,
	KNIGHT_Block,
	KNIGHT_BlockIdle,
	KNIGHT_CrouchBlockIdle,
	KNIGHT_Death,
	KNIGHT_Idle,
	KNIGHT_Idle2,
	KNIGHT_Impact,
	KNIGHT_PowerUp,
	KNIGHT_Run,
	KNIGHT_Slash,

	MICHELLE_BreakDance,
	MICHELLE_BreakDanceReady,
	MICHELLE_Dancing,
	MICHELLE_DancingTwerk,
	MICHELLE_Flair,
	MICHELLE_HipHopDance1,
	MICHELLE_HipHopDance2,
	MICHELLE_HipHopDance3,
	MICHELLE_HipHopDance4,
	MICHELLE_RumbaDance,
	MICHELLE_Twist,
	MICHELLE_WaveHipHop,

	ADAM_ComboPunch,
	ADAM_ElbowPunch,
	ADAM_Idle,
	ADAM_Idle2,
	ADAM_Jab,
	ADAM_MMAKick,
	ADAM_Victory,


	END,
};


class Graphic
{
public:


	Graphic(int w, int h);
	~Graphic();

	void PopulateObjsPos();
	void PopulateObjs(int num, int obj);
	void DeleteObjs(int num);
	void Draw();
	void ProcessInput();
	void PrintCameraStatement(Camera* cam_);
	void SetWindowWidthHeight(int w, int h);

	void ResetFrameBufferWidthHeight(int w, int h) const;
	void PopulateObjPaths();

	GLFWwindow* window;
	Camera* cam;

	int posOffset = 0;
	float deltaTime, lastFrame;
	bool camLock = true;


	std::vector<Object*> objs;
	std::vector<glm::vec3> objsPos;
	std::vector<BillBoardObject*> bos;
	BillboardManager* boManager;
	std::vector<std::string> objPaths;

	Floor* floor;
	glm::vec3 populateLastPosition = glm::vec3(0.f, 12.f, -20.f);
	int totalRenderingAmount = 0;

	Camera* currentCam;
	bool testOneTime = true;
	SkyBox* skybox;

	Frustum* frustum;


private:

	Shader* shader;
	Shader* billboardShader;
	Shader* floorShader;
	Shader* interpolationComputeShader;
	int windowWidth;
	int windowHeight;
};
