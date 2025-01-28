#ifndef PONG_MAP_H
#define PONG_MAP_H
#include"common/Map.h"
#include"game_objects/Player.h"

class PongMap : public Map {
	PlayerControls playerControls{ GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_SPACE, GLFW_KEY_LEFT, GLFW_KEY_RIGHT };

	public:
		std::vector<Player*> players;
		PongMap(std::string mapName);
		void set_controls(float deltaTime) override;
		void update(float deltaTime) override;
	protected:
		void load_skinned_objects() override;
	private:
		void check_ball_collision(GameObject* entity);

		int team_1_points = 0;
		int team_2_points = 0;
};
#endif // !PONG_MAP_H
