#ifndef UI_MANAGER_H
#define UI_MANAGER_H
#include<vector>
#include<glm/glm.hpp>
#include<glm/ext/matrix_clip_space.hpp>
#include"renderer/Shader.h"
#include"UIElement.h"
#include"common/AssetManager.h"
#include"UIText.h"

struct TextElement {
    std::string text;
    glm::vec2 position;
};

namespace UIManager {
    void init(int screenWidth, int screenHeight);
    int add_text_element(std::string text);
    void load_elements();
    void load_elements(uint8_t* cards, uint8_t selectedCard);
    void draw();
    void load_defaults();
    void remove_text_element(int index);
    TextElement* get_text_element(int index);
    void set_text_element_top(int index);
    void set_text_element_bottom(int index);
    void set_text_element_left(int index);
    void set_text_element_right(int index);
    void set_screen_res(int width, int height);
    void center(std::vector<UIElement*> elements);
};
#endif // !UI_MANAGER_H