#include "UIManager.h"
#include"common/Map.h"
#include <regex>

Shader* imageShader;
Shader* eguiShader;
Shader* textShader;

glm::mat4 m_projection;
std::map<std::string, UIElement*> uiElements;
std::vector<TextElement*> textElements;
std::set<std::string> uiDrawList;
std::map<std::string, Window*> windows;

int screenWidth, screenHeight;

Map* ui_map;

void UIManager::init(int screenWidth, int screenHeight)
{
    imageShader = new Shader("C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/ui_image.vert.glsl",
        "C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/ui_image.frag.glsl");
    eguiShader = new Shader("C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/egui.vert.glsl",
        "C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/egui.frag.glsl");
    textShader = new Shader("C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/text.vert.glsl",
        "C:/Dev/opengl_code/Erl/Erl/Engine/src/renderer/shaders/text.frag.glsl");
    uiElements.clear();
    set_screen_res(screenWidth, screenHeight);
    UIText::init();
}

void UIManager::set_screen_res(int width, int height) 
{
    screenWidth = width;
    screenHeight = height;
    m_projection = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);
}

void UIManager::load_elements(uint8_t* cards, uint8_t selectedCard)
{
}

void UIManager::load_map(Map* map)
{
    ui_map = map;
}

void UIManager::toggle_draw_ui_element(std::string element)
{
    if (uiDrawList.count(element) > 0)
    {
        uiDrawList.erase(element);
    }
    else {
        uiDrawList.insert(element);
    }
}

void UIManager::toggle_window(std::string windowName)
{
    windows[windowName]->active = !windows[windowName]->active;
}

void UIManager::load_elements()
{
   /* uiElements["crosshair"] = new UIElement(AssetManager::get_ui_element(0));
    center(uiElements["crosshair"]);
    uiDrawList.insert("crosshair");*/

    windows["console"] = new Window(glm::vec2(0.0f, 0.0f), glm::vec2(screenWidth, screenHeight / 3), "hello");
}

void UIManager::draw() 
{
    for (const auto& window : windows)
    {
        if(window.second->active) window.second->draw();
    }

    for (TextElement* text : textElements)
    {
        UIText::draw(textShader, m_projection, text->text, text->position.x, text->position.y, text->scale, text->color);
    }

    for (std::string element : uiDrawList)
    {
        uiElements[element]->draw(imageShader, m_projection);
    }
}

void UIManager::center(UIElement* element)
{
    element->set_size(64.0f, 64.0f);
    element->set_position(screenWidth/2.0f - 32.0f, screenHeight/2.0f - 32.0f);
}

int UIManager::add_text_element(std::string text)
{
    textElements.push_back(new TextElement{ text, glm::vec2(0, 0), 1.0f });
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

Window::Window(glm::vec2 pos, glm::vec2 size, std::string title) : m_pos(pos), m_size(size), title(title)
{
    UIElement* textBox = new UIElement(glm::vec4(.6f));
    textBox->set_size(size.x, size.y / 8);
    textBox->set_position(0, size.y - size.y / 8);
    m_uiElements.push_back(textBox);

    UIElement* background = new UIElement(glm::vec4(0.4f));
    background->set_size(size.x, size.y);
    background->set_position(pos.x, pos.y);
    m_uiElements.push_back(background);

    m_textElements.push_back(new TextElement{ "", glm::vec2(10.0f, size.y - 10.0f), TEXT_SCALE, glm::vec3(0.0f, 0.0f,0.0f) });

    
    
    InputManager::set_key_binding(GLFW_KEY_ENTER, [this](float dt) mutable {
        if (active)
        {
            std::string input = m_textElements[textIndex]->text;
            std::regex pattern(R"(s_(\w+)\s+(\d+))");
            std::smatch matches;

            if (std::regex_match(input, matches, pattern)) {
                std::string action = matches[1];
                int number = std::stoi(matches[2]);

                std::cout << "Action: " << action << "\n";
                std::cout << "Number: " << number << "\n";

               
                if (number < ui_map->entities.size())
                {
                    if (action == "clear")
                    {
                        ui_map->entities[number]->actions.clear();
                    }
                    else {
                        ui_map->entities[number]->actions[action] = Scripts::get_script(action);
                    }
                }
            }
            else {
                std::cout << "Input didn't match expected format.\n";
            }
            
            for (int i = 0; i <  m_textElements.size(); i++)
            {
                m_textElements[i]->position.y -= 50;
                if (i == m_textElements.size() - 1)
                {
                    m_textElements[i]->text = "> " + m_textElements[i]->text;
                }
            }
            m_textElements.push_back(new TextElement{ "", glm::vec2(10.0f, m_size.y - 10.0f), TEXT_SCALE, glm::vec3(0.0f, 0.0f, 0.0f)});
            textIndex++;
            InputManager::clear_text_input();
        }
    });

    InputManager::set_key_binding(GLFW_KEY_UP, [this](float dt) mutable {
        if (active)
        {
            if (m_textElements.size() > 1) InputManager::set_text_input(m_textElements[m_textElements.size() - 2]->text.substr(2, m_textElements[m_textElements.size() - 2]->text.size()).c_str());
        }
    });

    InputManager::set_key_binding(GLFW_KEY_BACKSPACE, [this](float dt) mutable {
        if (active)
        {
            InputManager::clear_text_input_last_char();
        }
    });
}

void Window::update_text_input()
{
    if (active)
    {
        if (InputManager::get_text_input())
        {
            m_textElements[textIndex]->text = InputManager::get_text_input();
        }
    }
}

void Window::draw()
{
    update_text_input();

    for (TextElement* text : m_textElements)
    {
        UIText::draw(textShader, m_projection, text->text, m_pos.x + text->position.x, m_pos.y + text->position.y, text->scale, text->color);
    }

    for (UIElement* element : m_uiElements)
    {
        element->draw(eguiShader, m_projection);
    }
}