#include"Bone.h"

Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel) :
	m_Name(name),
	m_ID(ID),
	m_LocalTransform(1.0f)
{
	m_NumPositions = channel->mNumPositionKeys;

	for (int positionIndex = 0; positionIndex < m_NumPositions; ++positionIndex)
	{
		aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
		double timeStamp = channel->mPositionKeys[positionIndex].mTime;
		KeyPosition data;
		data.position = glm::vec3(aiPosition.x, aiPosition.y, aiPosition.z);
		data.timeStamp = (float)timeStamp;
		m_Positions.push_back(data);
	}

	m_NumRotations = channel->mNumRotationKeys;
	for (int rotationIndex = 0; rotationIndex < m_NumRotations; ++rotationIndex)
	{
		aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
		double timeStamp = channel->mRotationKeys[rotationIndex].mTime;
		KeyRotation data;
		data.orientation = glm::vec3(aiOrientation.x, aiOrientation.y, aiOrientation.z);
		data.timeStamp = timeStamp;
		m_Rotations.push_back(data);
	}

	m_NumScalings = channel->mNumScalingKeys;
	for (int keyIndex = 0; keyIndex < m_NumScalings; ++keyIndex)
	{
		aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
		double timeStamp = channel->mScalingKeys[keyIndex].mTime;
		KeyScale data;
		data.scale = glm::vec3(scale.x, scale.y, scale.z);
		data.timeStamp = timeStamp;
		m_Scales.push_back(data);
	}
}

void Bone::update(float animationTime)
{
	glm::mat4 translation = interpolation_position(animationTime);
	glm::mat4 rotation = interpolation_rotation(animationTime);
	glm::mat4 scale = interpolation_scaling(animationTime);
	m_LocalTransform = translation * rotation * scale;
}

glm::mat4 Bone::get_local_transform() const
{
	return m_LocalTransform;
}

std::string Bone::get_bone_name() const
{
	return m_Name;
}

int Bone::get_bone_id() const
{
	return m_ID;
}

int Bone::get_position_index(float animationTime) const
{
	for (int index = 0; index < m_NumPositions - 1; ++index)
	{
		if (animationTime < m_Positions[index + 1].timeStamp)
			return index;
	}
	assert(0);
}

int Bone::get_rotation_index(float animationTime) const
{
	for (int index = 0; index < m_NumRotations - 1; ++index)
	{
		if (animationTime < m_Rotations[index + 1].timeStamp)
			return index;
	}
	assert(0);
}

int Bone::get_scale_index(float animationTime) const
{
	for (int index = 0; index < m_NumScalings - 1; ++index)
	{
		if (animationTime < m_Scales[index + 1].timeStamp)
			return index;
	}
	assert(0);
}

//private
float Bone::get_scale_factor(float lastTimeStamp, float nextTimeStamp, float animationTime) const
{
	float scaleFactor = 0.0f;
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	scaleFactor = midWayLength / framesDiff;
	return scaleFactor;
}

glm::mat4 Bone::interpolation_position(float animationTime)
{
	if (1 == m_NumPositions)
		return glm::translate(glm::mat4(1.0f), m_Positions[0].position);

	int p0Index = get_position_index(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = get_scale_factor(m_Positions[p0Index].timeStamp,
		m_Positions[p1Index].timeStamp, animationTime);
	glm::vec3 finalPosition = glm::mix(m_Positions[p0Index].position, m_Positions[p1Index].position
		, scaleFactor);
	return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::interpolation_rotation(float animationTime)
{
	if (1 == m_NumRotations)
	{
		auto rotation = glm::normalize(m_Rotations[0].orientation);
		return glm::toMat4(rotation);
	}

	int p0Index = get_rotation_index(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = get_scale_factor(m_Rotations[p0Index].timeStamp,
		m_Rotations[p1Index].timeStamp, animationTime);
	glm::quat finalRotation = glm::slerp(m_Rotations[p0Index].orientation, m_Rotations[p1Index].orientation
		, scaleFactor);
	finalRotation = glm::normalize(finalRotation);
	return glm::toMat4(finalRotation);
}

glm::mat4 Bone::interpolation_scaling(float animationTime)
{
	if (1 == m_NumScalings)
		return glm::scale(glm::mat4(1.0f), m_Scales[0].scale);

	int p0Index = get_scale_index(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = get_scale_factor(m_Scales[p0Index].timeStamp,
		m_Scales[p1Index].timeStamp, animationTime);
	glm::vec3 finalScale = glm::mix(m_Scales[p0Index].scale, m_Scales[p1Index].scale
		, scaleFactor);
	return glm::scale(glm::mat4(1.0f), finalScale);
}