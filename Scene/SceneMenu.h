#pragma once

#include "Scene.h"
#include "ScenePlay.h"

class SceneMenu : public Scene {
private:
    std::string m_title;
    std::vector<std::string> m_menuStrings;

    sf::Text m_menuText;
    std::vector<sf::Text> m_menuItems;

    std::vector<std::string> m_levelPaths;
    size_t m_selectedMenuIndex = 0;

    void init();

    void update() override;

    void onEnd() override;

    void sDoAction(const Action& action) override;

public:
    explicit SceneMenu(GameEngine* gameEngine = nullptr);

    void sRender() override;
};