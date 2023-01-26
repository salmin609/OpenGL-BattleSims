#pragma once

class HerdManager;
class BufferManager;
class Shader;


//need to change to derived class
class BillboardMovingCS
{
public:
	BillboardMovingCS(Shader* boMovingShader_, HerdManager* herdManager_);
	~BillboardMovingCS();
	void SetUniforms();
	void SetBuffers();
	void Move(float dt);
private:
	Shader* boMovingShader;
	BufferManager* csBuffers;
	HerdManager* herdManager;
	int* reached;
};
