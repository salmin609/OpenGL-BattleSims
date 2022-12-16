/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for populate SSBOs of animating object.
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */


#include "BoneStorageManager.h"


BoneStorageManager::BoneStorageManager(const std::vector<VertexBoneData>& boneInfos)
{
	const size_t size = boneInfos.size();
	int start = 0;
	int end = 0;

	for(size_t i = 0 ; i < size; ++i)
	{
		const VertexBoneData info = boneInfos[i];

		for(int j = 0; j < MAX_NUM_BONES_PER_VERTEX; ++j)
		{
			float weight = info.Weights[j];
			int id = static_cast<int>(info.BoneIDs[j]);

			bones.push_back(id);
			weights.push_back(weight);
			end++;
			
		}
		indexStarts.push_back(start);
		indexEnds.push_back(end);
		start = end;
	}

	


}

BoneStorageManager::~BoneStorageManager()
{

}
