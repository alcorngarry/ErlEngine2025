#include"InputManager.h"

std::map<int, Command*> keyBindings;
std::map<int, Command*> mouseBindings;
std::map<int, Command*> gamepadBindings;
GLFWwindow* m_window;

int windowWidth = 1920;
int windowHeight = 1080;

double xpos, ypos = 0.0f;
bool firstMouse = true;
double lastX = 1920 / 2.0f;
double lastY = 1080 / 2.0f;
double yaw = -90.0f, pitch = 0.0f;

void InputManager::init(GLFWwindow* window) {
    m_window = window;
}

void InputManager::update() {

    update_cursor();
   
    //command structure controls // for players
    for (auto& binding : keyBindings) {
        int key = binding.first;
        Command* command = binding.second;

        for (int keyIndex = 0; keyIndex < 1024; ++keyIndex) {
            //for moving camera... fix maybe later
            if ((key == GLFW_KEY_W || key == GLFW_KEY_A || key == GLFW_KEY_S || key == GLFW_KEY_D) && glfwGetKey(m_window, key) == GLFW_PRESS)
            {
                command->execute();
            } else if (glfwGetKey(m_window, key) == GLFW_PRESS && !KeysProcessed[key]) {
                command->execute();
                KeysProcessed[key] = true;
            }
            else if (glfwGetKey(m_window, key) == GLFW_RELEASE) {
                KeysProcessed[key] = false;
            }
        }
    }

    for (auto& binding : mouseBindings) {
        int mouseButton = binding.first;
        Command* command = binding.second;

        for (int mouseButtonIndex = 0; mouseButtonIndex < 7; ++mouseButtonIndex) {
            if (glfwGetMouseButton(m_window, mouseButton) == GLFW_PRESS) {
                command->execute();
            }
        }
    }

    for (int button = 0; button < 7; ++button) {
        MouseButtons[button] = glfwGetMouseButton(m_window, button) == GLFW_PRESS;
    }

    //four max controllers
    for (int i = 0; i < 4; ++i) {
        if (glfwJoystickIsGamepad(i)) {
            GLFWgamepadstate state;
            if (glfwGetGamepadState(i, &state)) {
                for (int button = 0; button < GLFW_GAMEPAD_BUTTON_LAST + 1; ++button) {
                    if (state.buttons[button] == GLFW_PRESS && !GamepadButtonsProcessed[button]) {
                        if (gamepadBindings.find(button) != gamepadBindings.end()) {
                            gamepadBindings[button]->execute();
                            GamepadButtonsProcessed[button] = true;
                        }
                    }
                    else if (state.buttons[button] == GLFW_RELEASE) {
                        GamepadButtonsProcessed[button] = false;
                    }
                }

                for (int axis = 0; axis < GLFW_GAMEPAD_AXIS_LAST; ++axis) {
                    if (fabs(state.axes[axis]) > 0.2f) {
                    }
                }
            }
        }
    }
}

void InputManager::set_key_binding(int key, Command* command)
{
    keyBindings[key] = command;
}

void InputManager::set_mouse_binding(int key, Command* command)
{
    mouseBindings[key] = command;
}

void InputManager::set_gamepad_binding(int key, Command* command)
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
}

double InputManager::get_xpos()
{
    return xpos;
}

double InputManager::get_last_xpos()
{
    return lastX;
}

double InputManager::get_ypos()
{
    return ypos;
}

double InputManager::get_last_ypos()
{
    return lastY;
}

double InputManager::get_yaw()
{
    return yaw;
}


double InputManager::get_pitch()
{
    return pitch;
}