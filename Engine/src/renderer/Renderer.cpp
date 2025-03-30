#include"Renderer.h"

std::vector<SkinnedGameObject*> m_skinned_entities;
std::vector<GameObject*> m_entities, m_lights;

Shader* shaderProgram;
Shader* lightShaderProgram;
Shader* skinnedShaderProgram;
Shader* pickingShaderProgram;
Shader* cubemapShaderProgram;
Shader* grassShaderProgram;
Shader* lineShaderProgram;

Camera* m_camera;
SkyBox* m_skybox;
DebugMenu* debugMenu;

glm::mat4 view, projection;
glm::vec3 delta(0.0f);
glm::vec3 out_origin;
glm::vec3 out_direction = glm::vec3(0.0f, 0.0f, 0.0f);

//move later
float tMin = 0.0f;
float tMax = 100000.0f;

int selectedIndex = -1;
int m_windowWidth, m_windowHeight;

void Renderer::init_render(GLFWwindow* window)
{
	shaderProgram = new Shader("C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/default.vert.glsl", "C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/default.frag.glsl");
	lightShaderProgram = new Shader("C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/light.vert.glsl", "C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/light.frag.glsl");
	skinnedShaderProgram = new Shader("C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/skinned.vert.glsl", "C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/skinned.frag.glsl");
	pickingShaderProgram = new Shader("C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/picking.vert.glsl", "C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/picking.frag.glsl");
	cubemapShaderProgram = new Shader("C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/cubemap.vert.glsl", "C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/cubemap.frag.glsl");
	lineShaderProgram = new Shader("C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/line.vert.glsl", "C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/line.frag.glsl");
	grassShaderProgram = new Shader("C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/grass.vert.glsl", "C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/grass.frag.glsl", "C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/grass.gs.glsl");
	debugMenu = new DebugMenu(window);
	glfwGetWindowSize(window, &m_windowWidth, &m_windowHeight);
}

void Renderer::add_sky_box(SkyBox* skybox)
{
	m_skybox = skybox;
}

void Renderer::add_render_object(GameObject* gameObject)
{
	m_entities.push_back(gameObject);
}

void Renderer::remove_render_object(int index)
{
	m_entities.erase(m_entities.begin() + index);
	selectedIndex = -1;
}

std::vector<GameObject*> Renderer::get_rendered_entities()
{
	return m_entities;
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
	view = m_camera->get_view_matrix();
	projection = m_camera->get_projection_matrix();

	//draw first for environment mapping
	cubemapShaderProgram->use();
	cubemapShaderProgram->setInt("skybox", 1);
	cubemapShaderProgram->setMat4("view", glm::mat4(glm::mat3(view)));
	cubemapShaderProgram->setMat4("projection", projection);
	m_skybox->draw();

	
	skinnedShaderProgram->use();
	if (!m_lights.empty())
	{
		skinnedShaderProgram->setVec3("lightPos", m_lights[0]->Position);
	}
	else {
		skinnedShaderProgram->setVec3("lightPos", glm::vec3(0.0f));
	}
	skinnedShaderProgram->setVec3("lightColor", glm::vec3(1.0f));
	skinnedShaderProgram->setInt("skybox", 1);
	
	for (SkinnedGameObject* skinned_entity : m_skinned_entities)
	{
		//draw_aabb(skinned_entity->GameModel->getMinAABB(), skinned_entity->GameModel->getMaxAABB());
		Renderer::draw_skinned(skinned_entity->GameModel, skinned_entity->ModelMatrix, skinned_entity->transforms);
	}

	shaderProgram->use();
	if (!m_lights.empty())
	{
		shaderProgram->setVec3("lightPos", m_lights[0]->Position);
	}
	else {
		shaderProgram->setVec3("lightPos", glm::vec3(0.0f));
	}
	shaderProgram->setVec3("lightColor", glm::vec3(1.0f));

	for (int i = 0; i < m_entities.size(); i++)
	{
		shaderProgram->setBool("selected", i == Renderer::get_selected_index());
		Renderer::draw_static(shaderProgram, m_entities[i]->GameModel, m_entities[i]->ModelMatrix);
	}

	//render_grass(glm::vec3(0.0f), camera);
	draw_ray(out_direction);

	lightShaderProgram->use();
	lightShaderProgram->setVec3("lightColor", glm::vec3(0.98f, 0.80f, 0.70f));
	for (GameObject* light : m_lights)
	{
		Renderer::draw_static(lightShaderProgram, light->GameModel, light->ModelMatrix);
	}

}

void Renderer::render_grass(glm::vec3 pos, Camera* camera)
{
	grassShaderProgram->use();
	grassShaderProgram->setMat4("VP", camera->get_projection_matrix() * camera->get_view_matrix());
	grassShaderProgram->setVec3("cameraPos", camera->get_camera_pos());

	glm::vec3 points[10000];
	int k = 0;

	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			points[k] = glm::vec3(i * 5, 0.0f, j * 5);
			k++;
		}
	}

	unsigned int VAO, VBO;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::get_grass());

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);  

	glDrawArrays(GL_POINTS, 0, 10000);

	glDisableVertexAttribArray(0);
}

void Renderer::draw_skinned(Model* model, glm::mat4 modelMatrix, std::vector<glm::mat4>* transform)
{
	skinnedShaderProgram->setMat4("view", view);
	skinnedShaderProgram->setMat4("projection", projection);

	for (int i = 0; i < transform->size(); ++i)
	{
		skinnedShaderProgram->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transform->at(i));
	}

	skinnedShaderProgram->setMat4("model", modelMatrix);
	model->draw(*skinnedShaderProgram);
}

void Renderer::draw_static(Shader* shader, Model* model, glm::mat4 modelMatrix)
{
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);
	shader->setMat4("model", modelMatrix);
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

void Renderer::draw_ray(glm::vec3 ray)
{
	if (ray != glm::vec3(0.0f))
	{
		lineShaderProgram->use();
		lineShaderProgram->setMat4("view", view);
		lineShaderProgram->setMat4("projection", projection);

		glm::vec3 startPoint = out_origin;
		ray *= 10000.0f;
		glm::vec3 endPoint = startPoint + ray;

		float vertices[] = {
			startPoint.x, startPoint.y, startPoint.z,
			endPoint.x, endPoint.y, endPoint.z,
		};

		unsigned int VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, 2);
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
}

// This is for the debug menu, not part of game rendering.
void Renderer::create_menu(float deltaTime)
{
	debugMenu->create_menu(m_entities, m_camera, deltaTime);
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
		Renderer::draw_static(pickingShaderProgram, m_entities[i]->GameModel, m_entities[i]->ModelMatrix);
		glGetError();
	}

	glDisableVertexAttribArray(0);
	glFlush();
	glFinish();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned char data[4];
	glReadPixels(xpos, m_windowHeight - ypos - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	int pickedID = data[0] + (data[1] * 256) + (data[2] * 256 * 256);

	if (!(pickedID == 0x00ffffff))
	{
		std::ostringstream oss;
		oss << "selected mesh id: " << pickedID;
		std::cout << oss.str() << std::endl;
		
		if(pickedID < (int)m_entities.size()) selectedIndex = pickedID;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::cast_ray(int xpos, int ypos) 
{
	float NDC_X = ((int)xpos * (2.0f / m_windowWidth)) - 1;
	float NDC_Y = -((int)ypos * (2.0f / m_windowHeight)) + 1;
	float near_plane_height = glm::tan(45 / 2.0f) * 0.1f;
	float aspect_ratio = (float)m_windowWidth / (float)m_windowHeight;

	float X_3D = NDC_X * near_plane_height * aspect_ratio;
	float Y_3D = NDC_Y * near_plane_height;

	glm::vec3 near_plane_point(X_3D, Y_3D, -0.1f); 
	near_plane_point = glm::inverse(view) * glm::vec4(near_plane_point, 1.0f);
	glm::vec3 cam_dir_vec = near_plane_point - m_camera->get_camera_pos();

	out_direction = glm::normalize(cam_dir_vec);
	out_origin = m_camera->get_camera_pos();
}

int Renderer::get_selected_index()
{
	return selectedIndex;
}

void Renderer::deselect_index()
{
	selectedIndex = -1;
}

glm::vec3 Renderer::get_ray_vector()
{
	return out_direction;
}