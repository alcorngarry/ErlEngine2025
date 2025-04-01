#ifndef CHAMBER_H
#define CHAMBER_H
#include"common/Map.h"
#include"game_objects/Player.h"
#include"commands/MoveCameraCommand.h"
#include"commands/FireCommand.h"

class Chamber : public Map {
	PlayerControls playerControls{ GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_SPACE, GLFW_KEY_LEFT, GLFW_KEY_RIGHT };

	public:
		std::vector<Player*> players;
		Chamber(std::string mapName);
		void set_controls(float deltaTime) override;
		void update(float deltaTime) override;
	protected:
		void load_skinned_objects() override;
		void load_camera(float windowWidth, float windowHeight) override;
		void draw(float deltaTime) override;
};
#endif // !CHAMBER_H
