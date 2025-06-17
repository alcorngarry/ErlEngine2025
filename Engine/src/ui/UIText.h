#ifndef UI_TEXT_H
#define UI_TEXT_H
#include<ft2build.h>
#include FT_FREETYPE_H
#include<iostream>
#include<map>
#include<glad/glad.h>
#include"stb_image/stb_image.h"
#include"renderer/Shader.h"

struct Character {
    unsigned int TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
    int pixelWidth;
};

namespace UIText {
    void init();
    int get_text_length(std::string text);
    int get_max_char_height();
    void draw(Shader* shader, glm::mat4 projection, std::string text, float x, float y, float scale, glm::vec3 color);
};
#endif // !UI_TEXT_H