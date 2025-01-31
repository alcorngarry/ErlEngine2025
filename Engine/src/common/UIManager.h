#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include<vector>
#include<glm/glm.hpp>
#include<glm/ext/matrix_clip_space.hpp>
#include"renderer/Shader.h"
#include"UIElement.h"
#include"AssetManager.h"

namespace UIManager {
    void init(int screenWidth, int screenHeight);
    void load_elements(unsigned int* cards, unsigned int card);
    void draw();
    void load_defaults();
    void set_screen_res(int width, int height);
    void center(std::vector<UIElement*> elements);
};
#endif // !UI_MANAGER_H