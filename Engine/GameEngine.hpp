#pragma once

#include <map>
#include <memory>

#include "SFML/Graphics/RenderWindow.hpp"
#include "../Scene/Scene.h"
#include "../Assets/Assets.hpp"
#include "../Scene/SceneMenu.h"


typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine {
protected:
    sf::RenderWindow m_window;
    Assets m_assets;
    std::string m_currentScene;
    SceneMap m_sceneMap;
    size_t m_simulationSpeed = 1;
    bool m_running = true;

    void init(const std::string& path) {
        m_assets.loadFromFile(path);
        m_window.create(sf::VideoMode(1280, 768), "Mega Woman 2D");
        m_window.setFramerateLimit(60);

        changeScene("MENU", std::make_shared<SceneMenu>(this));
    }

    void update() { currentScene()->update(); }

    void sUserInput() {
        sf::Event event{};

        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { quit(); }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::X) {
                    std::cout << "Save screenshot to " << "test.png" << std::endl;
                    sf::Texture texture;
                    texture.create(m_window.getSize().x, m_window.getSize().y);
                    texture.update(m_window);
                    if (texture.copyToImage().saveToFile("test.png")) {
                        std::cout << "Screenshot saved to " << "test.png" << std::endl;
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
                // if the current scene does not have an action associated with this key, skip the event
                if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end()) continue;
                

                // determine start or end action by whether it was key press or release
                const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";

                // look up the action and send the action to the scene
                currentScene()->doAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
            }

        }
    }

    std::shared_ptr<Scene> currentScene() { return m_sceneMap[m_currentScene]; }

public:
    explicit GameEngine(const std::string& path) { init(path); }

    void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false) {
        m_currentScene = sceneName;
        m_sceneMap[sceneName] = scene;
    }

    void quit() {
        m_running = false;
        m_window.close();
    }

    void run() {
        while (isRunning()) {
            sUserInput();
            update();
            m_window.display();
        }
    }

    sf::RenderWindow& window() { return m_window; }

    const Assets& assets() const { return m_assets; }

    bool isRunning() {
        return m_running && m_window.isOpen();
    }
};