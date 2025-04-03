#include "BoardSpace.h"
#include "game_objects/Player.h"

BoardSpace::BoardSpace(uint16_t id, Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation) : GameObject(id, model, pos, size, rotation, true) {}

BoardSpace::BoardSpace(GameObject* gameObject) : GameObject(gameObject->id, gameObject->GameModel, gameObject->Position, gameObject->Size, gameObject->Rotation, gameObject->isRendered){}

void BoardSpace::process_space(Player* player)
{
	switch (spaceType)
	{
	case 0:
		//loadState = CHANGE_MAP;
		break;
	case 1:
		player->add_groats(3);
		std::cout << "added 3 groats to player: " << player->id << ", total groats = " << player->groats << std::endl;
		break;
	case 2:
		player->add_groats(6);
		std::cout << "added 6 groats to player: " << player->id << ", total groats = " << player->groats << std::endl;
		break;
	case 3:
		player->remove_groats(3);
		std::cout << "removed 3 groats to player: " << player->id << ", total groats = " << player->groats << std::endl;
		break;
	default:
		std::cout << "error processing space" << std::endl;
	}
}