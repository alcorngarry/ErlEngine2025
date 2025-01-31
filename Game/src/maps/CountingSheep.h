#ifndef COUNTING_SHEEP_H
#define COUNTING_SHEEP_H
#include"common/Map.h"
#include"game_objects/Player.h"
#include"commands/AddSubtractCountCommand.h"

class CountingSheep : public Map {
	PlayerControls playerControls{ GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_SPACE, GLFW_KEY_LEFT, GLFW_KEY_RIGHT };
	public:
		CountingSheep(std::string mapName);
		void set_controls(float deltaTime) override;
		void update(float deltaTime) override;
		void draw(float deltaTime) override;
	protected:
		void load_skinned_objects() override;
	private:
		std::vector<GameObject*> sheep;
		int currentSheep = 0;
		int numSheep = 0;
		float startTime = 0.0f;
};
#endif // !COUNTING_SHEEP_H

