#ifndef RENDERER_H
#define RENDERER_H
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<stb_image/stb_image.h>
#include<math/ErlMath.h>
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
	//not implemented check if wanted
	struct Erl3DRenderItem {
		Model* model;
		glm::mat4 transform;
	};

	struct Ray {
		glm::vec3 origin;
		glm::vec3 direction;
		float length;
	};

	void init_render(GLFWwindow* window);
	void render(Camera* camera);

	void add_sky_box(SkyBox* skybox);
	void add_render_object(GameObject* gameObject);
	void add_ray(glm::vec3 origin, glm::vec3 direction, float length);
	std::vector<Renderer::Ray*> get_rays();
	void remove_render_object(int index);
	std::vector<GameObject*> get_rendered_entities();
	void add_light_render_object(GameObject* gameObject);
	void add_skinned_render_object(SkinnedGameObject* skinnedGameObject);
	
	void create_menu(float deltaTime);
	void deselect_index();
	glm::vec3 get_ray_vector();
	float get_window_width();
	float get_window_height();
	int get_selected_index();
	void select_entity(float xpos, float ypos);

	void draw_aabb(const glm::vec3& minAABB,const glm::vec3& maxAABB);
	void draw_rays();
	void draw_static(Shader* shader, Model* model, glm::mat4 modelMatrix);
	void draw_skinned(Model* model, glm::mat4 modelMatrix, std::vector<glm::mat4>* transform);
	void render_grass(glm::vec3 pos, Camera* camera);
};

#endif // !RENDERER_H