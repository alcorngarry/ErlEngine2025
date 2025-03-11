#include "BoardSpace.h"

BoardSpace::BoardSpace(uint16_t id, Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation) : GameObject(id, model, pos, size, rotation) {}

BoardSpace::BoardSpace(GameObject* gameObject) : GameObject(gameObject->id, gameObject->GameModel, gameObject->Position, gameObject->Size, gameObject->Rotation){}