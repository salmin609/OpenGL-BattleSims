/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: class for bone data / weights for each vertex.
 *                each boneData hold maximum 16 ids of bone, weights
 *                to store big object.
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */

#pragma once
#include <iostream>
#include <glm/fwd.hpp>

#define MAX_NUM_BONES_PER_VERTEX 16
using uint = glm::uint;

struct VertexBoneData
{
    glm::uint BoneIDs[MAX_NUM_BONES_PER_VERTEX] = { 0 };
    float Weights[MAX_NUM_BONES_PER_VERTEX] = { 0.0f };
    int index = 0;

    VertexBoneData()
    {
    }

    bool SlotEmpty()
    {
        return index < MAX_NUM_BONES_PER_VERTEX;
    }

    void AddBoneData(uint BoneID, float Weight)
    {
        if (Weight == 0.f)
            return;

        for(int i = 0; i < index; ++i)
        {
            if (BoneIDs[i] == BoneID)
                return;
        }

        if(index == MAX_NUM_BONES_PER_VERTEX)
        {
            std::cout << "Tried to put bone : " << BoneID << std::endl;
            //std::cout << "Tried to put bone : " << BoneID << std::endl;
            return;
        }

        BoneIDs[index] = BoneID;
        Weights[index] = Weight;

        index++;
    }
};
