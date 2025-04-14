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

class DebugMenu
{
	public:
		DebugMenu(GLFWwindow* glfwWindow);
		DebugMenu();

		~DebugMenu();
		void create_menu(std::map<uint16_t, GameObject*>& entities, Camera* camera, float deltaTime);
		void display_board_tiles(std::map<uint16_t, GameObject*> entities);
		void display_fps(float deltaTime);
		void draw_entity_hierarchy(std::map<uint16_t, GameObject*>& entities);
		void draw_entity_properties(GameObject* entity, Camera* camera);
		void draw_mouse_pos();
		void draw_ray_cast();
		void draw_camera_position(Camera* camera);
		void create_new_map();
		void shut_down();
		void set_controls();
		void clear_controls();
	private:
		Camera* m_camera;
		int windowWidth = 0;
		int windowHeight = 0;
};
#endif // !DEBUG_MENU_H
