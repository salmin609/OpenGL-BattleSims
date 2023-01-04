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

class Line;
class Timer;
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
struct GLFWwindow;
class AnimationModel;
class Object;
class SkyBox;

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

	void ResetCamAngle();

	GLFWwindow* window;
	Camera* cam;
	Timer* timer;
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
	Shader* lineShader;
	int windowWidth;
	int windowHeight;
	Line* floorLine;
};

