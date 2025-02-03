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
void UIManager::load_elements(unsigned int* cards, unsigned int selectedCard)
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

void UIManager::add_text_element(std::string text, float x, float y)
{
    textElements.push_back(new TextElement{ text, glm::vec2(x, y)});
}


TextElement* UIManager::get_text_element(int index)
{
    return textElements.at(index);
}

void UIManager::draw() 
{
    for (UIElement* element : uiElements)
    {
        element->draw(shader, m_projection);
    }
   
    for (TextElement* text : textElements)
    {
        UIText::draw(text_shader, m_projection, text->text, text->position.x, text->position.y);
    }
}

void UIManager::center(std::vector<UIElement*> elements)
{
    float normalizedWidth = 142.0f / (float)screenWidth;
    float normalizedHeight = 272.0f / (float)screenHeight;
    float buffer = (screenWidth - elements.size() * 142.0f ) / 2;

    for (int i = 0; i < elements.size(); i++)
    {
        float normalizedX = i * normalizedWidth;
        // float normalizedY = 0.0f;
        //fix lol
        /*if(elements[i]->is_selected())
        {
            elements[i]->set_size(normalizedWidth * (float)screenWidth * 2.0f, normalizedHeight * (float)screenHeight * 2.0f);
            elements[i]->set_position(normalizedX * 2.0f * (float)screenWidth + buffer, (float)screenHeight - (272.0f * 2.0f )- 50.0f);
        } else {*/
            elements[i]->set_size(normalizedWidth * (float)screenWidth, normalizedHeight * (float)screenHeight);
            elements[i]->set_position(normalizedX * (float)screenWidth + buffer, (float)screenHeight - 272.0f - 50.0f);
        //}
        
    }
}