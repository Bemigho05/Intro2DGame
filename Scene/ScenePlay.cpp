#include "ScenePlay.h"
#include "../ECS/EntityManager.hpp"
#include "../Physics/Physics.hpp"
#include "Scene.h"


void ScenePlay::init(const std::string& levelPath) {
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");   // Toggle drawing (T)extures
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION"); // Toggle drawing (C)ollision Boxes
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");      // Toggle drawing (G)rid
    registerAction(sf::Keyboard::A, "LEFT");
    registerAction(sf::Keyboard::D, "RIGHT");
    registerAction(sf::Keyboard::W, "JUMP");
    registerAction(sf::Keyboard::Space, "SHOOT");

    // TODO: Register all other gameplay Actions
    // registerAction(sf::Keyboard::W, "JUMP");

    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("Tech"));
    // m_gridText.setFont(m_game->assets().getFont("Tech"));

    loadLevel(levelPath);
}

Vec2f ScenePlay::gridToMidPixel(float x, float y, std::shared_ptr<Entity>) {
    // TODO: This function takes in a grid (x,y) position and an Entity
    //       Return a Vec2f indicating where the CENTER position of the Entity should be
    //       You must use the Entity's Animation size to position it correctly
    //       The size of the grid width and height is stored in m_gridSize.x and m_gridSize.y
    //       The bottom-left corner of the Animation should aligh with the bottom left of the grid cell

    return Vec2f(0, 0);
}

void ScenePlay::loadLevel(const std::string& fileName) {
    // reset the entity manager every time we load a level
    m_entityManager = EntityManager();

    // TODO: read in the level file and add the appropriate entities
    //       use the PlayerConfig struct m_playerConfig to store player properties
    //       this struct is defined at the top of Scene_Play.h

    // NOTE: all the code below is sample code which shows you how to
    //       set up and use entities with the new syntax, it should be removed

    spawnPlayer();

    //// some sample entities
    //auto brick = m_entityManager.addEntity("tile");
    //// IMPORTANT: always add the CAnimation component first so that gridToMidPixel can compute correctly
    //brick->add<CAnimation>(m_game->assets().getAnimation("Brick"), true);
    //brick->add<CTransform>(Vec2f(96, 480));
    //// NOTE: Your final code should position the entity with the grid x,y position read from the file:
    //// brick->add<CTransform>(gridToMidPixel(gridX, gridY, brick));

    //if (brick->get<CAnimation>().animation.getName() == "Brick") {
    //    std::cout << "This could be a good way of identifying if a tile is a brick!\n";
    //}

    for (int i = 0; i < 50; ++i) {
        auto block = m_entityManager.addEntity("tile");


        block->add<CTransform>(Vec2f(224 + i * 32, 480));
        block->add<CShape>(32, 32, sf::Color::Magenta, sf::Color::White, 1);
        block->add<CBoundingBox>(Vec2f(32, 32));
    }

    for (int i = 0; i < 20; ++i) {
        auto block = m_entityManager.addEntity("tile");
        block->add<CTransform>(Vec2f(64 + 224 + i * 32, 480 - i * 32 * 2));
        block->add<CShape>(32, 32, sf::Color::Magenta, sf::Color::White, 1);
        block->add<CBoundingBox>(Vec2f(32, 32));
    }

    

    //auto question = m_entityManager.addEntity("tile");
    //question->add<CAnimation>(m_game->assets().getAnimation("Ground"), true);
    //question->add<CTransform>(Vec2f(352, 480));

    // NOTE: THIS IS INCREDIBLY IMPORTANT PLEASE READ THIS EXAMPLE
    //       Components are now returned as references rather than pointers
    //       If you do not specify a reference variable type, it will COPY the component
    //       Here is an example:
    //
    //       This will COPY the transform into the variable 'transform1' - it is INCORRECT
    //       Any changes you make to transform1 will not be changed inside the entity
    //       auto transform1 = entity->get<CTransform>()
    //
    //       This will REFERENCE the transform with the variable 'transform2' - it is CORRECT
    //       Now any changes you make to transform2 will be changed inside the entity
    //       auto& transform2 = entity->get<CTransform>()
}

void ScenePlay::spawnPlayer() {
    // here is a sample player entity which you can use to construct other entities
    m_player = m_entityManager.addEntity("player");
    m_player->add<CState>("Spawn");
    m_player->add<CTransform>(Vec2f(224, 352));
    m_player->add<CBoundingBox>(m_game->assets().getAnimation("Stand").getSize());
    m_player->add<CAnimation>(m_game->assets().getAnimation(m_player->get<CState>().state), false);
    m_player->add<CGravity>();
    m_player->add<CInput>();

    // TODO: be sure to add the remaining components to the player
}

void ScenePlay::spawnBullet(std::shared_ptr<Entity> entity) {
    auto&& bullet = m_entityManager.addEntity("bullet");
    bullet->add<CTransform>();
    bullet->add<CBoundingBox>();
    
}

void ScenePlay::sMovement() {
    auto& transform = m_player->get<CTransform>();
    auto& input = m_player->get<CInput>();
    auto& state = m_player->get<CState>();
    auto& gravity = m_player->get<CGravity>();
    auto& animation = m_player->get<CAnimation>();
    auto& assets = m_game->assets();

    // Jumping
    if (input.up && gravity.gravity == 0) {
        gravity.gravity = -15.0f;
        if (state.state != "AIR") {
            state.state = "AIR";
            animation.animation = assets.getAnimation("Air");
        }

    }

    // State management
    if (gravity.gravity == 0) {
        if (transform.pos.x == transform.prevPos.x) { // Check if it isn't falling
            if (state.state != "STAND") {
                state.state = "STAND";
                animation.animation = assets.getAnimation("Stand");
            }

        }
    }

    // Apply gravity
    gravity.gravity = gravity.gravity + 0.5f;
    transform.prevPos = transform.pos;
    transform.pos.y += gravity.gravity;

    // Horizontal movement
    float moveSpeed = 5.0f;
    if (input.left && !input.right) { // left only
        transform.pos.x -= moveSpeed;
        transform.scale = { -1, 1 };
        if (state.state != "AIR")
            if (state.state != "LEFT") {
                state.state = "LEFT";
                animation.animation = assets.getAnimation("Run");
            }

            
    }
    else if (input.right && !input.left) { // right only
        transform.pos.x += moveSpeed;
        transform.scale = { 1, 1 };
        if (state.state != "AIR")
            if (state.state != "RIGHT") {
                state.state = "RIGHT";
                animation.animation = assets.getAnimation("Run");
            }
    }

    else if (!input.right && !input.left && !input.up) {
        if (state.state != "AIR")
            if (state.state != "STAND") {
                state.state = "STAND";
                animation.animation = assets.getAnimation("Stand");
            }
    }
    else {
            if (state.state != "STAND")
            if (state.state != "AIR") {
                state.state = "AIR";
                animation.animation = assets.getAnimation("Air");
            }
        
    }

    
}


void ScenePlay::sLifespan() {
    
}

void ScenePlay::sCollision() {
    for (auto& e : m_entityManager.getEntities("tile")) {
        auto overlap = Physics::GetOVerlap(m_player, e);
        if (overlap.x > 0 && overlap.y > 0) {
            auto prevOverlap = Physics::GetPreviousOverlap(m_player, e);

            // Vertical collision
            if (prevOverlap.x > 0) {
                if (m_player->get<CTransform>().pos.y > e->get<CTransform>().pos.y) {
                    // Player is below tile
                    m_player->get<CTransform>().pos.y += overlap.y;
                }
                else {
                    // Player is above tile (landing)
                    m_player->get<CTransform>().pos.y -= overlap.y;
                    m_player->get<CGravity>().gravity = 0;
                    if (m_player->get<CState>().state == "AIR") {
                        m_player->get<CState>().state = "STAND";
                    }
                }
            }

            // Horizontal collision
            if (prevOverlap.y > 0) {
                if (m_player->get<CTransform>().pos.x > e->get<CTransform>().pos.x) {
                    m_player->get<CTransform>().pos.x += overlap.x;
                }
                else {
                    m_player->get<CTransform>().pos.x -= overlap.x;
                }
            }
        }
    }
}

void ScenePlay::sAnimation() 
{
    
    auto& player_anim = m_player->get<CAnimation>();
    auto& player_state = m_player->get<CState>().state;
    if ((player_anim.repeat)) {
        player_anim.animation.update();
    }
       
    else if ((player_anim.animation.hasEnded())) {
        m_player->add<CState>("STAND");
        player_anim.animation = m_game->assets().getAnimation("Stand");
        player_anim.repeat = true;
    }
    else if (!(player_anim.animation.hasEnded())) {
        player_anim.animation.update();
    }
}

void ScenePlay::sRender() {
    // color the background darker, so you know that the game is paused
    if (!m_paused) {
        m_game->window().clear(sf::Color(100, 100, 255));
    }
    else {
        m_game->window().clear(sf::Color(50, 50, 150));
    }

    // set the viewport of the window to be centered on the player if it's far enough right
    auto& pPos = m_player->get<CTransform>().pos;
    float windowCenterX = std::max(m_game->window().getSize().x / 2.0f, pPos.x);
    sf::View view = m_game->window().getView();
    view.setCenter(windowCenterX, m_game->window().getSize().y - view.getCenter().y);
    m_game->window().setView(view);

    // draw all Entity textures / animations
    if (m_drawTextures) {
        for (const auto& e : m_entityManager.getEntities()) {
            auto& transform = e->get<CTransform>();
            if (e->has<CAnimation>()) {
                auto& animation = e->get<CAnimation>().animation;
                animation.getSprite().setRotation(transform.angle);
                animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
                animation.getSprite().setScale(transform.scale.x, transform.scale.y);
                m_game->window().draw(animation.getSprite());
            }

            if (e->has<CShape>()) {
                e->get<CShape>().rect.setPosition(e->get<CTransform>().pos);
                m_game->window().draw(e->get<CShape>().rect);
            }
        }
    }

    // draw all Entity collision bounding boxes with a rectangle shape
    if (m_drawCollision) {
        for (const auto& e : m_entityManager.getEntities()) {
            if (e->has<CBoundingBox>()) {
                auto& box = e->get<CBoundingBox>();
                auto& transform = e->get<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition(transform.pos.x, transform.pos.y);
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                rect.setOutlineColor(sf::Color::White);
                rect.setOutlineThickness(1);
                m_game->window().draw(rect);
            }
        }
    }

    // draw the grid so that can easily debug
    if (m_drawGrid) {
        float leftX = m_game->window().getView().getCenter().x - width() / 2.0;
        float rightX = leftX + width() + m_gridSize.x;
        float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);

        for (float x = nextGridX; x < rightX; x += m_gridSize.x) {
            drawLine(Vec2f(x, 0), Vec2f(x, height()));
        }

        for (float y = 0; y < height(); y += m_gridSize.y) {
            drawLine(Vec2f(leftX, height() - y), Vec2f(rightX, height() - y));

            for (float x = nextGridX; x < rightX; x += m_gridSize.x) {
                std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
                std::string yCell = std::to_string((int)y / (int)m_gridSize.y);
                m_gridText.setString("(" + xCell + "," + yCell + ")");
                m_gridText.setPosition(x + 3, height() - y - m_gridSize.y + 2);
                m_game->window().draw(m_gridText);
            }
        }
    }
}

void ScenePlay::sDoAction(const Action& action) {
    if (action.type() == "START") {
        if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
        else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }
        else if (action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; }
        else if (action.name() == "PAUSE") { setPaused(!m_paused); }
        else if (action.name() == "QUIT") { onEnd(); }
        else if (action.name() == "JUMP") {
            if (!m_player->get<CGravity>().gravity)
            m_player->get<CInput>().up = true;
            //if (action.type() == "START" && m_player->get<CGravity>().gravity == 0) {
            //    m_player->get<CGravity>().gravity = -15.0f;
            //    m_player->get<CState>().state = "AIR";
            //    m_player->get<CAnimation>().animation = m_game->assets().getAnimation("Air");
            //    m_player->get<CAnimation>().repeat = false;
            //}
        }
        else if (action.name() == "LEFT") {

            m_player->get<CInput>().left = true;

        }
        else if (action.name() == "RIGHT") {
            m_player->get<CInput>().right = true;

        }
        else if (action.name() == "SHOOT") {
            m_player->get<CInput>().shoot = true;
        }
    }
    else if (action.type() == "END") {
        if (action.name() == "JUMP") {
            // m_player jump
            m_player->get<CInput>().up = false;
        }
        else if (action.name() == "LEFT") {
            m_player->get<CInput>().left = false;

        }
        else if (action.name() == "RIGHT") {
            m_player->get<CInput>().right = false;
            
        }
        else if (action.name() == "SHOOT") {
            m_player->get<CInput>().shoot = true;
        }
    }
}

void ScenePlay::onEnd() {
    // TODO:
}

//    void changePlayerStateTo(PlayerState s);
//    void spawnCoinSpin(std::shared_ptr<Entity> tile);
//    void spawnBrickDebris(std::shared_ptr<Entity> tile);


ScenePlay::ScenePlay(GameEngine* gameEngine, const std::string& levelPath) : Scene(gameEngine), m_levelPath(levelPath) { init(levelPath); }

void ScenePlay::update() {
    m_entityManager.update();

    // TODO: implement pause functionality
    sMovement();
    sLifespan();
    sCollision();
    sAnimation();
    sRender();

    m_currentFrame++;
}

