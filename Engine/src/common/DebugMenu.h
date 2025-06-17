#ifndef DEBUG_MENU_H
#define DEBUG_MENU_H

#include<glm/gtc/type_ptr.hpp>
#include<imgui/imgui.h>
#include<imgui/imgui_impl_glfw.h>
#include<imgui/imgui_impl_opengl3.h>
#include"ImGuizmo.h"
#include<string>
#include<vector>
#include<common/Camera.h>
#include<renderer/Renderer.h>
#include<input/InputManager.h>
#include"commands/MoveCameraCommand.h"
#include"commands/SelectEntityCommand.h"
#include"commands/AddRemoveEntityCommand.h"
#include"game_objects/Player.h"

enum class MENU_TYPE {
	EDITOR,
	PLAYER,
	DISABLED
};

namespace DebugMenu
{
		void init(GLFWwindow* glfwWindow);
		void load_map(Map* map);
		void create_menu(float deltaTime);
		void show_scripts(GameObject* entity);
		void show_all_entities();
		void display_fps(float deltaTime);
		void draw_entity_hierarchy();
		void display_player_velocity();
		void draw_entity_properties(GameObject* entity);
		void draw_mouse_pos();
		void draw_camera_position();
		void shut_down();
		void set_controls();
		void clear_controls();
};
#endif // !DEBUG_MENU_H
