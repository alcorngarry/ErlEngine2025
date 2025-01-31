#include"Renderer.h"

Shader* shaderProgram;
Shader* lightShaderProgram;
Shader* skinnedShaderProgram;
Shader* pickingShaderProgram;
Shader* cubemapShaderProgram;

std::vector<SkinnedGameObject*> m_skinned_entities;
std::vector<GameObject*> m_entities, m_lights;
Camera* m_camera;
SkyBox* m_skybox;

DebugMenu* debugMenu;

glm::mat4 view, projection;

int selectedIndex = -1;

void Renderer::init_render(GLFWwindow* window)
{
	shaderProgram = new Shader("C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/default.vert.glsl", "C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/default.frag.glsl");
	lightShaderProgram = new Shader("C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/light.vert.glsl", "C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/light.frag.glsl");
	skinnedShaderProgram = new Shader("C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/skinned.vert.glsl", "C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/skinned.frag.glsl");
	pickingShaderProgram = new Shader("C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/picking.vert.glsl", "C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/picking.frag.glsl");
	cubemapShaderProgram = new Shader("C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/cubemap.vert.glsl", "C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/cubemap.frag.glsl");
	debugMenu = new DebugMenu(window);
}

void Renderer::add_sky_box(SkyBox* skybox)
{
	m_skybox = skybox;
}

void Renderer::add_render_object(GameObject* gameObject)
{
	m_entities.push_back(gameObject);
}

void Renderer::add_light_render_object(GameObject* gameObject)
{
	m_lights.push_back(gameObject);
}

void Renderer::add_skinned_render_object(SkinnedGameObject* skinnedGameObject)
{
	m_skinned_entities.push_back(skinnedGameObject);
}

void Renderer::render(Camera* camera)
{
	m_camera = camera;
	view = camera->get_view_matrix();
	projection = camera->get_projection_matrix();

	//draw first for environment mapping
	cubemapShaderProgram->use();
	cubemapShaderProgram->setInt("skybox", 0);
	cubemapShaderProgram->setMat4("view", glm::mat4(glm::mat3(camera->get_view_matrix())));
	cubemapShaderProgram->setMat4("projection", projection);
	m_skybox->draw();

	skinnedShaderProgram->use();
	skinnedShaderProgram->setVec3("lightPos", m_lights[0]->Position);
	skinnedShaderProgram->setVec3("lightColor", glm::vec3(1.0f));
	skinnedShaderProgram->setInt("skybox", 0);
	
	for (SkinnedGameObject* skinned_entity : m_skinned_entities)
	{
		draw_aabb(skinned_entity->GameModel->getMinAABB(), skinned_entity->GameModel->getMaxAABB());
		Renderer::draw_skinned(skinned_entity->GameModel, skinned_entity->Position, skinned_entity->Size, skinned_entity->Rotation, skinned_entity->transforms);
	}

	shaderProgram->use();
	shaderProgram->setVec3("lightPos", m_lights[0]->Position);
	shaderProgram->setVec3("lightColor", glm::vec3(1.0f));

	for (int i = 0; i < m_entities.size(); i++)
	{
		shaderProgram->setBool("selected", i == Renderer::get_selected_index());
		shaderProgram->setInt("objectId", m_entities[i]->id);
		Renderer::draw_static(shaderProgram, m_entities[i]->GameModel, m_entities[i]->Position, m_entities[i]->Size, m_entities[i]->Rotation);
	}

	lightShaderProgram->use();
	lightShaderProgram->setVec3("lightColor", glm::vec3(1.0f));
	for (GameObject* light : m_lights)
	{
		Renderer::draw_static(lightShaderProgram, light->GameModel, light->Position, light->Size, light->Rotation);
	}
}

void Renderer::draw_skinned(Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, std::vector<glm::mat4>* transform)
{
	skinnedShaderProgram->setMat4("view", view);
	skinnedShaderProgram->setMat4("projection", projection);

	for (int i = 0; i < transform->size(); ++i)
	{
		skinnedShaderProgram->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transform->at(i));
	}

	glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), pos);

	modelMat = glm::rotate(modelMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMat = glm::rotate(modelMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMat = glm::rotate(modelMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMat = glm::scale(modelMat, size);

	skinnedShaderProgram->setMat4("model", modelMat);

	model->draw(*skinnedShaderProgram);
}

void Renderer::draw_static(Shader* shader, Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation)
{
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);

	glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), pos);
	
	modelMat = glm::rotate(modelMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMat = glm::rotate(modelMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMat = glm::rotate(modelMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMat = glm::scale(modelMat, size);

	shader->setMat4("model", modelMat);
	model->draw(*shaderProgram);
}

void Renderer::draw_aabb(const glm::vec3& minAABB, const glm::vec3& maxAABB)
{
	const GLfloat vertices[] = {
		minAABB.x, minAABB.y, minAABB.z,
		minAABB.x, minAABB.y, maxAABB.z,
		minAABB.x, maxAABB.y, minAABB.z,
		minAABB.x, maxAABB.y, maxAABB.z,
		maxAABB.x, minAABB.y, minAABB.z,
		maxAABB.x, minAABB.y, maxAABB.z,
		maxAABB.x, maxAABB.y, minAABB.z,
		maxAABB.x, maxAABB.y,maxAABB.z
	};

	const GLuint indices[] = {
		0, 1, 1, 3, 3, 2, 2, 0,
		4, 5, 5, 7, 7, 6, 6, 4,
		0, 4, 1, 5, 2, 6, 3, 7
	};

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	glBindVertexArray(VAO);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

// This is for the debug menu, not part of game rendering.

void Renderer::create_menu(float deltaTime)
{
	debugMenu->create_menu(m_entities, m_camera, deltaTime);
}

void Renderer::create_menu(float deltaTime, int roll)
{
	debugMenu->create_menu(m_entities, m_camera, deltaTime, roll);
}

void Renderer::select_entity(float xpos, float ypos)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pickingShaderProgram->use();

	pickingShaderProgram->setMat4("view", view);
	pickingShaderProgram->setMat4("projection", projection);

	for (int i = 0; i < m_entities.size(); i++)
	{
		int r = (i & 0x000000FF) >> 0;
		int g = (i & 0x0000FF00) >> 8;
		int b = (i & 0x00FF0000) >> 16;

		pickingShaderProgram->setVec4("PickingColor", glm::vec4((float)r / 255.0, (float)g / 255.0, (float)b / 255.0, 1.0f));
		Renderer::draw_static(pickingShaderProgram, m_entities[i]->GameModel, m_entities[i]->Position, m_entities[i]->Size, m_entities[i]->Rotation);
		glGetError();
	}

	glDisableVertexAttribArray(0);
	glFlush();
	glFinish();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned char data[4];
	glReadPixels(xpos, 1080 - ypos - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	int pickedID = data[0] + (data[1] * 256) + (data[2] * 256 * 256);

	if (!(pickedID == 0x00ffffff))
	{
		std::ostringstream oss;
		oss << "selected mesh id: " << pickedID;
		std::cout << oss.str() << std::endl;

		selectedIndex = pickedID;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int Renderer::get_selected_index()
{
	return selectedIndex;
}

void Renderer::deselect_index()
{
	selectedIndex = -1;
}

