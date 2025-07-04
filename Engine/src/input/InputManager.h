#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include<map>
#include<vector>
#include<string>
#include<GLFW/glfw3.h>
#include"Command.h"
#include"commands/ToggleConsoleCommand.h"
#include<functional>

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

struct DoubleBinding {
    int firstButton = 0;
    int secondButton = 0;
    bool buttonsProcessed = false;
    bool isDrag = false;
};

namespace InputManager {
    static bool KeysProcessed[1024] = { false };
    static bool MouseProcessed[7] = { false };
    static bool GamepadButtonsProcessed[15] = { false };
    static GamepadState gamepadStates[4] = { false };
    static bool GamePadConnected[2] = { false };

    void init(GLFWwindow* window);
    void update(float deltaTime);
    void handle_gamepad_command();
    double get_scroll_value();
    bool is_mouse_button_pressed();
    void update_cursor();
    void update_text_input();
    void set_key_binding(int key, std::function<void(float)>);
    void set_mouse_binding(int key, Command* command);
    void remove_mouse_binding(int key);
    void remove_key_binding(int key);
    void remove_key_and_mouse_binding(int key, int mouse);
    void remove_all_bindings();
    void remove_all_key_bindings();
    void set_gamepad_binding(std::vector<int> key, Command* command);
    void set_key_and_mouse_binding(int key, int mouse, Command* command, bool isDrag);
    void set_pitch(float pitchOffset);
    void set_text_input_control();
    void set_yaw(float m_yaw);
    void set_text_input(const char* text);

    double get_xpos();
    double get_last_xpos();
    double get_ypos();
    double get_last_ypos();
    double get_yaw();
    double get_pitch();
    char* get_text_input();
    void clear_text_input();
    void clear_text_input_last_char();

    bool are_multiple_keys_pressed(int firstButton, int secondButton);
    bool is_key_pressed(int button);
    void show_cursor(bool show);
};
#endif // !INPUT_MANAGER_H
