#define IMGUI_DEFINE_MATH_OPERATORS
#include "DebugMenu.h"

static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
static bool useSnap(false);
float snap[3] = { 1.0f, 1.0f, 1.0f };
		MENU_TYPE type = MENU_TYPE::PLAYER;
Map* m_map;

void DebugMenu::init(GLFWwindow* glfwWindow) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void DebugMenu::load_map(Map* map)
{
    m_map = map;
}

void DebugMenu::create_menu(float deltaTime) 
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(m_map->camera->m_windowHeight / 3, 0), ImGuiCond_Always);

    ImGui::Begin("Debug Menu");
    
    display_fps(deltaTime);
    draw_camera_position();
    display_player_velocity();
    show_all_entities();
    
    draw_mouse_pos();
    draw_entity_hierarchy();

    int selectedIndex = Renderer::get_selected_index();
    if (selectedIndex != -1) {
        GameObject* entity = selectedIndex < m_map->entities.size() ? m_map->entities[selectedIndex] : m_map->players[selectedIndex - m_map->entities.size()];
        draw_entity_properties(entity);
    }

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugMenu::show_scripts(GameObject* entity)
{
    if (entity->actions.size() > 0)
    {
        ImGui::Text("Scripts");

        for (const auto& actions : entity->actions)
        {
            ImGui::Text(actions.first.c_str());
        }
    }
}

void DebugMenu::show_all_entities()
{
    if (ImGui::CollapsingHeader("Map entities")) {
        for (const auto& pair : m_map->entities)
        {
            if (ImGui::Selectable((pair.second->GameModel->fileName + " " + ErlMath::vec3_to_string(pair.second->Position)).c_str())) {
                Renderer::set_selected_index(pair.first);
            }
        }
    }
}

void DebugMenu::display_fps(float deltaTime) {
    std::string fps = "fps = " + std::to_string((int)(1.0f / deltaTime));
    ImGui::Text(fps.c_str());
}

void DebugMenu::draw_entity_hierarchy() {
    if (ImGui::CollapsingHeader("Entity Hierarchy")) {
        for (uint16_t i = 0; i < AssetManager::get_num_loaded_assets(); i++) {
            if (ImGui::Selectable(AssetManager::get_model(i)->fileName.c_str())) {
                GameObject* entity = new GameObject(i, AssetManager::get_model(i), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f), true);
                Renderer::add_render_object(entity);
                ErlPhysics::add_physics_object(entity);
                m_map->entities[entity->instanceId] = entity;
                Renderer::set_selected_index(entity->instanceId);
            }
        }
    }
}

void DebugMenu::display_player_velocity()
{
    if (m_map->players.size() > 0)
    {
        ImGui::Text("Velocity:");
        ImGui::Text("X: %.2f", m_map->players[0]->Velocity.x);
        ImGui::Text("Y: %.2f", m_map->players[0]->Velocity.y);
        ImGui::Text("Z: %.2f", m_map->players[0]->Velocity.z);
    }
}
   
void DebugMenu::draw_entity_properties(GameObject* entity) {
    ImGui::Text("Entity %zu", Renderer::get_selected_index());

    glm::mat4 modelMatrix = entity->ModelMatrix;
    glm::mat4 view = m_map->camera->get_view_matrix();
    glm::mat4 projection = m_map->camera->get_projection_matrix();
    glm::vec3 position, scale, rotation;

    if (ImGui::IsKeyPressed(ImGuiKey_1))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey_2))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(ImGuiKey_3))
        mCurrentGizmoOperation = ImGuizmo::SCALE;
    if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
        mCurrentGizmoOperation = ImGuizmo::SCALE;
    float matrixTranslation[3], matrixRotation[3], matrixScale[3];
   
    ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(modelMatrix), matrixTranslation, matrixRotation, matrixScale);
    ImGui::InputFloat3("Tr", matrixTranslation);
    ImGui::InputFloat3("Rt", matrixRotation);
    ImGui::InputFloat3("Sc", matrixScale);
    
    ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, glm::value_ptr(modelMatrix));

    entity->ModelMatrix = modelMatrix;

    if (mCurrentGizmoOperation != ImGuizmo::SCALE)
    {
        if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
            mCurrentGizmoMode = ImGuizmo::LOCAL;
        ImGui::SameLine();
        if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
            mCurrentGizmoMode = ImGuizmo::WORLD;
    }

    if (ImGui::IsKeyPressed(ImGuiKey_S))
        useSnap = !useSnap;
    ImGui::Checkbox(" ", &useSnap);
    ImGui::SameLine();
    switch (mCurrentGizmoOperation)
    {
    case ImGuizmo::TRANSLATE:
        ImGui::InputFloat3("Snap", &snap[0]);
        break;
    case ImGuizmo::ROTATE:
        ImGui::InputFloat("Angle Snap", &snap[0]);
        break;
    case ImGuizmo::SCALE:
        ImGui::InputFloat("Scale Snap", &snap[0]);
        break;
    }
    ImGuizmo::SetRect(0.0f, 0.0f, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
    ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), mCurrentGizmoOperation, mCurrentGizmoMode, glm::value_ptr(modelMatrix), NULL, &snap[0]);

    if (ImGuizmo::IsUsing()) {
        entity->ModelMatrix = modelMatrix;
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(modelMatrix), glm::value_ptr(position), glm::value_ptr(rotation), glm::value_ptr(scale));
        if (entity->instanceId >= m_map->entities.size())
        {
            m_map->playerStarts[entity->instanceId - m_map->entities.size()] = position;
        }
        entity->Position = position;
        entity->Rotation = rotation;
        entity->Size = scale;
    }

    show_scripts(entity);
}

void DebugMenu::draw_mouse_pos()
{
    ImVec2 mousePos = ImGui::GetIO().MousePos;
    ImGui::Text("Mouse Position:");
    ImGui::Text("X: %.2f", mousePos.x);
    ImGui::Text("Y: %.2f", mousePos.y);
}

void DebugMenu::draw_camera_position() {
    ImGui::Text("Camera Position:");
    ImGui::Text("X: %.2f", m_map->camera->get_camera_pos().x);
    ImGui::Text("Y: %.2f", m_map->camera->get_camera_pos().y);
    ImGui::Text("Z: %.2f", m_map->camera->get_camera_pos().z);

    ImGui::Text("Camera Front:");
    ImGui::Text("X: %.2f", m_map->camera->get_camera_front().x);
    ImGui::Text("Y: %.2f", m_map->camera->get_camera_front().y);
    ImGui::Text("Z: %.2f", m_map->camera->get_camera_front().z);

    ImGui::Text("Camera Up:");
    ImGui::Text("X: %.2f", m_map->camera->get_camera_up().x);
    ImGui::Text("Y: %.2f", m_map->camera->get_camera_up().y);
    ImGui::Text("Z: %.2f", m_map->camera->get_camera_up().z);
}

void DebugMenu::shut_down() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void DebugMenu::set_controls()
{
    InputManager::set_key_and_mouse_binding(GLFW_KEY_LEFT_SHIFT, GLFW_MOUSE_BUTTON_MIDDLE, new MoveCameraCommand(m_map->camera, MOUSE_DRAG), true);
    InputManager::set_mouse_binding(-1, new MoveCameraCommand(m_map->camera, SCROLL));
    InputManager::set_mouse_binding(GLFW_MOUSE_BUTTON_LEFT, new SelectEntityCommand(false));
    InputManager::set_mouse_binding(GLFW_MOUSE_BUTTON_RIGHT, new SelectEntityCommand(true));
    InputManager::set_mouse_binding(GLFW_MOUSE_BUTTON_MIDDLE, new MoveCameraCommand(m_map->camera, ORBIT));
    
    AddRemoveEntityCommand are(m_map, true);
    AddRemoveEntityCommand are2(m_map, false);

    InputManager::set_key_binding(GLFW_KEY_N, [are](float dt) mutable { are.execute(dt); });
    InputManager::set_key_binding(GLFW_KEY_R, [are2](float dt) mutable { are2.execute(dt); });
}

void DebugMenu::clear_controls()
{
    InputManager::remove_key_and_mouse_binding(GLFW_KEY_LEFT_SHIFT, GLFW_MOUSE_BUTTON_MIDDLE);
    InputManager::remove_mouse_binding(-1);
    InputManager::remove_mouse_binding(GLFW_MOUSE_BUTTON_LEFT);
    InputManager::remove_mouse_binding(GLFW_MOUSE_BUTTON_RIGHT);
    InputManager::remove_mouse_binding(GLFW_MOUSE_BUTTON_MIDDLE);
    InputManager::remove_key_binding(GLFW_KEY_N);
    InputManager::remove_key_binding(GLFW_KEY_R);
}