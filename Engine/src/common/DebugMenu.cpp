#define IMGUI_DEFINE_MATH_OPERATORS
#include"DebugMenu.h"

DebugMenu::DebugMenu()
{
}

DebugMenu::DebugMenu(GLFWwindow* glfwWindow)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

DebugMenu::~DebugMenu()
{
}

void DebugMenu::create_menu(std::vector<GameObject*> entities, Camera* camera, float deltaTime, int roll)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	std::string fps = std::to_string((int)(1.0f / deltaTime));
	fps = "fps = " + fps;
	char const* fpsChar = fps.c_str();

	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_Always);

	ImGui::Begin("Debug Menu");
	ImGui::Text(fpsChar);

	int selectedIndex = Renderer::get_selected_index();
	if (selectedIndex != -1)
	{
		GameObject* entity = entities[selectedIndex];

		ImGui::Text("Entity %zu", selectedIndex);

		ImGui::Text("Position:");
		ImGui::InputFloat(("X: %.2f" + std::to_string(selectedIndex)).c_str(), &entity->Position.x, 1.0f, 1000.0f, "%.1f");
		ImGui::InputFloat(("Y: %.2f" + std::to_string(selectedIndex)).c_str(), &entity->Position.y, 1.0f, 1000.0f, "%.1f");
		ImGui::InputFloat(("Z: %.2f" + std::to_string(selectedIndex)).c_str(), &entity->Position.z, 1.0f, 1000.0f, "%.1f");

		ImGui::Text("Size:");
		ImGui::InputFloat(("Size X##" + std::to_string(selectedIndex)).c_str(), &entity->Size.x, 1.0f, 100.0f, "%.1f");
		ImGui::InputFloat(("Size Y##" + std::to_string(selectedIndex)).c_str(), &entity->Size.y, 1.0f, 100.0f, "%.1f");
		ImGui::InputFloat(("Size Z##" + std::to_string(selectedIndex)).c_str(), &entity->Size.z, 1.0f, 100.0f, "%.1f");

		ImGui::Text("Rotation:");
		ImGui::InputFloat(("Rotation X##" + std::to_string(selectedIndex)).c_str(), &entity->Rotation.x, 1.0f, 180.0f, "%.1f");
		ImGui::InputFloat(("Rotation Y##" + std::to_string(selectedIndex)).c_str(), &entity->Rotation.y, 1.0f, 180.0f, "%.1f");
		ImGui::InputFloat(("Rotation Z##" + std::to_string(selectedIndex)).c_str(), &entity->Rotation.z, 1.0f, 180.0f, "%.1f");

		ImGui::Separator();

		ImVec2 mousePos = ImGui::GetIO().MousePos;

		ImGui::Text("Mouse Position:");
		ImGui::Text("X: %.2f", mousePos.x);
		ImGui::Text("Y: %.2f", mousePos.y);

		

		glm::mat4 modelMatrix = entity->ModelMatrix;
		glm::mat4 view = camera->get_view_matrix();
		glm::mat4 projection = camera->get_projection_matrix();

		// Set up the gizmo for manipulation
		ImGuizmo::SetRect(0.0f, 0.0f, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
		ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection),
			ImGuizmo::TRANSLATE | ImGuizmo::ROTATE | ImGuizmo::SCALE,
			ImGuizmo::LOCAL, glm::value_ptr(modelMatrix));

		if (ImGuizmo::IsUsing()) {
			// Update entity's transformation if gizmo is used

			//InputManager::set_mouse_binding(GLFW_MOUSE_BUTTON_LEFT, nullptr);
			entity->ModelMatrix = modelMatrix;
			//entity->Position = glm::vec3(modelMatrix[3]);
		}
		else {
			//InputManager::set_mouse_binding(GLFW_MOUSE_BUTTON_LEFT, new SelectEntityCommand(InputManager::get_xpos(), InputManager::get_ypos(), false));
		}
	}

	ImGui::Text("Camera Position:");
	ImGui::Text("X: %.2f", camera->getCameraPos().x);
	ImGui::Text("Y: %.2f", camera->getCameraPos().y);
	ImGui::Text("Z: %.2f", camera->getCameraPos().z);

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugMenu::shut_down()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}