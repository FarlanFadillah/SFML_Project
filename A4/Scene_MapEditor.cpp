#include "Scene_MapEditor.hpp"



SceneMapEditor::SceneMapEditor(GameEngine* gameEngine, const std::string& levelPath)
	:Scene(gameEngine)
{
	init(levelPath);
}
void SceneMapEditor::init(const std::string& levelPath)
{
    std::cout << "MAP EDITOR\n";
	m_actionMap.clear();
    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::Up, "UP");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::D, "RIGHT");
    registerAction(sf::Keyboard::Right, "RIGHT");
    registerAction(sf::Keyboard::A, "LEFT");
    registerAction(sf::Keyboard::Left, "LEFT");
    registerAction(sf::Keyboard::Escape, "EXIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");   // Toggle drawing textures
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION"); // Toggle drawing collision boxes
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");      // Toggle drawing Grid

    registerAction(sf::Keyboard::F1, "SAVE_MAP");
    registerAction(sf::Keyboard::R, "RESET_TO_ZERO_POINTS");
    registerAction(sf::Keyboard::Tab, "SHOW_BUTTON");

    auto& window = m_game->window();
    CameraView.setSize(window.getSize().x, window.getSize().y);
    CameraView.setCenter(window.getSize().x / 2, window.getSize().y / 2);
    window.setView(CameraView);

    spawnCamera(Vec2(window.getSize().x / 2, window.getSize().y / 2));


    //set View Rect
    m_rect = sf::IntRect((CameraView.getCenter().x - (m_game->m_windowConfig.W / 2)) - m_gridSize.x,
        (CameraView.getCenter().y - (m_game->m_windowConfig.H / 2)) - m_gridSize.y,
        (CameraView.getSize().x) + m_gridSize.x * 2,
        (CameraView.getSize().y) + m_gridSize.y * 2);

    createTileButton();
    
}

void SceneMapEditor::buttonHover(Vec2& pos)
{
    for (auto& e : m_entityManager.getEntities("Button"))
    {
        auto& eButton = e->getComponent<CButton>();
        eButton.mouseOver = isInside(pos, e);
    }
}

void SceneMapEditor::spawnTile(std::string tile)
{
    auto entity = m_entityManager.addEntity("Tile");
    entity->addComponent<CDraggAble>();
    entity->getComponent<CDraggAble>().dragging = true;
    entity->addComponent<CTransform>(mPos, // POSITION
        Vec2(0.0f, 0.0f), // VELOCITY
        Vec2(4.0f, 4.0f), // SCALE  scale set to 4x16, 4x16
        0);
        
    entity->addComponent<CAnimation>(
        m_game->assets().getAnimation(tile),
        true
    );

    m_dragging = true;
}

void SceneMapEditor::createTileButton()
{
    int col = 23, row = 5, i = 1;
    int maxCol = col + 15;
    int maxRow = row + 15;
    while (col < maxCol && row < maxRow)
    {
        auto entity = m_entityManager.addEntity("Button");


        entity->addComponent<CAnimation>(
            m_game->assets().getAnimation(std::to_string(i)),
            true
        );
        auto& size = entity->getComponent<CAnimation>().animation.getSize();
        entity->addComponent<CBoundingBox>(Vec2(size.x*2, size.y*2));

        entity->addComponent<CTransform>(Vec2(col, row), // GRID
            Vec2(0.0f, 0.0f), // VELOCITY
            Vec2(2.0f, 2.0f), 
            Vec2(size.x*2, size.y*2)); // SCALE  scale set to 4x16, 4x16 
        auto tile = std::to_string(i);
        entity->addComponent<CButton>(Action("SPAWN_TILE", "START", tile));
        col++;
        i++;

        if (col >= maxCol)
        {
            col = 23;
            row++;
        }
    }
    
}

void SceneMapEditor::onEnd()
{
   // m_game->changeScene("MENU", std::make_shared<SceneMenu>(m_game));
}

void SceneMapEditor::update()
{
    if (!m_paused)
    {
        sMovement();
        sCamera();
    }

    m_entityManager.update();
    sRender();
    currentScenePlayFrame++;
}

void SceneMapEditor::sRender()
{
    m_stats.clear();
    auto& window = m_game->window();

    if (!m_paused)
    {
        // color the background darker when paused
        window.clear(sf::Color(195, 195, 195));
    }
    else
    {
        window.clear(sf::Color(100, 100, 255));
    }

    //draw all tiles
    if (m_drawTextures)
    {
        for (auto e : m_entityManager.getEntities("Tile"))
        {
            auto& pos = e->getComponent<CTransform>().pos;
            auto& scale = e->getComponent<CTransform>().scale;
            auto& sprite = e->getComponent<CAnimation>().animation.getSprite();


            sprite.setPosition(pos.x, pos.y);
            sprite.setScale(scale.x, scale.y);

            if (!m_rect.contains(pos.x, pos.y)) continue;
            window.draw(sprite);
        }
    }

    //draw tile drag
    if (m_drawTextures)
    {
        for (auto e : m_entityManager.getEntities("Tile")) 
        {
            if (e->hasComponent<CDraggAble>() && e->getComponent<CDraggAble>().dragging)
            {
                auto& pos = e->getComponent<CTransform>().pos;
                auto& scale = e->getComponent<CTransform>().scale;
                auto& sprite = e->getComponent<CAnimation>().animation.getSprite();

                pos = mPos;

                sprite.setPosition(pos.x, pos.y); 
                sprite.setScale(scale.x, scale.y); 

                if (!m_rect.contains(pos.x, pos.y)) continue; 
                window.draw(sprite); 
                break;
            }
            
        }
    }

    // Set the viewport of the window to be centered on the player if it's far enough right
    if (m_drawGrid)
    {
        sf::IntRect rect(CameraView.getCenter().x - window.getSize().x / 2,
            CameraView.getCenter().y - window.getSize().y / 2,
            window.getSize().x,
            window.getSize().y);

        sf::RectangleShape rectS;
        rectS.setSize(sf::Vector2f(64, 64));
        rectS.setFillColor(sf::Color(0, 0, 0, 0));
        rectS.setOutlineColor(sf::Color(255, 255, 255, 255));
        rectS.setOutlineThickness(0.5f);

        sf::Text text;
        text.setFont(m_game->assets().getFont("Ostrich_Regular"));
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);

        int col = -1, row = -1;
        int maxCol = rect.width / 64, maxRow = rect.height / 64;
        while (col <= maxCol && row <= maxRow)
        {
            int x = (col * 64) + rect.left - (rect.left % 64);
            int y = (row * 64) + rect.top - (rect.top % 64);

            text.setString(std::to_string(x / 64) + " : " + std::to_string(y / 64));
            text.setPosition(x, y);
            
            rectS.setPosition(x, y);
            
            window.draw(rectS);
            window.draw(text);

            col++;
            if (col >= maxCol + 1)
            {
                col = -1;
                row++;
            }
        }

    }

    if (true)
    {
        auto& cameraPos = m_camera->getComponent<CTransform>().pos;
        sf::CircleShape circle; 

        circle.setRadius(2);
        circle.setOrigin(15 / 2, 15 / 2);
        circle.setFillColor(sf::Color::Blue);
        circle.setPosition(cameraPos.x, cameraPos.y); 

        window.draw(circle); 
    }

    if (m_showButton)
    {
        sf::RectangleShape rect;
        for (auto& e : m_entityManager.getEntities("Button"))
        {
            auto& button = e->getComponent<CButton>();
            float scale = button.clicked ? 0.8f : 1.0f;

            auto& bcTransform = e->getComponent<CTransform>();
            auto& sprite = e->getComponent<CAnimation>().animation.getSprite();
            auto& size = e->getComponent<CAnimation>().animation.getSize(); 

            auto wPos = windowToWorldPos(bcTransform.pos);
            

            sprite.setPosition(wPos.x, wPos.y);
            sprite.setScale(bcTransform.scale.x * scale, bcTransform.scale.y * scale);
            window.draw(sprite);

            if (button.mouseOver)
            {
                rect.setSize(sf::Vector2f(size.x * bcTransform.scale.x, size.y * bcTransform.scale.y));
                rect.setFillColor(sf::Color(255, 255, 255, 100));
                rect.setOrigin(rect.getSize().x / 2, rect.getSize().y / 2);
                rect.setPosition(wPos.x, wPos.y);
                window.draw(rect);
            }
        }
    }

    // Draw Stats
    if (m_drawStats)
    {

        std::string mStat = m_mouseOnClick ? "True" : "False";
        m_stats.push_back("Mouse On Click \t:  " + mStat);
        int row = 0;

        for (auto e : m_stats)
        {
            m_text.setCharacterSize(30);
            m_text.setFont(m_game->assets().getFont("Ostrich_Regular"));
            m_text.setString(e);
            m_text.setFillColor(sf::Color::Black);
            m_text.setOutlineThickness(1.5f);
            //m_text.setScale(m_zoomScale, m_zoomScale);

            m_text.setPosition(CameraView.getCenter().x - CameraView.getSize().x / 2 + 5,
                (CameraView.getCenter().y - CameraView.getSize().y / 2) + (row * (m_text.getGlobalBounds().height + 5)));
            window.draw(m_text);
            row++;
        }
    }
    window.display();
}

void SceneMapEditor::sDoAction(const Action& action)
{
    if (action.type() == "START")
    {
        if (action.name() == "LEFT")
        {
            m_camera->getComponent<CInput>().left = true;
        }
        else if (action.name() == "RIGHT")
        {
            m_camera->getComponent<CInput>().right = true;
        }
        else if (action.name() == "UP")
        {
            m_camera->getComponent<CInput>().up = true;
        }
        else if (action.name() == "DOWN")
        {
            m_camera->getComponent<CInput>().down = true;
        }
        else if (action.name() == "EXIT")
        {
            onEnd();
        }
        else if (action.name() == "PAUSE")
        {
            setPaused(m_paused);
        }
        else if (action.name() == "TOGGLE_GRID")
        {
            m_drawGrid = !m_drawGrid;
        }
        else if (action.name() == "TOGGLE_TEXTURE")
        {
            m_drawTextures = !m_drawTextures;
        }
        else if (action.name() == "TOGGLE_COLLISION")
        {
            m_drawCollision = !m_drawCollision;
        }
        else if (action.name() == "RESET_TO_ZERO_POINTS")
        {
            auto& window = m_game->window();
            m_camera->getComponent<CTransform>().pos = Vec2(window.getSize().x/2, window.getSize().y/2);
        }
        else if (action.name() == "SAVE_MAP")
        {
            saveLevel();
        }
        else if (action.name() == "LEFT_CLICK")
        {
            auto pos = windowToWorldPos(action.pos());
            m_mouseOnClick = true;
            if(buttonPressed(pos)) return;

            auto wPos = windowToWorldPos(action.pos());
            for (auto e : m_entityManager.getEntities("Tile"))
            {
                if (m_dragging && e->getComponent<CDraggAble>().dragging)
                {
                    auto& spriteSize = e->getComponent<CAnimation>().animation.getSize();
                    auto& scale = e->getComponent<CTransform>().scale;
                    std::cout << "s\n";
                    Vec2 size(spriteSize.x * scale.x, spriteSize.y * scale.y);
                    e->getComponent<CTransform>().pos = Vec2((wPos.x - ((int)wPos.x % (int)size.x)) + size.x / 2, wPos.y - ((int)wPos.y % (int)size.y) + size.y / 2);
                    e->getComponent<CDraggAble>().dragging = false;
                    m_dragging = false;
                    break;
                }
            }
        }
        
        else if (action.name() == "SPAWN_TILE")
        {
            spawnTile(action.tile());
        }
        else if (action.name() == "MOUSE_MOVE")
        {
            mPos = windowToWorldPos(action.pos());
            buttonHover(mPos);
        }
        else if (action.name() == "SHOW_BUTTON")
        {
            m_showButton = !m_showButton;
        }
    }
    else if (action.type() == "END")
    {
        if (action.name() == "LEFT")
        {
            m_camera->getComponent<CInput>().left = false;
        }
        else if (action.name() == "RIGHT")
        {
            m_camera->getComponent<CInput>().right = false;
        }
        else if (action.name() == "UP")
        {
            m_camera->getComponent<CInput>().up = false;
        }
        else if (action.name() == "DOWN")
        {
            m_camera->getComponent<CInput>().down = false;
        }
        else if (action.name() == "LEFT_CLICK")
        {
            auto pos = windowToWorldPos(action.pos()); 
            m_mouseOnClick = false;
            buttonPressed(pos);
        }
    }
}

void SceneMapEditor::sDrawImGui()
{
}

void SceneMapEditor::sCamera()
{
    auto& playerPos = m_camera->getComponent<CTransform>().pos;
    auto& window = m_game->window();

    float distX = playerPos.x - CameraView.getCenter().x;
    float distY = playerPos.y - CameraView.getCenter().y;

    CameraView.move(sf::Vector2f(distX, distY));


    m_rect.left = (CameraView.getCenter().x - (m_game->m_windowConfig.W / 2)) - m_gridSize.x;
    m_rect.top = (CameraView.getCenter().y - (m_game->m_windowConfig.H / 2)) - m_gridSize.y;

    m_game->window().setView(CameraView);
}


void SceneMapEditor::loadLevel(const std::string& levelPath)
{

}

void SceneMapEditor::saveLevel()
{
    //Save all entities to map/level file

    std::cout << "MAP SAVED\n";
}

void SceneMapEditor::sMovement()
{
    Vec2 playerVelocity(0, 0);

    auto& input = m_camera->getComponent<CInput>();
    auto& playerTransform = m_camera->getComponent<CTransform>();

    // Player movement

    if (input.up)
    {
        playerVelocity.y -= 5;
    }
    else if (input.down)
    {
        playerVelocity.y += 5;
    }



    if (input.right)
    {
        playerVelocity.x += 5;
        playerTransform.scale.x = abs(playerTransform.scale.x);
    }
    else if (input.left)
    {
        playerVelocity.x -= 5;
        playerTransform.scale.x = -abs(playerTransform.scale.x);
    }
    
    m_camera->getComponent<CTransform>().velocity = playerVelocity;


    // Update velocities
    for (auto e : m_entityManager.getEntities())
    {
        e->getComponent<CTransform>().pos +=
            e->getComponent<CTransform>().velocity;
    }
}

void SceneMapEditor::spawnCamera(Vec2 pos)
{
    // TODO: Use player config
    auto entity = m_entityManager.addEntity("Camera");

    entity->addComponent<CTransform>(pos, // POSITION
        Vec2(0.0f, 0.0f), // VELOCITY
        Vec2(4.0f, 4.0f), // SCALE  scale set to 4x16, 4x16
        0);
    entity->addComponent<CBoundingBox>(Vec2(64, 64));

    m_camera = entity;
}

bool SceneMapEditor::buttonPressed(Vec2 pos)
{
    bool result = false;
    for (auto& e : m_entityManager.getEntities("Button"))
    {
        auto& eButton = e->getComponent<CButton>(); 
        if (isInside(pos, e))
        {
            eButton.clicked = m_mouseOnClick; 
            if (!m_mouseOnClick)
            {
                sDoAction(Action(eButton.action));
                result = true;
            }
        }
        else
        {
            eButton.clicked = false;
        }

    }
    return result;
}

Vec2 SceneMapEditor::windowToWorldPos(Vec2 pos)
{
    int cameraLeft = CameraView.getCenter().x - CameraView.getSize().x / 2;
    int cameraTop = CameraView.getCenter().y - CameraView.getSize().y / 2;

    return Vec2((pos.x + cameraLeft), pos.y + cameraTop);
}

bool SceneMapEditor::isInside(Vec2 pos, std::shared_ptr<Entity> e)
{
    auto& size = e->getComponent<CBoundingBox>().halfSize;
    auto ePos = windowToWorldPos(e->getComponent<CTransform>().pos);


    float distX = fabs(pos.x - ePos.x);
    float distY = fabs(pos.y - ePos.y);

    return (distX <= size.x && distY <= size.y);

}


