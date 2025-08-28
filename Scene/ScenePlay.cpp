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
    registerAction(sf::Keyboard::Space, "JUMP");
    registerAction(sf::Keyboard::E, "ATTACK");
    registerAction(sf::Keyboard::LShift, "DASH");
	registerAction(sf::Keyboard::Q, "HEAVY_ATTACK");

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


    for (int i = 0; i < 100; ++i) {
        auto block = m_entityManager.addEntity("tile");


        block->add<CTransform>(Vec2f(224 + static_cast<float>(i) * 32, 480));
        block->add<CShape>(32, 32, sf::Color::Magenta, sf::Color::White, 1);
        block->add<CBoundingBox>(Vec2f(32, 32));
    }



}

void ScenePlay::spawnPlayer() {
    
    m_player = m_entityManager.addEntity("player");
    m_player->add<CTransform>(Vec2f(224, 352));
    m_player->add<CBoundingBox>(m_game->assets().getPlayerAnimation("Stand").getSize());
    m_player->add<CAnimation>(m_game->assets().getPlayerAnimation("Stand"));
    m_player->add<CGravity>();
    m_player->add<CInput>();

}

void ScenePlay::spawnBullet(std::shared_ptr<Entity> entity) {
    
    
}

void ScenePlay::sMovement() {
	static float direction = 1.0f; // 1 = right, -1 = left
    auto& transform = m_player->get<CTransform>();
    auto& input = m_player->get<CInput>();
    auto& gravity = m_player->get<CGravity>();
	auto& animation = m_player->get<CAnimation>();

    transform.prevPos = transform.pos;

    float moveSpeed{};
    if (input.up && gravity.canJump && (gravity.gravity == 0.0f)) {
        m_player->get<CGravity>().canJump = false;
		gravity.gravity = -15.0f;
    }

	gravity.gravity += 0.5f;
	transform.pos.y += gravity.gravity;

    if (input.dashing) { 
        moveSpeed = 10.0f;
        transform.pos.x += moveSpeed * direction;
        transform.scale = { direction, 1.0f };
        return;
    }
    else if (input.heavyAttacking) { return; }
    else if (input.attacking) { return; }
	else moveSpeed = 5.0f;
	
    if (input.left && input.right) {}
    else if (input.left || input.right) {
		direction = input.left ? -1.0f : 1.0f;
        transform.pos.x += moveSpeed * direction;
        transform.scale = { direction, 1.0f };
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
    auto& input = m_player->get<CInput>();
    auto& player_animation = m_player->get<CAnimation>();
    auto& gravity = m_player->get<CGravity>();

    if (player_animation.mustFinish)  goto update;

    if (input.heavyAttack) {
		input.heavyAttacking = true;
        if (player_animation.getName() != "HeavyAttack") {
            player_animation = m_game->assets().getPlayerAnimation("HeavyAttack");
            player_animation.mustFinish = true;
            player_animation.repeat = false;
        }
    }
    else if (input.attack) {
        input.attacking = true;
        if (player_animation.getName() != "Attack") {
            player_animation = m_game->assets().getPlayerAnimation("Attack");
            player_animation.mustFinish = true;
            player_animation.repeat = false;
        }
    }
    else if (input.dash) {
        input.dashing = true;
        if ((gravity.gravity >= 0.0f) && (!gravity.canJump)) {
            if (player_animation.getName() != "DashAir") {
                player_animation = m_game->assets().getPlayerAnimation("DashAir");
                player_animation.mustFinish = false;
                player_animation.repeat = true;
			}
        }

        else if (player_animation.getName() != "Dash") {
            player_animation = m_game->assets().getPlayerAnimation("Dash");
            player_animation.mustFinish = true;
            player_animation.repeat = false;
        }
    }


    else
        if ((input.left || input.right) && (gravity.canJump) && (input.left != input.right)) {
            if (player_animation.getName() != "Run") {
                player_animation = m_game->assets().getPlayerAnimation("Run");
                player_animation.mustFinish = false;
				player_animation.repeat = true;
            }
        }
        else if ((gravity.gravity > -15.0f) && (gravity.gravity < 0.0f)) {
            if (player_animation.getName() != "Jump") {
                player_animation = m_game->assets().getPlayerAnimation("Jump");
                player_animation.mustFinish = false;
				player_animation.repeat = true;
            }
        }
        else if ((gravity.gravity >= 0.0f) && (!gravity.canJump)) {
            if (player_animation.getName() != "Fall") {
                player_animation = m_game->assets().getPlayerAnimation("Fall");
                player_animation.mustFinish = false;
                player_animation.repeat = true;
            }
        }
        else {
            if (player_animation.getName() != "Stand") {
                player_animation = m_game->assets().getPlayerAnimation("Stand");
                player_animation.mustFinish = false;
                player_animation.repeat = true;

            }
        }

update:

    if (!(player_animation.hasEnded())) {
        player_animation.update();
    }
    else if ((player_animation.repeat)) {
        player_animation.update();
    }

    
    else {
        auto& name = player_animation.getName();
        if (name == "HeavyAttack") 
            input.heavyAttacking = false;
        else if (name == "Dash") 
            input.dashing = false;
        else if (name == "Attack") 
            input.attacking = false;
        player_animation = m_game->assets().getPlayerAnimation("Stand");
        player_animation.mustFinish = false;
        player_animation.repeat = true;
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

    if (m_drawTextures) {
        for (const auto& e : m_entityManager.getEntities()) {
            auto& transform = e->get<CTransform>();
            if (e->has<CAnimation>()) {
                auto& animation = e->get<CAnimation>();
     
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
        else if (action.name() == "DASH") { m_player->get<CInput>().dash = true; }
        else if (action.name() == "HEAVY_ATTACK") { m_player->get<CInput>().heavyAttack = true; }
    }


    else if (action.type() == "END") {
        if (action.name() == "JUMP") { m_player->get<CInput>().up = false; }
        else if (action.name() == "LEFT") { m_player->get<CInput>().left = false; }
        else if (action.name() == "RIGHT") { m_player->get<CInput>().right = false; }
        else if (action.name() == "ATTACK") { m_player->get<CInput>().attack = false; }
        else if (action.name() == "DASH") { m_player->get<CInput>().dash = false; }
		else if (action.name() == "HEAVY_ATTACK") { m_player->get<CInput>().heavyAttack = false; }
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

