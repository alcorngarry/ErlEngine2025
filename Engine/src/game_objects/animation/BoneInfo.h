#ifndef BONE_INFO_H
#define BONE_INFO_H
#include<glm/glm.hpp>

struct BoneInfo
{
	int id;
	glm::mat4 offset;
};
#endif // !BONE_INFO_H
