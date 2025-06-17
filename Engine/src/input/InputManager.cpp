#include"InputManager.h"

std::map<int, Command*> mouseBindings;
std::map<int, std::function<void(float)>> keyBindings;
std::map<int, int> doubleBindingMap;
std::map<DoubleBinding*, Command*> keyAndMouseBindings;
std::map<std::vector<int>, Command*> gamepadBindings;
GLFWwindow* m_window;

char response[50] = ""; 

double xpos, ypos = 0.0f;
double lastX;
double lastY;
double scrollY = 0;
double dragXStart = 0.0f, dragYStart = 0.0f;
double yaw = -90.0f, pitch = 0.0f;

int windowWidth = -1;
int windowHeight = -1;
bool firstMouse = true;
bool isTextInputEnabled = false;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void character_callback(GLFWwindow* window, unsigned int codepoint);
void run_key_commands(float deltaTime);
void run_mouse_commands(float deltaTime);
void run_key_and_mouse_commands(float deltaTime);
void run_gamepad_commands(float deltaTime);

void InputManager::init(GLFWwindow* window) {
    m_window = window;
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCharCallback(window, character_callback);
}

void InputManager::update(float deltaTime) {
    update_cursor();

    if(keyBindings.size() > 0) run_key_commands(deltaTime);
    if(mouseBindings.size() > 0) run_mouse_commands(deltaTime);
    if(keyAndMouseBindings.size() > 0) run_key_and_mouse_commands(deltaTime);
    if(gamepadBindings.size() > 0) run_gamepad_commands(deltaTime);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    scrollY = yoffset;
    Command* scrollCommand = mouseBindings[-1];
    if(scrollCommand) scrollCommand->execute(0.0f);
}

void character_callback(GLFWwindow* window, unsigned int codepoint) {
    std::cout << codepoint << std::endl;
    std::cout << "Input: " << static_cast<char>(codepoint) << std::endl;
    //limit characters to fifty
    if (isTextInputEnabled && strlen(response) < 49)
    {
        response[strlen(response)] = static_cast<char>(codepoint);
    }
}

double InputManager::get_scroll_value()
{
    return scrollY;
}

bool InputManager::is_mouse_button_pressed()
{
    return glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

void InputManager::set_key_binding(int key, std::function<void(float)> func)
{
    keyBindings[key] = func;
}

void InputManager::set_key_and_mouse_binding(int key, int mouse, Command* command, bool isDrag)
{
    doubleBindingMap[key] = mouse;
    doubleBindingMap[mouse] = key;

    keyAndMouseBindings[new DoubleBinding{key, mouse, false, isDrag}] = command;
}

void InputManager::set_mouse_binding(int key, Command* command)
{
    mouseBindings[key] = command;
}

void InputManager::remove_mouse_binding(int key)
{
    mouseBindings.erase(key);
}

void InputManager::remove_key_binding(int key)
{
    keyBindings.erase(key);
}

void InputManager::remove_all_key_bindings()
{
    keyBindings.clear();
}

void InputManager::remove_all_bindings()
{
    keyBindings.clear();
    keyAndMouseBindings.clear();
    mouseBindings.clear();
    gamepadBindings.clear();
}

void InputManager::remove_key_and_mouse_binding(int key, int mouse)
{
    doubleBindingMap.erase(key);
    doubleBindingMap.erase(mouse);

    keyAndMouseBindings.erase(new DoubleBinding{ key, mouse });
}

void InputManager::set_gamepad_binding(std::vector<int> key, Command* command)
{
    gamepadBindings[key] = command;
}

void InputManager::update_cursor()
{
    glfwGetWindowSize(m_window, &windowWidth, &windowHeight);
    glfwGetCursorPos(m_window, &xpos, &ypos);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos; // Inverted Y-axis
    lastX = xpos;
    lastY = ypos;

    double sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    Command* lookAroundCommand = mouseBindings[-2];
    if (lookAroundCommand) lookAroundCommand->execute(0.0f);
}

double InputManager::get_xpos()
{
    return xpos;
}

double InputManager::get_last_xpos()
{
    return dragXStart;
}

double InputManager::get_ypos()
{
    return ypos;
}

double InputManager::get_last_ypos()
{
    return dragYStart;
}

double InputManager::get_yaw()
{
    return yaw;
}

double InputManager::get_pitch()
{
    return pitch;
}

void InputManager::set_pitch(float pitchOffset)
{
    pitch += pitchOffset;
}

void InputManager::set_yaw(float m_yaw)
{
    yaw = m_yaw;
}

bool InputManager::are_multiple_keys_pressed(int firstButton, int secondButton)
{
    return (glfwGetKey(m_window, firstButton) == GLFW_PRESS && glfwGetKey(m_window, secondButton) == GLFW_PRESS) || 
        (glfwGetKey(m_window, firstButton) == GLFW_PRESS && glfwGetMouseButton(m_window, secondButton) == GLFW_PRESS) ||
        (glfwGetMouseButton(m_window, firstButton) == GLFW_PRESS && glfwGetKey(m_window, secondButton) == GLFW_PRESS);
}

bool InputManager::is_key_pressed(int button)
{
    return glfwGetKey(m_window, button) == GLFW_PRESS;
}

void InputManager::show_cursor(bool show)
{
    if (show)
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void InputManager::set_text_input_control()
{
    isTextInputEnabled = !isTextInputEnabled;
}

char* InputManager::get_text_input()
{
    return response;
}

void InputManager::set_text_input(const char* text)
{
    strncpy_s(response, text, sizeof(response) - 1);
    response[sizeof(response) - 1] = '\0';
}

void InputManager::clear_text_input()
{
    memset(response, 0, sizeof(response));
}

void InputManager::clear_text_input_last_char()
{
    int index = 0;
    while (response[index] != '\0')
    {
        index++;
    }
    if(index != 0) response[index -1] = '\0';
}

void run_gamepad_commands(float deltaTime)
{
    for (int i = 0; i < 4; ++i) {
        if (glfwJoystickIsGamepad(i)) {
            GLFWgamepadstate state;
            if (glfwGetGamepadState(i, &state)) {
                for (int button = 0; button < GLFW_GAMEPAD_BUTTON_LAST + 1; ++button) {
                    if (state.buttons[button] == GLFW_PRESS && !InputManager::GamepadButtonsProcessed[button]) {
                        if (gamepadBindings.find({ i, button }) != gamepadBindings.end()) {
                            gamepadBindings[{i, button}]->execute(deltaTime);
                            InputManager::GamepadButtonsProcessed[button] = true;
                        }
                    }
                    else if (state.buttons[button] == GLFW_RELEASE) {
                        InputManager::GamepadButtonsProcessed[button] = false;
                    }
                }

                // 4 for now, till using triggers
                for (int axis = 0; axis < 2; ++axis) {
                    if (fabs(state.axes[axis]) > 0.2f) {
                        gamepadBindings[{i, 100 - axis}]->execute(deltaTime, state.axes[axis]);
                    }
                }
            }
        }
    }
}

void run_key_commands(float deltaTime)
{
    for (auto& binding : keyBindings) {
        int key = binding.first;

        if (doubleBindingMap.count(key) == 0 ||
            (doubleBindingMap.count(key) > 0 && !InputManager::are_multiple_keys_pressed(key, doubleBindingMap.at(key))))
        {
            if (glfwGetKey(m_window, key) == GLFW_PRESS && !InputManager::KeysProcessed[key]) {
                binding.second(deltaTime);
                InputManager::KeysProcessed[key] = true;
                //KeysProcessed[key] = !command->isContinuous;
            }
            else if (glfwGetKey(m_window, key) == GLFW_RELEASE) {
                InputManager::KeysProcessed[key] = false;
            }
        }
    }
}

void run_mouse_commands(float deltaTime)
{
    for (auto& binding : mouseBindings) {

        int mouseButton = binding.first;
        Command* command = binding.second;

        if (mouseButton >= 0)
        {
            if (doubleBindingMap.count(mouseButton) == 0 ||
                (doubleBindingMap.count(mouseButton) > 0 && !InputManager::are_multiple_keys_pressed(mouseButton, doubleBindingMap.at(mouseButton))))
            {
                if (glfwGetMouseButton(m_window, mouseButton) == GLFW_PRESS && !InputManager::MouseProcessed[mouseButton]) {
                    if (mouseButton == GLFW_MOUSE_BUTTON_MIDDLE) {
                        glfwGetCursorPos(m_window, &dragXStart, &dragYStart);
                    }
                    command->execute(deltaTime);
                    InputManager::MouseProcessed[mouseButton] = true;
                }
                else if (glfwGetMouseButton(m_window, mouseButton) == GLFW_PRESS && InputManager::MouseProcessed[mouseButton]) {
                    if (mouseButton == GLFW_MOUSE_BUTTON_MIDDLE) {
                        command->execute(deltaTime);
                        glfwGetCursorPos(m_window, &dragXStart, &dragYStart);
                    }
                }
                else if (glfwGetKey(m_window, mouseButton) == GLFW_RELEASE) {
                    InputManager::MouseProcessed[mouseButton] = false;
                }
            }
        }
    }
}

void run_key_and_mouse_commands(float deltaTime)
{
    for (auto& binding : keyAndMouseBindings)
    {
        DoubleBinding* keyAndMouseButtons = binding.first;
        Command* command = binding.second;

        if (InputManager::are_multiple_keys_pressed(keyAndMouseButtons->firstButton, keyAndMouseButtons->secondButton) && !keyAndMouseButtons->buttonsProcessed) {
            //will need to be generic.
            if (keyAndMouseButtons->isDrag) {
                glfwGetCursorPos(m_window, &dragXStart, &dragYStart);
            }
            command->execute(deltaTime);
            keyAndMouseButtons->buttonsProcessed = true;
        }
        else if (InputManager::are_multiple_keys_pressed(keyAndMouseButtons->firstButton, keyAndMouseButtons->secondButton) && keyAndMouseButtons->buttonsProcessed) {
            if (keyAndMouseButtons->isDrag) {
                command->execute(deltaTime);
                glfwGetCursorPos(m_window, &dragXStart, &dragYStart);
            }
        }
        else if (glfwGetKey(m_window, keyAndMouseButtons->secondButton) == GLFW_RELEASE) {
            keyAndMouseButtons->buttonsProcessed = false;
        }
    }
}