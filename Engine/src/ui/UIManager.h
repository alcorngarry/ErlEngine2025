#ifndef UI_MANAGER_H
#define UI_MANAGER_H
#include<vector>
#include <set>
#include<glm/glm.hpp>
#include<glm/ext/matrix_clip_space.hpp>
#include"renderer/Shader.h"
#include"UIElement.h"
#include"common/AssetManager.h"
#include"UIText.h"
#include<input/InputManager.h>

class Map;

struct TextElement {
    std::string text;
    glm::vec2 position;
    float scale;
    glm::vec3 color;
};

class Window {
    public:
        Window(glm::vec2 pos, glm::vec2 size, std::string title);
        void draw();
        bool active = false;
    private:
        const float TEXT_SCALE = 0.4f;
        int textIndex = 0;
        glm::vec2 m_pos, m_size;
        std::string title;
        std::vector<UIElement*> m_uiElements;
        std::vector<TextElement*> m_textElements;
        void update_text_input();
};

namespace UIManager {
    void init(int screenWidth, int screenHeight);
    int add_text_element(std::string text);
    void load_elements();
    void load_elements(uint8_t* cards, uint8_t selectedCard);
    void load_map(Map* map);
    void toggle_draw_ui_element(std::string element);
    void toggle_window(std::string windowName);
    void draw();
    void center(UIElement* element);
    void remove_text_element(int index);
    TextElement* get_text_element(int index);
    void set_text_element_top(int index);
    void set_text_element_bottom(int index);
    void set_text_element_left(int index);
    void set_text_element_right(int index);
    void set_screen_res(int width, int height);
};
#endif // !UI_MANAGER_H