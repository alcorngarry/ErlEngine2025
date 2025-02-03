#ifndef RENDERER_H
#define RENDERER_H
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<stb_image/stb_image.h>
#include"Shader.h"
#include"game_objects/Model.h"
#include"game_objects/GameObject.h"
#include"game_objects/SkinnedGameObject.h"
#include"common/Camera.h"
#include"ui/UIManager.h"
#include"common/AssetManager.h"
#include"ui/UIElement.h"
#include"common/DebugMenu.h"

namespace Renderer {
	struct Erl3DRenderItem {
		Model* model;
		glm::mat4 transform;
	};

	void init_render(GLFWwindow* window);
	void render(Camera* camera);

	void add_sky_box(SkyBox* skybox);
	void add_render_object(GameObject* gameObject);
	void add_light_render_object(GameObject* gameObject);
	void add_skinned_render_object(SkinnedGameObject* skinnedGameObject);
	
	void create_menu(float deltaTime);
	void create_menu(float deltaTime, int roll);
	void deselect_index();
	int get_selected_index();
	void select_entity(float xpos, float ypos);
	//private
	void draw_aabb(const glm::vec3& minAABB,const glm::vec3& maxAABB);
	void draw_static(Shader* shader, Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation);
	void draw_skinned(Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, std::vector<glm::mat4>* transform);
};

#endif // !RENDERER_H