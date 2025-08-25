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



    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("Tech"));

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

    spawnPlayer();


    for (int i = 0; i < 50; ++i) {
        auto block = m_entityManager.addEntity("tile");


        block->add<CTransform>(Vec2f(224 + static_cast<float>(i) * 32, 480));
        block->add<CShape>(32, 32, sf::Color::Magenta, sf::Color::White, 1);
        block->add<CBoundingBox>(Vec2f(32, 32));
    }



}

void ScenePlay::spawnPlayer() {
    
    m_player = m_entityManager.addEntity("player");
    m_player->add<CTransform>(Vec2f(224, 352));
    m_player->add<CBoundingBox>(m_game->assets().getAnimation("Stand").getSize());
    m_player->add<CAnimation>(m_game->assets().getAnimation("Stand"), false);
    m_player->add<CGravity>();
    m_player->add<CInput>();

}

void ScenePlay::spawnBullet(std::shared_ptr<Entity> entity) {
    
    
}

void ScenePlay::sMovement() {
    auto& transform = m_player->get<CTransform>();
    auto& input = m_player->get<CInput>();
    auto& gravity = m_player->get<CGravity>();

    transform.prevPos = transform.pos;

    float moveSpeed = 5.0f;
    if (input.up && gravity.canJump) {
        m_player->get<CGravity>().canJump = false;
		gravity.gravity = -15.0f;
    }

	gravity.gravity += 0.5f;
	transform.pos.y += gravity.gravity;
	
    if (input.left) {
        transform.pos.x -= moveSpeed;
        transform.scale = { -1, 1 };
    }
    if (input.right) {
        transform.pos.x += moveSpeed;
        transform.scale = { 1, 1 };
    }
}


void ScenePlay::sLifespan() {
    
}

void ScenePlay::sCollision() {
    for (auto& e : m_entityManager.getEntities("tile")) {
        auto overlap = Physics::GetOVerlap(m_player, e);
        if (overlap.x > 0 && overlap.y > 0) {
            auto prevOverlap = Physics::GetPreviousOverlap(m_player, e);

            if (prevOverlap.x > 0) {
                if (m_player->get<CTransform>().pos.y > e->get<CTransform>().pos.y) {
                    m_player->get<CTransform>().pos.y += overlap.y;
                }
                else {
                    m_player->get<CTransform>().pos.y -= overlap.y;
                    m_player->get<CGravity>().gravity = 0;
					m_player->get<CGravity>().canJump = true;
                }
            }

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
 
    if ((player_anim.repeat)) {
        player_anim.animation.update();
    }
       
    else if ((player_anim.animation.hasEnded())) {
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

        else if (action.name() == "JUMP") { m_player->get<CInput>().up = true; }
        else if (action.name() == "LEFT") { m_player->get<CInput>().left = true; }
        else if (action.name() == "RIGHT") { m_player->get<CInput>().right = true; }
        else if (action.name() == "ATTACK") { m_player->get<CInput>().attack = true; }
    }


    else if (action.type() == "END") {
        if (action.name() == "JUMP") { m_player->get<CInput>().up = false; }
        else if (action.name() == "LEFT") { m_player->get<CInput>().left = false; }
        else if (action.name() == "RIGHT") { m_player->get<CInput>().right = false; }
        else if (action.name() == "ATTACK") { m_player->get<CInput>().attack = false; }
    }
}

void ScenePlay::onEnd() {}

//    void changePlayerStateTo(PlayerState s);
//    void spawnCoinSpin(std::shared_ptr<Entity> tile);
//    void spawnBrickDebris(std::shared_ptr<Entity> tile);


ScenePlay::ScenePlay(GameEngine* gameEngine, const std::string& levelPath) : Scene(gameEngine), m_levelPath(levelPath) { init(levelPath); }

void ScenePlay::update() {
    m_entityManager.update();

    sMovement();
    sLifespan();
    sCollision();
    sAnimation();
    sRender();

    m_currentFrame++;
}

