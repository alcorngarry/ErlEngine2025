#ifndef BOARD_SPACE_H
#define BOARD_SPACE_H
#include"game_objects/GameObject.h"
class Player;

class BoardSpace : public GameObject {
	public:
		BoardSpace(uint16_t id, Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation);
		BoardSpace(GameObject* gameObject);
		void process_space(Player* player);
		
		std::vector<BoardSpace*> nextSpace;
		std::vector<int> nextSpaceIds;
		uint8_t spaceType = 2;
};

#endif // !BOARD_SPACE_H