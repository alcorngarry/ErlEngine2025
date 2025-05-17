#ifndef RENDERER_H
#define RENDERER_H
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<stb_image/stb_image.h>
#include<math/ErlMath.h>
#include"Shader.h"
#include"game_objects/Model.h"
#include"game_objects/SkinnedGameObject.h"
#include"ui/UIManager.h"
#include"common/AssetManager.h"
#include"ui/UIElement.h"
#include"math/physics/ErlPhysics.h"

namespace Renderer {
	//not implemented check if wanted
	struct Erl3DRenderItem {
		Model* model;
		glm::mat4 transform;
	};

	void init_render(GLFWwindow* window);
	void render(Camera* camera);

	void add_sky_box(SkyBox* skybox);
	void add_render_object(GameObject* gameObject);
	void remove_render_object(uint16_t uniqueID);
	void add_light_render_object(GameObject* gameObject);
	void add_skinned_render_object(SkinnedGameObject* skinnedGameObject);
	
	void deselect_index();
	float get_window_width();
	float get_window_height();
	int get_selected_index();
	void set_selected_index(int i);
	void select_entity(float xpos, float ypos);

	void draw_aabb(const glm::vec3& minAABB, const glm::vec3& maxAABB, const glm::mat4& model);
	void draw_rays();
	void draw_static(Shader* shader, Model* model, glm::mat4 modelMatrix);
	void draw_skinned(Model* model, glm::mat4 modelMatrix, std::vector<glm::mat4>* transform);
	void render_grass(glm::vec3 pos, Camera* camera);
};

#endif // !RENDERER_H