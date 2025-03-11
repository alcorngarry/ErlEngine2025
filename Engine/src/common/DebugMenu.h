#ifndef DEBUG_MENU_H
#define DEBUG_MENU_H

#include<glm/gtc/type_ptr.hpp>
#include<imgui/imgui.h>
#include<imgui/imgui_impl_glfw.h>
#include<imgui/imgui_impl_opengl3.h>
#include"ImGuizmo.h"
#include<string>
#include<vector>
#include"renderer/Renderer.h"
#include "../../../Game/src/game_objects/board_space/BoardSpace.h"


class DebugMenu
{
	public:
		DebugMenu(GLFWwindow* glfwWindow);
		DebugMenu();

		~DebugMenu();
		void create_menu(std::vector<GameObject*>& entities, Camera* camera, float deltaTime);
		void display_fps(float deltaTime);
		void draw_entity_hierarchy(std::vector<GameObject*>& entities);
		void draw_entity_properties(GameObject* entity, Camera* camera);
		void draw_mouse_pos();
		void draw_camera_position(Camera* camera);
		void create_new_map();
		void load_board_space_data(std::vector<BoardSpace*> boardSpaces);
		void shut_down();
};


#endif // !DEBUG_MENU_H
