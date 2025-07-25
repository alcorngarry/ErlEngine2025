#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H
#include<glad/glad.h>
#include"renderer/Shader.h"

class UIElement {
public:
    UIElement(unsigned int textureID);
    UIElement(glm::vec4 color);

    void draw(Shader* shader, glm::mat4 projection);
    void set_size(float width, float height);
    void set_position(float x, float y);
    bool is_selected();
    void toggle_select();

private:
    float m_x, m_y, m_width, m_height;
    unsigned int textureID;
    glm::vec4 color;
    bool isSelected = false;
};
#endif // !UI_ELEMENT_H