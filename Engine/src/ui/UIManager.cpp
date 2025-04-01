#include "UIManager.h"

Shader* shader;
Shader* text_shader;

glm::mat4 m_projection;
std::vector<UIElement*> uiElements;
std::vector<TextElement*> textElements;

int screenWidth, screenHeight;

void UIManager::init(int screenWidth, int screenHeight)
{
    shader = new Shader("C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/interface.vert.glsl", "C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/interface.frag.glsl");
    text_shader = new Shader("C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/text.vert.glsl", "C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/text.frag.glsl");
    uiElements.clear();
    set_screen_res(screenWidth, screenHeight);
    load_defaults();
    UIText::init();
}

void UIManager::load_defaults() 
{
    center(uiElements);
}

void UIManager::set_screen_res(int width, int height) 
{
    screenWidth = width;
    screenHeight = height;
    m_projection = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);
}

//will probably need to be decoupled.
void UIManager::load_elements(uint8_t* cards, uint8_t selectedCard)
{
    int size = 0;
    uiElements.clear();

    if (selectedCard == 6)
    {
        size = 1;
    }
    else {
        size = 5;
    }
    for (int i = 0; i < size; i++)
    {
        uiElements.push_back(new UIElement(AssetManager::get_card(cards[i] - 1)));
        if (i == selectedCard)
        {
            uiElements.back()->toggle_select();
        }
    }

    center(uiElements);
}

void UIManager::load_elements()
{
    uiElements.push_back(new UIElement(AssetManager::get_ui_element(0)));
    center(uiElements);
}

void UIManager::draw() 
{
    for (UIElement* element : uiElements)
    {
        element->draw(shader, m_projection);
    }
   
   /* for (TextElement* text : textElements)
    {
        UIText::draw(text_shader, m_projection, text->text, text->position.x, text->position.y);
    }*/
}

void UIManager::center(std::vector<UIElement*> elements)
{
    for (int i = 0; i < elements.size(); i++)
    {
        elements[i]->set_size(64.0f, 64.0f);
        elements[i]->set_position(screenWidth/2.0f - 32.0f, screenHeight/2.0f - 32.0f);
    }
}

int UIManager::add_text_element(std::string text)
{
    textElements.push_back(new TextElement{ text, glm::vec2(0, 0) });
    return textElements.size() - 1;
}

void UIManager::remove_text_element(int index)
{
    textElements.erase(textElements.begin() + index);
}

TextElement* UIManager::get_text_element(int index)
{
    return textElements.at(index);
}

void UIManager::set_text_element_top(int index)
{
    textElements[index]->position.y = 0 + UIText::get_max_char_height();
}

void UIManager::set_text_element_bottom(int index)
{
    textElements[index]->position.y = screenHeight - UIText::get_max_char_height();

}

void UIManager::set_text_element_left(int index)
{
    textElements[index]->position.x = 0;
}

void UIManager::set_text_element_right(int index)
{
    textElements[index]->position.x = screenWidth - UIText::get_text_length(textElements[index]->text);
}