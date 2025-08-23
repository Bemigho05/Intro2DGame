#pragma once

#include <map>
#include <memory>

#include "../Engine/GameEngine.hpp"  // Must come first (full definition of GameEngine)
#include "ScenePlay.h"
#include "../ECS/EntityManager.hpp"
#include "SFML/Graphics/Text.hpp"

#include "Scene.h"



class ScenePlay : public Scene {
    struct PlayerConfig {
        float X, Y, CX, CY, SPEED, MAX_SPEED, JUMP, GRAVITY;
        std::string WEAPON;
    };

private:
    std::shared_ptr<Entity> m_player;
    std::string m_levelPath;
    PlayerConfig m_playerConfig;
    bool m_drawTextures = true;
    bool m_drawCollision = true;
    bool m_drawGrid = true;
    const Vec2f m_gridSize = { 64, 64 };
    sf::Text m_gridText;
    // Physics m_worldPhysics;

    void init(const std::string& levelPath);

    Vec2f gridToMidPixel(float, float, std::shared_ptr<Entity>);

    void loadLevel(const std::string& fileName);

    void spawnPlayer();

    void spawnBullet(std::shared_ptr<Entity> entity);

    void sMovement();
    void sLifespan();
    void sCollision();
    void sAnimation();
    void sRender() override;
 

    void sDoAction(const Action& action) override;

    void onEnd() override;
public:
    ScenePlay(GameEngine* gameEngine, const std::string& levelPath);

    void update() override;
};

