#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include<map>
#include <vector>
#include<GLFW/glfw3.h>
#include"Command.h"

/*GLFW_GAMEPAD_BUTTON_A,
GLFW_GAMEPAD_BUTTON_B,
GLFW_GAMEPAD_BUTTON_X,
GLFW_GAMEPAD_BUTTON_Y,
GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
GLFW_GAMEPAD_BUTTON_BACK,
GLFW_GAMEPAD_BUTTON_START,
GLFW_GAMEPAD_BUTTON_GUIDE,
GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,
GLFW_GAMEPAD_BUTTON_DPAD_UP,
GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
GLFW_GAMEPAD_BUTTON_DPAD_DOWN
GLFW_GAMEPAD_BUTTON_DPAD_LEFT.*
GLFW_GAMEPAD_AXIS_LEFT_X,
GLFW_GAMEPAD_AXIS_LEFT_Y,
GLFW_GAMEPAD_AXIS_RIGHT_X,
GLFW_GAMEPAD_AXIS_RIGHT_Y,
GLFW_GAMEPAD_AXIS_LEFT_TRIGGER
GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER*/

struct GamepadState {
    bool buttons[15] = { false };
    float axes[6] = { 0.0f }; //(2 for left stick, 2 for right stick, 2 for triggers)
};

namespace InputManager {
    static bool Keys[1024] = { false };
    static bool KeysProcessed[1024] = { false };
    static bool MouseButtons[7] = { false };
    static bool GamepadButtonsProcessed[15] = { false };
    static GamepadState gamepadStates[4] = { false };

    void init(GLFWwindow* window);
    void update();
    double get_scroll_value();
    bool is_mouse_button_pressed();
    void update_cursor();
    void set_key_binding(int key, Command* command);
    void set_mouse_binding(int key, Command* command);
    void remove_mouse_binding(int key);
    void set_gamepad_binding(std::vector<int> key, Command* command);

    double get_xpos();
    double get_last_xpos();
    double get_ypos();
    double get_last_ypos();
    double get_yaw();
    double get_pitch();
};
#endif // !INPUT_MANAGER_H
