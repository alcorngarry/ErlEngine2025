#ifndef BOARD_SPACE_H
#define BOARD_SPACE_H
#include"game_objects/GameObject.h"

class BoardSpace : public GameObject {
	public:
		BoardSpace(uint16_t id, Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation);
		BoardSpace(GameObject* gameObject);
		//virtual void process_space() = 0;

		std::vector<BoardSpace*> nextSpace;
		std::vector<uint8_t> nextSpaceIds;
		uint8_t spaceType = 0;
};

#endif // !BOARD_SPACE_H