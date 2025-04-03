#define IMGUI_DEFINE_MATH_OPERATORS
#include "DebugMenu.h"

static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
static bool useSnap(false);
float snap[3] = { 1.f, 1.f, 1.f };


DebugMenu::DebugMenu() {}

DebugMenu::DebugMenu(GLFWwindow* glfwWindow) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    glfwGetWindowSize(glfwWindow, &windowWidth, &windowHeight);
}

DebugMenu::~DebugMenu() {}

void DebugMenu::create_menu(std::vector<GameObject*>& entities, Camera* camera, float deltaTime) 
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(windowHeight / 3, 0), ImGuiCond_Always);

    ImGui::Begin("Debug Menu");
    
    display_fps(deltaTime);

    draw_entity_hierarchy(entities);
    draw_camera_position(camera);
    display_board_tiles(entities);
    draw_mouse_pos();
    draw_ray_cast();

    int selectedIndex = Renderer::get_selected_index();
    if (selectedIndex != -1) {
        GameObject* entity = entities[selectedIndex];
        draw_entity_properties(entity, camera);

        if (auto boardSpace = dynamic_cast<BoardSpace*>(entity)) {
            std::string id = "BoardSpace: " + std::to_string((int)boardSpace->assetId);
            ImGui::InputInt(id.c_str(), &boardSpace->nextSpaceIds[0], 1, 20);
        }
    }

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugMenu::display_board_tiles(std::vector<GameObject*> entities)
{
    if (ImGui::CollapsingHeader("BoardSpace Specific Properties"))
    {
        std::map<uint8_t, BoardSpace*> idToBoardSpaceMap;
        for (size_t i = 0; i < entities.size(); ++i)
        {
            if (auto boardSpace = dynamic_cast<BoardSpace*>(entities[i])) {
                idToBoardSpaceMap[boardSpace->assetId] = boardSpace;
                boardSpace->nextSpace[0] = idToBoardSpaceMap[boardSpace->nextSpaceIds[0]];
                ImGui::PushID(i);
                std::string id = "BoardSpace: " + std::to_string((int)boardSpace->assetId);
                ImGui::InputInt(id.c_str(), &boardSpace->nextSpaceIds[0], 1, 20);
                ImGui::PopID();
            }
        }
    }
}

void DebugMenu::display_fps(float deltaTime) {
    std::string fps = "fps = " + std::to_string((int)(1.0f / deltaTime));
    ImGui::Text(fps.c_str());
}

void DebugMenu::draw_entity_hierarchy(std::vector<GameObject*>& entities) {
    if (ImGui::CollapsingHeader("Entity Hierarchy")) {
        for (uint16_t i = 0; i < AssetManager::get_num_loaded_assets(); i++) {
            if (ImGui::Selectable(AssetManager::get_model(i)->directory.c_str())) {
                GameObject* entity = new GameObject(i, AssetManager::get_model(i), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f), true);
                entities.push_back(entity);
                Renderer::add_render_object(entity);
            }
        }
    }
}

void DebugMenu::draw_entity_properties(GameObject* entity, Camera* camera) {
    ImGui::Text("Entity %zu", Renderer::get_selected_index());

    glm::mat4 modelMatrix = entity->ModelMatrix;
    glm::mat4 view = camera->get_view_matrix();
    glm::mat4 projection = camera->get_projection_matrix();
    glm::vec3 position, scale, rotation;

    static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
    static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
    static bool boundSizing = false;
    static bool boundSizingSnap = false;

    if (ImGui::IsKeyPressed(ImGuiKey_T))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey_E))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(ImGuiKey_R))
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
        entity->Position = position;
        entity->Rotation = rotation;
        entity->Size = scale;
    }
}

void DebugMenu::draw_mouse_pos()
{
    ImVec2 mousePos = ImGui::GetIO().MousePos;
    ImGui::Text("Mouse Position:");
    ImGui::Text("X: %.2f", mousePos.x);
    ImGui::Text("Y: %.2f", mousePos.y);
}

void DebugMenu::draw_ray_cast()
{
    glm::vec3 ray = Renderer::get_ray_vector();
    ImGui::Text("Ray:");
    ImGui::Text("%.2f, %.2f, %.2f", ray.x, ray.y, ray.z);
}

void DebugMenu::draw_camera_position(Camera* camera) {
    ImGui::Text("Camera Position:");
    ImGui::Text("X: %.2f", camera->get_camera_pos().x);
    ImGui::Text("Y: %.2f", camera->get_camera_pos().y);
    ImGui::Text("Z: %.2f", camera->get_camera_pos().z);

    ImGui::Text("Camera Front:");
    ImGui::Text("X: %.2f", camera->get_camera_front().x);
    ImGui::Text("Y: %.2f", camera->get_camera_front().y);
    ImGui::Text("Z: %.2f", camera->get_camera_front().z);

    ImGui::Text("Camera Up:");
    ImGui::Text("X: %.2f", camera->get_camera_up().x);
    ImGui::Text("Y: %.2f", camera->get_camera_up().y);
    ImGui::Text("Z: %.2f", camera->get_camera_up().z);
}

void DebugMenu::create_new_map() 
{
}

void DebugMenu::load_board_space_data(std::vector<BoardSpace*> boardSpaces)
{
    ImGui::Text("BoardData:");
    for (BoardSpace* boardSpace : boardSpaces)
    {
        ImGui::Text("spaceID", boardSpace->nextSpaceIds[0]);
    }
}

void DebugMenu::shut_down() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}