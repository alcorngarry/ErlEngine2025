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
    void load_elements(unsigned int* cards, unsigned int card);
    void draw();
    void load_defaults();
    void add_text_element(std::string text, float x, float y);
    TextElement* get_text_element(int index);
    void set_screen_res(int width, int height);
    void center(std::vector<UIElement*> elements);
};
#endif // !UI_MANAGER_H