#include "Action.hpp"
#include "Scene_Play.hpp"
#include "Vec2.hpp"
#include <iostream>
#include <fstream>
#include "Physics.h"
#include <algorithm>
#include "TXT.hpp"
#include <Scene_Menu.hpp>
#include <Scene_Computer.h>



/*
░█████╗░██████╗░██╗███╗░░░███╗███████╗  ░██████╗░█████╗░███████╗███╗░░██╗███████╗
██╔══██╗██╔══██╗██║████╗░████║██╔════╝  ██╔════╝██╔══██╗██╔════╝████╗░██║██╔════╝
██║░░╚═╝██████╔╝██║██╔████╔██║█████╗░░  ╚█████╗░██║░░╚═╝█████╗░░██╔██╗██║█████╗░░
██║░░██╗██╔══██╗██║██║╚██╔╝██║██╔══╝░░  ░╚═══██╗██║░░██╗██╔══╝░░██║╚████║██╔══╝░░
╚█████╔╝██║░░██║██║██║░╚═╝░██║███████╗  ██████╔╝╚█████╔╝███████╗██║░╚███║███████╗
░╚════╝░╚═╝░░╚═╝╚═╝╚═╝░░░░░╚═╝╚══════╝  ╚═════╝░░╚════╝░╚══════╝╚═╝░░╚══╝╚══════╝
*/

CrimeScene::CrimeScene(GameEngine* gameEngine, const std::string& path)
    :Scene(gameEngine)
{

    init(path); 
}

void CrimeScene::init(std::string path)
{
    m_actionMap.clear();
    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::D, "RIGHT");
    registerAction(sf::Keyboard::A, "LEFT");
    registerAction(sf::Keyboard::Escape, "EXIT");
    registerAction(sf::Keyboard::L, "PAUSE");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");   // Toggle drawing textures
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION"); // Toggle drawing collision boxes
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");      // Toggle drawing Grid
    registerAction(sf::Keyboard::F3, "TOGGLE_CAMERA_LIMIT");      // Toggle Camera Limit To World Size
    registerAction(sf::Keyboard::F4, "TOGGLE_DEBUG_MODE");      // Toggle Debug Mode

    //record
    registerAction(sf::Keyboard::R, "RECORD");
    registerAction(sf::Keyboard::P, "PLAYRECORD");
    registerAction(sf::Keyboard::F1, "SCREENSHOT");
    registerAction(sf::Keyboard::F2, "CAMERA_MOVEMENT");
    registerAction(sf::Keyboard::PageUp, "ZOOM_IN");
    registerAction(sf::Keyboard::PageDown, "ZOOM_OUT");
    registerAction(sf::Keyboard::I, "STATS");

    currentState = GameState::Play;

    //dl::setDialogScript("res/config/dialogue_assets.ini", dialogMap); 
    //emt::loadEntityPotrait("res/images/character/Emily/Emily_Assets.ini", m_game->assets(), exp_map); 

    loadLevel(path);
    auto& window = m_game->window();
    CameraView.setSize(window.getSize().x, window.getSize().y);
    CameraView.setCenter(window.getSize().x / 2, window.getSize().y / 2);
    window.setView(CameraView);


    //set View Rect
    m_rect = sf::IntRect((CameraView.getCenter().x - (CameraView.getSize().x / 2)) - m_gridSize.x,
        (CameraView.getCenter().y - (CameraView.getSize().y / 2)) - m_gridSize.y,
        (CameraView.getSize().x) + m_gridSize.x * 2,
        (CameraView.getSize().y) + m_gridSize.y * 2);

    guiSetup("res/config/GUI.ini", m_entityManager);

}

void CrimeScene::sRender()
{
    // TODO: remove this testing code
    auto& window = m_game->window();
    auto date = m_game->getDateSystem();

    if (!m_paused)
    {
        // color the background darker when paused
        window.clear(sf::Color::Black);
    }
    else
    {
        window.clear(sf::Color::Black);
    }
    //draw all tiles

    m_game->window().setView(CameraView);
    if (m_drawTextures)
    {
        for (auto e : m_entityManager.getEntities("Tile"))
        {
            auto& pos = e->getComponent<CTransform>().pos;
            auto& scale = e->getComponent<CTransform>().scale;
            auto& sprite = e->getComponent<CAnimation>().animation.getSprite();
            auto& size = e->getComponent<CAnimation>().animation.getSize();


            sprite.setPosition(pos.x, pos.y);
            sprite.setScale(scale.x, scale.y);

            if (!m_rect.contains(pos.x, pos.y)) continue;
            if (e->getComponent<CDraggAble>().dragging) continue;

            window.draw(sprite);
        }
    }
    if (m_drawTextures)
    {
        for (auto e : m_entityManager.getEntities("Wall"))
        {
            auto& pos = e->getComponent<CTransform>().pos;
            auto& scale = e->getComponent<CTransform>().scale;
            auto& sprite = e->getComponent<CAnimation>().animation.getSprite();
            auto& size = e->getComponent<CAnimation>().animation.getSize();


            sprite.setPosition(pos.x, pos.y);
            sprite.setScale(scale.x, scale.y);

            if (!m_rect.contains(pos.x, pos.y)) continue;
            if (e->getComponent<CDraggAble>().dragging) continue;

            window.draw(sprite);
        }
    }
    // draw all entity textures / animations
    auto& entities = m_entityManager.getEntities("Render");
    std::sort(entities.begin(), entities.end(), [](std::shared_ptr<Entity>& a, std::shared_ptr<Entity>& b)
        {
            auto& aCT = a->getComponent<CTransform>();
            auto& aBB = a->getComponent<CAnimation>().animation;

            auto& bCT = b->getComponent<CTransform>();
            auto& bBB = b->getComponent<CAnimation>().animation;
            return aCT.pos.y + (aBB.getSize().y * aCT.scale.y) / 2 < bCT.pos.y + (bBB.getSize().y * bCT.scale.y) / 2;

        }
    );
    if (m_drawTextures)
    {
        for (auto e : m_entityManager.getEntities("Render"))
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






    // draw bounding box
    if (m_drawCollision)
    {
        for (auto e : m_entityManager.getEntities())
        {
            if (e->hasComponent<CBoundingBox>())
            {
                if (e->tag() == "Gui") continue;
                auto& box = e->getComponent<CBoundingBox>();
                auto& anim = e->getComponent<CAnimation>().animation;
                auto& transform = e->getComponent<CTransform>();
                auto animSize = Vec2(anim.getSize().x * abs(transform.scale.x), anim.getSize().y * abs(transform.scale.y));
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x, box.size.y));
                rect.setPosition((transform.pos.x - animSize.x / 2) + box.offRect.x, (transform.pos.y - animSize.y / 2) + box.offRect.y);
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                rect.setOutlineColor(sf::Color::Red);
                rect.setOutlineThickness(1);
                window.draw(rect);
            }
        }
    }
    m_game->window().setView(window.getDefaultView());

    //draw dialogue box into the screen
    if (currentState == GameState::Dialogue)
    {
        /*auto& sprite = m_guiObjectMap["Dialogue_Box"].img;
        auto& pos = m_guiObjectMap["Dialogue_Box"].pos;
        sprite.setPosition(pos.x, pos.y);
        window.draw(sprite);*/

        for (auto& e : m_entityManager.getEntities("Gui"))
        {
            auto& anim = e->getComponent<CAnimation>().animation;
            auto& sprite = anim.getSprite();
            auto& eCT = e->getComponent<CTransform>();

            sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
            sprite.setPosition(eCT.pos.x, eCT.pos.y);

            window.draw(sprite);

            if (anim.getName() == "dialogue_box")
            {
                for (auto& e : m_entityManager.getEntities("NPC"))
                {
                    auto& cDialog = e->getComponent<CDialogue>();
                    if (cDialog.isOn)
                    {

                        auto& txt = cDialog.dialogue_map[cDialog.dialogueState][cDialog.dialogIndex];
                        //draw dialogue text
                        sf::Text text;
                        text.setFont(m_game->assets().getFont("Times_New_Roman"));
                        text.setString(dl::getDialogueText(txt));
                        text.setFillColor(sf::Color::Black);
                        text.setPosition(sprite.getGlobalBounds().left + 192, sprite.getGlobalBounds().top + 32);

                        window.draw(text);

                        //cDialog.dialogueState = date.getDaysName();
                        //draw potrait
                        //draw emotion in dialogue box
                        sf::Sprite exp;
                        auto& texture = m_game->assets().getTexture(e->name() + dl::getExpression(txt));

                        exp.setTexture(texture);
                        exp.setScale(2.50f, 2.50f);
                        exp.setPosition(sprite.getGlobalBounds().left + 8, sprite.getGlobalBounds().top + 8);
                        window.draw(exp);

                        break;
                    }
                }
            }

        }
    }


    // Draw Stats
    if (m_drawStats)
    {
        m_stats.clear();

        //m_stats.push_back("Zoom Scale\t\t : x " + std::to_string(m_zoomScale));
        //m_stats.push_back("Camera Center\t:  " + std::to_string((int)CameraView.getCenter().x) + ", " + std::to_string((int)CameraView.getCenter().y));
        //m_stats.push_back("World Size\t\t  : " + std::to_string((int)m_worldSize.x) + ", " + std::to_string((int)m_worldSize.y) + " [ tiles ]");
        //m_stats.push_back("Player State " + m_player->getComponent<CState>().state);
        m_stats.push_back(formatNumber(date.getDate().day) + " / " + formatNumber(date.getDate().month) + " / " + formatNumber(date.getDate().year));
        m_stats.push_back(formatNumber(date.getDate().hour) + " : " + formatNumber(date.getDate().minute) + " : " + formatNumber(date.getDate().second));
        m_stats.push_back(date.getDaysName());
        m_stats.push_back(currentState == GameState::Dialogue ? "Dialogue State" : "Play State");

        if (m_debugMode)
        {
            m_stats.push_back("Debug Mode On");
        }
        else
        {
            m_stats.push_back("Debug Mode Off");
        }
        int row = 0;

        for (auto e : m_stats)
        {
            m_text.setCharacterSize(30);
            m_text.setFont(m_game->assets().getFont("Ostrich_Regular"));
            m_text.setString(e);
            m_text.setFillColor(sf::Color::Red);
            m_text.setOutlineColor(sf::Color::Black);
            m_text.setOutlineThickness(2.0f);
            m_text.setStyle(sf::Text::Style::Bold);

            m_text.setPosition(5, (row * (m_text.getGlobalBounds().height + (5))));
            window.draw(m_text);
            row++;
        }
    }

    window.display();
}

void CrimeScene::loadLevel(const std::string& levelPath)
{
    // Reset entity manager on every new level
    m_entityManager = EntityManager();
    std::ifstream fin(levelPath);
    std::ifstream in(levelPath);

    loadMap(fin, "Tile_Layer_1", "width=\"", "height=\"");
    loadObject(in, "object_layer_1", "</objectgroup>");
}

void CrimeScene::spawnPlayer()
{
    // TODO: Use player config
    auto entity = m_entityManager.addEntity("Player");

    entity->addComponent<CTransform>(Vec2(11 * 64, 30 * 64), // POSITION
        Vec2(0.0f, 0.0f), // VELOCITY
        Vec2(4.0f, 4.0f), // SCALE  scale set to 4x16, 4x16
        0);
    entity->addComponent<CAnimation>(
        m_game->assets().getAnimation("IdleD"),
        true
    );
    entity->addComponent<CBoundingBox>(Vec2(12 * 4, 6 * 4), Vec2(2 * 4, 9 * 4));

    m_player = entity;
}

void CrimeScene::spawnObjects(std::string& className, std::string& animationName, Vec2& pos, Vec2 boundingBox, Vec2 off)
{
    // Span object "temporary code"
    std::string id;
    if (className.find("NPC") != std::string::npos)
    {
        id = className.substr(4, className.length() - 4);
        className = "NPC";
    }

    auto object = m_entityManager.addEntity(className);
    object->setName(id);
    //std::cout << animationName << "\n";
    object->addComponent<CTransform>(pos * 4.0f, // POSITION
        Vec2(0.0f, 0.0f), // VELOCITY
        Vec2(4.0f, 4.0f), // SCALE  scale set to 4x16, 4x16
        0);
    object->addComponent<CAnimation>(
        m_game->assets().getAnimation(animationName),
        true
    );
    auto& size = object->getComponent<CAnimation>().animation.getSize();

    Vec2 bb(size.x * 4.0f, size.y * 4.0f);
    if (boundingBox.x > 0 && boundingBox.y > 0)
    {
        object->addComponent<CBoundingBox>(boundingBox * 4, off * 4);
    }
    else
    {
        object->addComponent<CBoundingBox>(bb, Vec2(0, 0));
    }


    if (className == "Player")
    {
        object->addComponent<CState>("IdleD");
        m_player = object;
    }

    //determine whos get the cDraggable component
    if (className == "Furniture")
    {
        object->addComponent<CDraggAble>();
    }

    //npc dialogue set up
    if (className == "NPC")
    {
        object->addComponent<CDialogue>(dl::setupdialogue("res/config/dialogue/" + id + "/EN.ini"));
    }

}

void CrimeScene::loadMap(std::istream& fin, std::string word, std::string prefix1, std::string prefix2)
{
    std::string input;
    while (getline(fin, input))
    {
        if (input.find(word) != std::string::npos)
        {
            m_worldSize.x = ln::getIntFromString(input, prefix1, 3);
            m_worldSize.y = ln::getIntFromString(input, prefix2, 3);

            getline(fin, input);

            int i = 0;
            int col = 0;
            int row = 0;
            while (col < m_worldSize.x && row < m_worldSize.y)
            {
                getline(fin, input, ',');
                input = ln::removeSpacesAndNewlines(input);
                if (input.find("</data>") != std::string::npos)
                {
                    input = input.substr(0, input.find("</data>"));
                }
                if (input == "0") {
                    col++;
                    if (col >= m_worldSize.x)
                    {
                        getline(fin, input);
                        row++;
                        col = 0;
                    }
                    continue;
                }
                auto entity = m_entityManager.addEntity("Tile");
                entity->addComponent<CTransform>(Vec2((col * m_gridSize.x) + m_gridSize.x / 2, (row * m_gridSize.y) + m_gridSize.y / 2), // POSITION
                    Vec2(0.0f, 0.0f), // VELOCITY
                    Vec2(4.0f, 4.0f), // SCALE  scale set to 4x16, 4x16
                    0);

                entity->addComponent<CAnimation>(
                    m_game->assets().getAnimation(input),
                    true
                );
                col++;
                if (col >= m_worldSize.x)
                {
                    getline(fin, input);
                    row++;
                    col = 0;
                }
                i++;
            }
            return;
        }
    }
}

void CrimeScene::loadObject(std::istream& fin, std::string start, std::string end)
{
    std::string input;

    std::string name, type, id;
    float x = 0, y = 0;
    int index = 1;

    //for aabb
    int offx, offy, wb, hb;
    while (getline(fin, input))
    {
        if (input.find(start) != std::string::npos)
        {
            getline(fin, input);
            while (true)
            {
                //std::cout << index << "\n";
                index++;
                if (input.find(end) != std::string::npos)
                {
                    break;
                }
                if (input.find("<properties>") != std::string::npos) continue;
                x = std::stof(ln::getValueFromString(input, "x=\"", "\" y="));
                y = std::stof(ln::getValueFromString(input, "y=\"", "\" width"));

                if (input.find("/>") != std::string::npos)
                {
                    y -= std::stof(ln::getValueFromString(input, "height=\"", "\"/>")) / 2;
                }
                else
                {
                    y -= std::stof(ln::getValueFromString(input, "height=\"", "\">")) / 2;
                }

                x += std::stof(ln::getValueFromString(input, "width=\"", "\" height")) / 2;

                Vec2 vec(x, y);
                name = ln::getValueFromString(input, "name=\"", "\" type");

                type = ln::getValueFromString(input, "type=\"", "\" gid");

                getline(fin, input);
                if (input.find("<properties>") != std::string::npos)
                {
                    getline(fin, input);
                    hb = std::stof(ln::getValueFromString(input, "value=\"", "\"/>"));

                    getline(fin, input);
                    offx = std::stof(ln::getValueFromString(input, "value=\"", "\"/>"));

                    getline(fin, input);
                    offy = std::stof(ln::getValueFromString(input, "value=\"", "\"/>"));

                    getline(fin, input);
                    wb = std::stof(ln::getValueFromString(input, "value=\"", "\"/>"));

                    getline(fin, input);
                    getline(fin, input);
                    getline(fin, input);

                    spawnObjects(type, name, vec, Vec2(wb, hb), Vec2(offx, offy));
                }
                else
                {
                    spawnObjects(type, name, vec, Vec2(0, 0), Vec2(0, 0));
                }
            }
            return;
        }
    }
}

void CrimeScene::update()
{
    if (!m_paused && currentState == GameState::Play)
    {
        m_game->getDateSystem().update(m_currentFrame);
        sDragAndDrop();
        sMovement();
        sCollision();
        sCamera();
        // sLifespan();
    }
    sAnimation();

    m_entityManager.update();
    sRender();
    m_currentFrame++;
}

void CrimeScene::onEnd()
{
    //std::ofstream out("OfficeSceneAction.txt");
    m_game->changeScene("MENU", std::make_shared<SceneMenu>(m_game));
}

void CrimeScene::sDoAction(const Action& action)
{
    if (action.type() == "START")
    {
        if (action.name() == "LEFT")
        {
            m_player->getComponent<CInput>().left = true;
        }
        else if (action.name() == "RIGHT")
        {
            m_player->getComponent<CInput>().right = true;
        }
        else if (action.name() == "UP")
        {
            m_player->getComponent<CInput>().up = true;
        }
        else if (action.name() == "DOWN")
        {
            m_player->getComponent<CInput>().down = true;
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
        else if (action.name() == "CAMERA_MOVEMENT")
        {
            //smoothCam = !smoothCam;
        }
        else if (action.name() == "SCREENSHOT")
        {
            screenShot();
        }
        else if (action.name() == "TOGGLE_CAMERA_LIMIT")
        {
            //m_cameraLimit = !m_cameraLimit;
        }
        else if (action.name() == "ZOOM_IN")
        {
            auto& window = m_game->window();
            //if (m_zoomScale > 0.5f && m_zoomScale <= 2.0f)
            //{
            //    CameraView.setSize(window.getSize().x, window.getSize().y);
            //    m_zoomScale -= 0.25f;
            //    CameraView.zoom(m_zoomScale);

            //    //set View Rect
            //    m_rect.width = CameraView.getSize().x + m_gridSize.x * 2;
            //    m_rect.height = CameraView.getSize().y + m_gridSize.y * 2;
            //}
        }
        else if (action.name() == "ZOOM_OUT")
        {
            auto& window = m_game->window();
            //if (m_zoomScale >= 0.25f && m_zoomScale < 2.0f)
            //{
            //    CameraView.setSize(window.getSize().x, window.getSize().y);
            //    m_zoomScale += 0.25f;
            //    CameraView.zoom(m_zoomScale);

            //    //set View Rect
            //    m_rect.width = CameraView.getSize().x + m_gridSize.x * 2;
            //    m_rect.height = CameraView.getSize().y + m_gridSize.y * 2;
            //}
        }
        else if (action.name() == "STATS")
        {
            m_drawStats = !m_drawStats;
        }
        else if (action.name() == "LEFT_CLICK")
        {
            m_mouseOnClick = true;
            //std::cout << action.pos().x << " " << action.pos().y << "\n";
            auto wPos = windowToWorldPos(action.pos());

            //Check Computer Click
            if (!m_paused && currentState == GameState::Play)
            {
                checkCollisionMouse(wPos, "Key", Action("COMPUTER_SCENE", "START"));
                dragAndDrop(wPos);
            }
            if (currentState == GameState::Play) checkCollisionMouse(wPos, "NPC", Action("START_DIALOGUE", "START"));
        }
        else if (action.name() == "MOUSE_MOVE")
        {
            m_pos = windowToWorldPos(action.pos());
        }
        else if (action.name() == "TOGGLE_DEBUG_MODE")
        {
            if (m_debugMode && m_dragging)
            {
                for (auto& e : m_entityManager.getEntities())
                {
                    if (e->hasComponent<CDraggAble>() && e->getComponent<CDraggAble>().dragging)
                    {
                        e->getComponent<CTransform>().pos = m_lastDragPos;
                        e->getComponent<CDraggAble>().dragging = false;
                        break;
                    }
                }

                m_dragging = false;
            }
            m_debugMode = !m_debugMode;
        }
        else if (action.name() == "COMPUTER_SCENE")
        {
            m_game->changeScene("COMPUTER",
                std::make_shared<SceneComputer>(
                    m_game,
                    "res/config/cComp.ini")
                );
        }
        else if (action.name() == "START_DIALOGUE")
        {
            currentState == GameState::Dialogue ? currentState = GameState::Play : currentState = GameState::Dialogue;
        }
        else if (action.name() == "NEXT_DIALOGUE")
        {
            proceedDialogues(m_entityManager);
        }
    }
    else if (action.type() == "END")
    {
        if (action.name() == "LEFT")
        {
            m_player->getComponent<CInput>().left = false;
        }
        else if (action.name() == "RIGHT")
        {
            m_player->getComponent<CInput>().right = false;
        }
        else if (action.name() == "UP")
        {
            m_player->getComponent<CInput>().up = false;
        }
        else if (action.name() == "DOWN")
        {
            m_player->getComponent<CInput>().down = false;
        }
        else if (action.name() == "LEFT_CLICK")
        {
            m_mouseOnClick = false;
            buttonPressed(action.pos(), "Gui", m_entityManager);
        }
    }
}

void CrimeScene::sDrawImGui()
{
}

void CrimeScene::sCamera()
{

    auto& playerPos = m_player->getComponent<CTransform>().pos;
    auto& window = m_game->window();

    float distX = playerPos.x - CameraView.getCenter().x;
    float distY = playerPos.y - CameraView.getCenter().y;
    float scale;

    scale = 1.0f;

    CameraView.move(sf::Vector2f(distX * scale, distY * scale));
    

    m_rect.left = (CameraView.getCenter().x - (CameraView.getSize().x / 2)) - m_gridSize.x;
    m_rect.top = (CameraView.getCenter().y - (CameraView.getSize().y / 2)) - m_gridSize.y;

}

void CrimeScene::sMovement()
{
    Vec2 playerVelocity(0, 0);

    auto& input = m_player->getComponent<CInput>();
    auto& playerState = m_player->getComponent<CState>().state;
    auto& playerTransform = m_player->getComponent<CTransform>();


    playerTransform.prevPos = playerTransform.pos;
    // Player movement

    if (input.up)
    {
        playerState = "RunU";
        playerVelocity.y -= 5;
    }
    else if (input.down)
    {
        playerVelocity.y += 5;
        playerState = "RunD";
    }
    else if (!input.up && playerState == "RunU")
    {
        playerState = "IdleU";
    }
    else if (!input.down && playerState == "RunD")
    {
        playerState = "IdleD";
    }

    if (input.right)
    {
        playerVelocity.x += 5;
        playerState = "RunR";
        playerTransform.scale.x = abs(playerTransform.scale.x);
    }
    else if (input.left)
    {
        playerVelocity.x -= 5;
        playerState = "RunL";
        playerTransform.scale.x = -abs(playerTransform.scale.x);
    }
    else if (!input.right && playerState == "RunR")
    {
        playerState = "IdleR";
        playerTransform.scale.x = abs(playerTransform.scale.x);
    }
    else if (!input.left && playerState == "RunL")
    {
        playerState = "IdleL";
        playerTransform.scale.x = -abs(playerTransform.scale.x);
    }
    m_player->getComponent<CTransform>().velocity = playerVelocity;


    // Update velocities
    for (auto e : m_entityManager.getEntities())
    {
        e->getComponent<CTransform>().pos +=
            e->getComponent<CTransform>().velocity;
    }
}
/* Helper function */
void CrimeScene::setAnimation(std::shared_ptr<Entity> entity,
    const std::string& animationName, bool repeat)
{
    // TODO: Maybe this function should be outside the class
    if (entity->getComponent<CAnimation>().animation.getName() != animationName)
    {
        entity->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), repeat);
    }
}

void CrimeScene::sAnimation()
{
    const auto& playerState = m_player->getComponent<CState>().state;
    //std::cout << m_player->hasComponent<CAnimation>() ? "Have Animation\n" : "Not Have Animation\n";
    if (currentState == GameState::Play)
    {
        if (playerState == "RunR")
        {
            setAnimation(m_player, "RunR", true);
        }
        else if (playerState == "RunL")
        {
            setAnimation(m_player, "RunR", true);
        }
        else if (playerState == "RunU")
        {
            setAnimation(m_player, "RunU", true);
        }
        else if (playerState == "RunD")
        {
            setAnimation(m_player, "RunD", true);
        }
        else if (playerState == "IdleR")
        {
            setAnimation(m_player, "IdleR", true);
        }
        else if (playerState == "IdleL")
        {
            setAnimation(m_player, "IdleR", true);
        }
        else if (playerState == "IdleU")
        {
            setAnimation(m_player, "IdleU", true);
        }
        else if (playerState == "IdleD")
        {
            setAnimation(m_player, "IdleD", true);
        }

        /*if (playerState == "RunR")
        {
            setAnimation(m_player, "Emily_runR", true);
        }
        else if (playerState == "RunL")
        {
            setAnimation(m_player, "Emily_runR", true);
        }
        else if (playerState == "RunU")
        {
            setAnimation(m_player, "Emily_runU", true);
        }
        else if (playerState == "RunD")
        {
            setAnimation(m_player, "Emily_runD", true);
        }
        else if (playerState == "IdleR")
        {
            setAnimation(m_player, "Emily_idleR", true);
        }
        else if (playerState == "IdleL")
        {
            setAnimation(m_player, "Emily_idleR", true);
        }
        else if (playerState == "IdleU")
        {
            setAnimation(m_player, "Emily_idleU", true);
        }
        else if (playerState == "IdleD")
        {
            setAnimation(m_player, "Emily_idleD", true);
        }*/
    }
    else if (currentState == GameState::Dialogue)
    {
        if (playerState == "RunR")
        {
            setAnimation(m_player, "IdleR", true);
        }
        else if (playerState == "RunL")
        {
            setAnimation(m_player, "IdleR", true);
        }
        else if (playerState == "RunU")
        {
            setAnimation(m_player, "IdleU", true);
        }
        else if (playerState == "RunD")
        {
            setAnimation(m_player, "IdleD", true);
        }
    }

    for (auto& e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CAnimation>())
        {
            e->getComponent<CAnimation>().animation.update();
        }
    }
}

void CrimeScene::sCollision()
{
    // TODO: Remove this testing code. Imaginary platform
    auto& playerTransform = m_player->getComponent<CTransform>();
    auto& playerState = m_player->getComponent<CState>().state;

    for (auto& e : m_entityManager.getEntities())
    {
        if (!e->hasComponent<CBoundingBox>() || e == m_player || e->tag() == "Gui") continue;

        if (m_physics.isIntersect(m_player, e))
        {
            playerTransform.pos += playerTransform.velocity * -1;
            break;
        }
        /*auto overLap = m_physics.getOverlap(m_player, e);
        auto prevOverLap = m_physics.getPrevOverlap(m_player, e);
        auto ePos = e->getComponent<CTransform>().pos;


        if (overLap.x > 0 && overLap.y > 0)
        {
            if (prevOverLap.y > 0 && playerTransform.pos.x > ePos.x)
            {
                playerTransform.pos += Vec2(overLap.x, 0);
            }
            else if (prevOverLap.y > 0 && playerTransform.pos.x < ePos.x)
            {
                playerTransform.pos += Vec2(-overLap.x, 0);
            }

            else if (prevOverLap.x > 0 && playerTransform.pos.y < ePos.y)
            {
                playerTransform.pos += Vec2(0, -overLap.y);
            }
            else if (prevOverLap.x > 0 && playerTransform.pos.y > ePos.y)
            {
                playerTransform.pos += Vec2(0, overLap.y);
            }
            break;
        }
        */
    }
}

bool CrimeScene::checkCollision(Vec2 ca, Vec2 cb, float ra, float rb)
{
    float dist = ca.dist(cb);
    return dist <= (ra + rb);
}

void CrimeScene::drawLine(const Vec2& p1, const Vec2& p2)
{
    sf::Vertex line[] = { sf::Vector2f(p1.x, p1.y), sf::Vector2f(p2.x, p2.y) };
    m_game->window().draw(line, 2, sf::Lines);
}

void CrimeScene::sDragAndDrop()
{
    for (auto& e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CDraggAble>() && e->getComponent<CDraggAble>().dragging)
        {
            e->getComponent<CTransform>().pos = m_pos;
        }
    }
}

Vec2 CrimeScene::windowToWorldPos(Vec2 pos)
{
    int cameraLeft = CameraView.getCenter().x - CameraView.getSize().x / 2;
    int cameraTop = CameraView.getCenter().y - CameraView.getSize().y / 2;

    return Vec2((pos.x + cameraLeft), pos.y + cameraTop);
}

void CrimeScene::screenShot()
{
    std::ifstream fin("res/config/log.ini");
    std::string name, token;
    int index = 0;

    fin >> token;

    if (token == "Last_Screenshot")
    {
        fin >> index;
        index++;
        name = "Screenshot" + std::to_string(index) + ".png";
    }
    else
    {
        name = "Screenshot.png";
    }

    auto& window = m_game->window();
    sf::Texture texture;
    texture.create(window.getSize().x, window.getSize().y);
    texture.update(window);
    texture.copyToImage().saveToFile("res/images/screenshot/" + name);

    std::ofstream save;
    save.open("res/config/log.ini");

    save << "Last_Screenshot " + std::to_string(index) + " " + name;
    save.close();
}

void CrimeScene::checkCollisionMouse(Vec2 pos, const std::string& name, Action action)
{
    for (auto& e : m_entityManager.getEntities(name))
    {
        auto& ePos = e->getComponent<CTransform>().pos;
        auto& pPos = m_player->getComponent<CTransform>().pos;
        if (isInside(pos, e) && abs(ePos.x - pPos.x) <= 96 && abs(ePos.y - pPos.y) <= 96)
        {
            sDoAction(action);

            if (name == "NPC")
            {
                auto& dialogue = e->getComponent<CDialogue>();
                dialogue.isOn = true;
            }
        }
    }
}

void CrimeScene::dragAndDrop(Vec2 pos)
{
    if (!m_debugMode) return;
    for (auto e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CDraggAble>() && isInside(pos, e) && !m_dragging)
        {
            e->getComponent<CDraggAble>().dragging = true;
            m_dragging = true;
            m_lastDragPos = e->getComponent<CTransform>().pos;
            break;
        }
        else if (m_dragging && e->getComponent<CDraggAble>().dragging)
        {
            auto& spriteSize = e->getComponent<CAnimation>().animation.getSize();
            auto& scale = e->getComponent<CTransform>().scale;

            Vec2 size(spriteSize.x * scale.x, spriteSize.y * scale.y);
            //e->getComponent<CTransform>().pos = Vec2((wPos.x - ((int)wPos.x % (int)size.x))+size.x/2, wPos.y - ((int)wPos.y % (int)size.y) + size.y/2);
            e->getComponent<CTransform>().pos = Vec2(pos.x, pos.y);
            e->getComponent<CDraggAble>().dragging = false;
            m_dragging = false;
        }
    }
}


std::ifstream file;
std::ofstream out;

/*
░█████╗░███████╗███████╗██╗░█████╗░███████╗  ░██████╗░█████╗░███████╗███╗░░██╗███████╗
██╔══██╗██╔════╝██╔════╝██║██╔══██╗██╔════╝  ██╔════╝██╔══██╗██╔════╝████╗░██║██╔════╝
██║░░██║█████╗░░█████╗░░██║██║░░╚═╝█████╗░░  ╚█████╗░██║░░╚═╝█████╗░░██╔██╗██║█████╗░░
██║░░██║██╔══╝░░██╔══╝░░██║██║░░██╗██╔══╝░░  ░╚═══██╗██║░░██╗██╔══╝░░██║╚████║██╔══╝░░
╚█████╔╝██║░░░░░██║░░░░░██║╚█████╔╝███████╗  ██████╔╝╚█████╔╝███████╗██║░╚███║███████╗
░╚════╝░╚═╝░░░░░╚═╝░░░░░╚═╝░╚════╝░╚══════╝  ╚═════╝░░╚════╝░╚══════╝╚═╝░░╚══╝╚══════╝
*/

OfficeScene::OfficeScene(GameEngine* gameEngine, const std::string& levelPath)
    : Scene(gameEngine)
    ,m_levelPath(levelPath)
{
    init(levelPath);
    int i = 0;
}

void OfficeScene::init(const std::string& levelPath)
{
    m_actionMap.clear(); 
    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::D, "RIGHT");
    registerAction(sf::Keyboard::A, "LEFT");
    registerAction(sf::Keyboard::Escape, "EXIT");
    registerAction(sf::Keyboard::L, "PAUSE");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");   // Toggle drawing textures
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION"); // Toggle drawing collision boxes
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");      // Toggle drawing Grid
    registerAction(sf::Keyboard::F3, "TOGGLE_CAMERA_LIMIT");      // Toggle Camera Limit To World Size
    registerAction(sf::Keyboard::F4, "TOGGLE_DEBUG_MODE");      // Toggle Debug Mode

    //record
    registerAction(sf::Keyboard::R, "RECORD");
    registerAction(sf::Keyboard::P, "PLAYRECORD");
    registerAction(sf::Keyboard::F1, "SCREENSHOT");
    registerAction(sf::Keyboard::F2, "CAMERA_MOVEMENT");
    registerAction(sf::Keyboard::PageUp, "ZOOM_IN");
    registerAction(sf::Keyboard::PageDown, "ZOOM_OUT");
    registerAction(sf::Keyboard::I, "STATS");

    currentState = GameState::Play;

    //dl::setDialogScript("res/config/dialogue_assets.ini", dialogMap); 
    //emt::loadEntityPotrait("res/images/character/Emily/Emily_Assets.ini", m_game->assets(), exp_map); 

    loadLevel(levelPath);
    auto& window = m_game->window();
    CameraView.setSize(window.getSize().x, window.getSize().y);
    CameraView.setCenter(window.getSize().x/2, window.getSize().y/2);
    CameraView.zoom(m_zoomScale);
    window.setView(CameraView);


    //set View Rect
    m_rect = sf::IntRect((CameraView.getCenter().x - (CameraView.getSize().x / 2)) - m_gridSize.x,
        (CameraView.getCenter().y - (CameraView.getSize().y / 2)) - m_gridSize.y,
        (CameraView.getSize().x) + m_gridSize.x * 2,
        (CameraView.getSize().y) + m_gridSize.y * 2);
    guiSetup("res/config/GUI.ini", m_entityManager);
}

void OfficeScene::loadLevel(const std::string& levelPath)
{
    // Reset entity manager on every new level
    m_entityManager = EntityManager();
    std::ifstream fin(levelPath);
    std::ifstream in(levelPath);

    loadMap(fin, "Tile_Layer_1", "width=\"", "height=\"");
    loadObject(in, "object_layer_1", "</objectgroup>");
    //spawnPlayer();
}

void OfficeScene::spawnPlayer()
{
    // TODO: Use player config
    auto entity = m_entityManager.addEntity("Player");

    entity->addComponent<CTransform>(   Vec2(11 *64, 30 * 64), // POSITION
                                        Vec2(0.0f, 0.0f), // VELOCITY
                                        Vec2(4.0f, 4.0f), // SCALE  scale set to 4x16, 4x16
                                        0); 
    entity->addComponent<CAnimation>(
        m_game->assets().getAnimation("IdleD"),
        true
    );
    entity->addComponent<CBoundingBox>(Vec2(12*4, 6*4), Vec2(2*4,9*4));

    m_player = entity;
}

void OfficeScene::spawnObjects(std::string& className, std::string& animationName, Vec2& pos, Vec2 boundingBox, Vec2 off)
{
    // Span object "temporary code"
    std::string id;
    if (className.find("NPC") != std::string::npos) 
    {
        id = className.substr(4, className.length() - 4);  
        className = "NPC"; 
    }

    auto object = m_entityManager.addEntity(className);
    object->setName(id);
    //std::cout << animationName << "\n";
    object->addComponent<CTransform>(pos*4.0f, // POSITION
        Vec2(0.0f, 0.0f), // VELOCITY
        Vec2(4.0f, 4.0f), // SCALE  scale set to 4x16, 4x16
        0);
    object->addComponent<CAnimation>(
        m_game->assets().getAnimation(animationName),
        true
    );
    auto& size = object->getComponent<CAnimation>().animation.getSize();

    Vec2 bb(size.x * 4.0f, size.y * 4.0f);
    if (boundingBox.x > 0 && boundingBox.y > 0)
    {
        object->addComponent<CBoundingBox>(boundingBox * 4, off * 4);
    }
    else
    {
        object->addComponent<CBoundingBox>(bb, Vec2(0,0));
    }


    if (className == "Player")
    {
        object->addComponent<CState>("IdleD"); 
        m_player = object;
    }

    //determine whos get the cDraggable component
    if (className == "Furniture")
    {
        object->addComponent<CDraggAble>();
    }

    //npc dialogue set up
    if (className == "NPC")
    {
        object->addComponent<CDialogue>(dl::setupdialogue("res/config/dialogue/" + id +"/EN.json"));
    }

}

void OfficeScene::playRecord(int& frame)
{
    if(file.eof() == 1)
    {
        file.close();
        m_playRecord = false; 
        std::cout << "Finish" << "\n";
    }
    else if (frame == r_frame)
    {
        sDoAction(Action(r_name, r_type));
        file >> r_frame >> r_name >> r_type;
    }
    else if(frame - r_frame == 1)
    {
        frame--;
        sDoAction(Action(r_name, r_type));
        file >> r_frame >> r_name >> r_type;
    }
}

void OfficeScene::record(const Action& action)
{
    //actionTextRecord += std::to_string(currentOfficeSceneFrame) + " " + action.name() + " " + action.type() + "\n";
}

void OfficeScene::loadMap(std::istream& fin, std::string word, std::string prefix1, std::string prefix2)
{
    std::string input;
    while (getline(fin, input))
    {
        if (input.find(word) != std::string::npos)
        {
            m_worldSize.x = ln::getIntFromString(input, prefix1, 3);
            m_worldSize.y = ln::getIntFromString(input, prefix2, 3);

            getline(fin, input);

            int i = 0;
            int col = 0;
            int row = 0;
            while (col < m_worldSize.x && row < m_worldSize.y)
            {
                getline(fin, input, ',');
                input = ln::removeSpacesAndNewlines(input);  
                if (input.find("</data>") != std::string::npos)
                {
                    input = input.substr(0, input.find("</data>"));
                }
                if (input == "0") {
                    col++; 
                    if (col >= m_worldSize.x)
                    {
                        getline(fin, input);   
                        row++; 
                        col = 0; 
                    }
                    continue;
                }
                auto entity = m_entityManager.addEntity("Tile");
                entity->addComponent<CTransform>(Vec2((col * m_gridSize.x)+ m_gridSize.x/2, (row * m_gridSize.y)+ m_gridSize.y/2), // POSITION
                    Vec2(0.0f, 0.0f), // VELOCITY
                    Vec2(4.0f, 4.0f), // SCALE  scale set to 4x16, 4x16
                    0);

                entity->addComponent<CAnimation>(
                    m_game->assets().getAnimation(input),
                    true
                );
                col++;
                if (col >= m_worldSize.x)
                {
                    getline(fin, input);
                    row++;
                    col = 0;
                }
                i++;
            }
            return;
        }
    }
}

void OfficeScene::loadObject(std::istream& fin, std::string start, std::string end)
{
    std::string input;

    std::string name, type, id;
    float x = 0, y = 0;
    int index = 1;

    //for aabb
    int offx, offy, wb, hb;
    while (getline(fin, input))
    {
        if (input.find(start) != std::string::npos)
        {
            getline(fin, input);
            while (true)
            {
                //std::cout << index << "\n";
                index++;
                if (input.find(end) != std::string::npos)
                {
                    break;
                }
                if (input.find("<properties>") != std::string::npos) continue;
                x = std::stof(ln::getValueFromString(input, "x=\"", "\" y="));
                y = std::stof(ln::getValueFromString(input, "y=\"", "\" width"));

                if (input.find("/>") != std::string::npos)
                {
                    y -= std::stof(ln::getValueFromString(input, "height=\"", "\"/>"))/2;
                }
                else
                {
                    y -= std::stof(ln::getValueFromString(input, "height=\"", "\">"))/2;
                }

                x += std::stof(ln::getValueFromString(input, "width=\"", "\" height")) / 2;

                Vec2 vec(x,y);
                name = ln::getValueFromString(input, "name=\"", "\" type");

                type = ln::getValueFromString(input, "type=\"", "\" gid");

                getline(fin, input);
                if (input.find("<properties>") != std::string::npos)
                {
                    getline(fin, input);
                    hb = std::stof(ln::getValueFromString(input, "value=\"", "\"/>"));

                    getline(fin, input);
                    offx = std::stof(ln::getValueFromString(input, "value=\"", "\"/>"));

                    getline(fin, input);
                    offy = std::stof(ln::getValueFromString(input, "value=\"", "\"/>"));

                    getline(fin, input);
                    wb = std::stof(ln::getValueFromString(input, "value=\"", "\"/>"));

                    getline(fin, input);
                    getline(fin, input);
                    getline(fin, input);
                    
                    spawnObjects(type, name, vec, Vec2(wb, hb), Vec2(offx, offy));
                }
                else
                {
                    spawnObjects(type, name, vec, Vec2(0, 0), Vec2(0,0)); 
                }
            }
            return;
        }
    }
}

void OfficeScene::update()
{
    if (!m_paused)
    {
        m_game->getDateSystem().update(m_currentFrame);
        if (m_playRecord)
        {
            //playRecord(currentOfficeSceneFrame);
        }
        if (currentState == GameState::Play)
        {
            sDragAndDrop();
            sMovement();
            sCollision();
            sCamera();
        }
            // sLifespan();
        
        sAnimation();
    }
    
    m_entityManager.update();
    sRender();
    m_currentFrame++;
}

void OfficeScene::onEnd()
{
    //std::ofstream out("OfficeSceneAction.txt");
    m_game->changeScene("MENU", std::make_shared<SceneMenu>(m_game));
}

void OfficeScene::sDoAction(const Action& action)
{
    if (m_record && action.name() != "RECORD" && action.name() != "PLAYRECORD")
    {
        record(action);
    }
    if (action.type() == "START")
    {
        if (action.name() == "LEFT")
        {
            m_player->getComponent<CInput>().left = true;
        }
        else if (action.name() == "RIGHT")
        {
            m_player->getComponent<CInput>().right = true;
        }
        else if (action.name() == "UP")
        {
            m_player->getComponent<CInput>().up = true;
        }
        else if (action.name() == "DOWN")
        {
            m_player->getComponent<CInput>().down = true;
        }
        else if (action.name() == "EXIT")
        {
            //onEnd();
            m_paused = !m_paused;
            ImGui::CloseCurrentPopup();
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
        else if (action.name() == "RECORD")
        {
            if (!m_record)
            {
                std::cout << "Record Started!" << "\n";
                //currentOfficeSceneFrame = 0;

                //Delete file record that exist in the file
                out.open("res/record/OfficeSceneAction.txt", std::ios::trunc);
                out.close();

                out.open("res/record/OfficeSceneAction.txt");

                m_record = true;
            }
            else
            {
                std::cout << "Record Ended!" << "\n";
                out << actionTextRecord;
                m_record = false;
                out.close();
                actionTextRecord = "";
            }
        }
        else if (action.name() == "PLAYRECORD")
        {
            if (m_record)
            {
                for (auto& it : m_actionMap) {
                    if (it.second == "RECORD") {
                        sDoAction(Action(getActionMap().at(it.first), "START"));
                    }
                }
                std::cout << "STOP" << "\n";
                m_record = false;
            }
            std::cout << "Play Record" << "\n";
            file = std::ifstream("res/record/OfficeSceneAction.txt");
            file >> r_frame >> r_name >> r_type;
            //currentOfficeSceneFrame = 0;
            m_playRecord = true;
        }
        else if (action.name() == "CAMERA_MOVEMENT")
        {
            smoothCam = !smoothCam;
        }
        else if (action.name() == "SCREENSHOT")
        {
            screenShot();
        }
        else if (action.name() == "TOGGLE_CAMERA_LIMIT")
        {
            m_cameraLimit = !m_cameraLimit;
        }
        else if (action.name() == "ZOOM_IN")
        {
            auto& window = m_game->window();
            if (m_zoomScale > 0.5f && m_zoomScale <= 2.0f)
            {
                CameraView.setSize(window.getSize().x, window.getSize().y);
                m_zoomScale -= 0.25f;
                CameraView.zoom(m_zoomScale);

                //set View Rect
                m_rect.width = CameraView.getSize().x + m_gridSize.x * 2;
                m_rect.height = CameraView.getSize().y + m_gridSize.y * 2;
            }
        }
        else if (action.name() == "ZOOM_OUT")
        {
            auto& window = m_game->window();
            if (m_zoomScale >= 0.25f && m_zoomScale < 2.0f)
            {
                CameraView.setSize(window.getSize().x, window.getSize().y);
                m_zoomScale += 0.25f;
                CameraView.zoom(m_zoomScale);

                //set View Rect
                m_rect.width = CameraView.getSize().x + m_gridSize.x * 2;
                m_rect.height = CameraView.getSize().y + m_gridSize.y * 2;
            }
        }
        else if (action.name() == "STATS")
        {
            m_drawStats = !m_drawStats;
        }
        else if (action.name() == "LEFT_CLICK")
        {
            m_mouseOnClick = true;
            //std::cout << action.pos().x << " " << action.pos().y << "\n";
            auto wPos = windowToWorldPos(action.pos()); 

            //Check Computer Click
            if (!m_paused && currentState == GameState::Play)
            {
                checkCollisionMouse(wPos, "Key", Action("COMPUTER_SCENE", "START"));  
                dragAndDrop(wPos); 
            }
            if(currentState == GameState::Play) checkCollisionMouse(wPos, "NPC", Action("START_DIALOGUE", "START")); 
        }
        else if (action.name() == "MOUSE_MOVE")
        {
            m_pos = windowToWorldPos(action.pos());
        }
        else if (action.name() == "TOGGLE_DEBUG_MODE")
        {
            if (m_debugMode && m_dragging)
            {
                for (auto& e : m_entityManager.getEntities())
                {
                    if (e->hasComponent<CDraggAble>() && e->getComponent<CDraggAble>().dragging)
                    {
                        e->getComponent<CTransform>().pos = m_lastDragPos; 
                        e->getComponent<CDraggAble>().dragging = false; 
                        break;
                    }
                }

                m_dragging = false;
            }
            m_debugMode = !m_debugMode;
        }
        else if(action.name() == "COMPUTER_SCENE")
        {
            m_game->changeScene("COMPUTER",
                std::make_shared<SceneComputer>(
                    m_game,
                    "res/config/cComp.ini")
            );
        }
        else if (action.name() == "START_DIALOGUE")
        {
            currentState == GameState::Dialogue ? currentState = GameState::Play : currentState = GameState::Dialogue;  
        }
        else if (action.name() == "NEXT_DIALOGUE")
        {
            proceedDialogues(m_entityManager);
        }
    }
    else if (action.type() == "END")
    {
        if (action.name() == "LEFT")
        {
            m_player->getComponent<CInput>().left = false;
        }
        else if (action.name() == "RIGHT")
        {
            m_player->getComponent<CInput>().right = false;
        }
        else if (action.name() == "UP")
        {
            m_player->getComponent<CInput>().up = false;
        }
        else if (action.name() == "DOWN")
        {
            m_player->getComponent<CInput>().down = false;
        }
        else if (action.name() == "LEFT_CLICK")
        {
            m_mouseOnClick = false;
            buttonPressed(action.pos(), "Gui", m_entityManager);
        }
    }
}

void OfficeScene::sDrawImGui()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiTableColumnFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoBackground;

    auto& window = m_game->window();
    auto date = m_game->getDateSystem();

    if (m_drawStats)
    {
        ImGui::SetNextWindowPos(ImVec2(window.getSize().x, window.getSize().y), ImGuiCond_Once, ImVec2(1, 1));
        //ImGui::SetNextWindowSize(sf::Vector2f(200, 150), ImGuiCond_Once); 
        m_stats.clear();
        //adding stats
        m_stats.push_back(formatNumber(date.getDate().day) + " / " + formatNumber(date.getDate().month) + " / " + formatNumber(date.getDate().year));
        m_stats.push_back(formatNumber(date.getDate().hour) + " : " + formatNumber(date.getDate().minute) + " : " + formatNumber(date.getDate().second));
        m_stats.push_back(date.getDaysName());
        m_stats.push_back(GameStateName.at(currentState));

        if (ImGui::Begin("Stats", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
        {
            for (auto& text : m_stats)
            {
                ImGui::TextColored(ImVec4(0, 1, 0, 1), text.c_str());
            }

            ImGui::Checkbox("Debug Mode", &m_debugMode);
            ImGui::Checkbox("Show Collision Rect", &m_drawCollision);
            ImGui::Checkbox("Draw Texture", &m_drawTextures);

        }
        ImGui::End();
    }

    // Draw Menu Bar
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(250, 300), ImGuiCond_Once);

    if (m_paused)
        ImGui::OpenPopup("Pause");
    else
        ImGui::CloseCurrentPopup();

    if (ImGui::BeginPopupModal("Pause", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar))
    {
        ImVec2 windowSize = ImGui::GetWindowSize();
        float buttonWidth = 120.0f;

        ImGui::SetCursorPosX((windowSize.x - buttonWidth) * 0.5f);
        if (ImGui::Button("Resume", ImVec2(buttonWidth, 0)))
        {
            ImGui::CloseCurrentPopup();
            m_paused = false;
        }
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

        ImGui::SetCursorPosX((windowSize.x - buttonWidth) * 0.5f);
        if (ImGui::Button("Main Menu", ImVec2(buttonWidth, 0)))
        {
            ImGui::CloseCurrentPopup();
            onEnd();
        }
    }

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
    if (ImGui::BeginMainMenuBar()) // Use BeginMenuBar() instead of BeginMainMenuBar()
    {
        if (ImGui::BeginMenu("Main"))
        {
            if (ImGui::MenuItem("Main Menu"))
            {
                onEnd();
            }
            if (ImGui::MenuItem("Load"))
            {
                // Load logic
            }
            if (ImGui::Checkbox("Fullscreen",(bool *)&m_game->m_windowConfig.FS))
            {
                m_game->setFullScreen();
            }
            
            for (auto& e : m_entityManager.getEntities("NPC"))
            {
                auto& dialogue = e->getComponent<CDialogue>();
                if (e->hasComponent<CDialogue>())
                {
                    std::vector<const char*> list;
                    for (auto& e : dialogue.dialogue_map)
                    {
                        list.push_back(e.first.c_str());
                    }
                    static int item_current = 0;
                    ImGui::Combo(e->name().c_str(), &item_current, list.data(), list.size());
                    dialogue.dialogueState = list[item_current];
                    break;
                }
            }
            ImGui::EndMenu();
        }
        ImGui::SameLine();
        if (ImGui::BeginMenu("Stats"))
        {
            ImGui::Checkbox("Show Stats", &m_drawStats);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar(); // EndMenuBar should be used here
    }


    

}

void OfficeScene::sCamera()
{
    
    auto& playerPos = m_player->getComponent<CTransform>().pos;
    auto& window = m_game->window();

    float distX = playerPos.x - CameraView.getCenter().x;
    float distY = playerPos.y - CameraView.getCenter().y;
    float scale;

    if (smoothCam)
    {
        scale = 0.5f;
    }
    else
    {
        scale = 1.0f;
    }

    CameraView.move(sf::Vector2f(distX * scale, distY*scale));
    //std::cout << CameraView.getCenter().x - CameraView.getSize().x / 2 << "\n";
    if (m_cameraLimit && !(CameraView.getSize().x <= m_worldSize.x * m_gridSize.x || CameraView.getSize().y <= m_worldSize.y * m_gridSize.y))
    {
        if (CameraView.getCenter().x - CameraView.getSize().x / 2 <= 0)
        {
            CameraView.setCenter(CameraView.getSize().x / 2, CameraView.getCenter().y);
        }
        else if (CameraView.getCenter().x + CameraView.getSize().x / 2 >= m_worldSize.x * m_gridSize.x)
        {
            CameraView.setCenter((m_worldSize.x * m_gridSize.x) - CameraView.getSize().x / 2, CameraView.getCenter().y);
        }

        if (CameraView.getCenter().y - (CameraView.getSize().y / 2) <= 0)
        {
            CameraView.setCenter(CameraView.getCenter().x, CameraView.getSize().y / 2);
        }
        else if (CameraView.getCenter().y + (CameraView.getSize().y / 2) >= m_worldSize.y * m_gridSize.y)
        {
            CameraView.setCenter(CameraView.getCenter().x, (m_worldSize.y * m_gridSize.y) - CameraView.getSize().y / 2);
        }
   }

   
    m_rect.left = (CameraView.getCenter().x - (CameraView.getSize().x / 2)) - m_gridSize.x;
    m_rect.top = (CameraView.getCenter().y - (CameraView.getSize().y / 2)) - m_gridSize.y;

}

void OfficeScene::sMovement()
{
    Vec2 playerVelocity(0, 0);

    auto& input = m_player->getComponent<CInput>();
    auto& playerState = m_player->getComponent<CState>().state;
    auto& playerTransform = m_player->getComponent<CTransform>();


    playerTransform.prevPos = playerTransform.pos;
    // Player movement
    
    if (input.up)
    {
        playerState = "RunU";
        playerVelocity.y -= 5;
    }else if (input.down)
    {
        playerVelocity.y += 5;
        playerState = "RunD";
    }
    else if (!input.up && playerState == "RunU")
    {
        playerState = "IdleU";
    }
    else if (!input.down && playerState == "RunD")
    {
        playerState = "IdleD";
    }
    
    if (input.right)
    {
        playerVelocity.x += 5;
        playerState = "RunR";
        playerTransform.scale.x = abs(playerTransform.scale.x);
    }
    else if (input.left) 
    {
        playerVelocity.x -= 5; 
        playerState = "RunL"; 
        playerTransform.scale.x = -abs(playerTransform.scale.x); 
    }
    else if (!input.right && playerState == "RunR")
    {
        playerState = "IdleR";
        playerTransform.scale.x = abs(playerTransform.scale.x);
    }
    else if (!input.left && playerState == "RunL")
    {
        playerState = "IdleL";
        playerTransform.scale.x = -abs(playerTransform.scale.x);
    }
    m_player->getComponent<CTransform>().velocity = playerVelocity;
    

    // Update velocities
    for (auto e : m_entityManager.getEntities())
    {
        e->getComponent<CTransform>().pos +=
            e->getComponent<CTransform>().velocity;
    }
}
/* Helper function */
void OfficeScene::setAnimation(std::shared_ptr<Entity> entity,
                    const std::string& animationName, bool repeat)
{
    // TODO: Maybe this function should be outside the class
    if (entity->getComponent<CAnimation>().animation.getName() != animationName)
    {
        entity->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), repeat);
    }
}

void OfficeScene::sAnimation()
{
    const auto& playerState = m_player->getComponent<CState>().state;
    //std::cout << m_player->hasComponent<CAnimation>() ? "Have Animation\n" : "Not Have Animation\n";
    if (currentState == GameState::Play)
    {
        if (playerState == "RunR")
        {
            setAnimation(m_player, "RunR", true);
        }
        else if (playerState == "RunL")
        {
            setAnimation(m_player, "RunR", true);
        }
        else if (playerState == "RunU")
        {
            setAnimation(m_player, "RunU", true);
        }
        else if (playerState == "RunD")
        {
            setAnimation(m_player, "RunD", true);
        }
        else if (playerState == "IdleR")
        {
            setAnimation(m_player, "IdleR", true);
        }
        else if (playerState == "IdleL")
        {
            setAnimation(m_player, "IdleR", true);
        }
        else if (playerState == "IdleU")
        {
            setAnimation(m_player, "IdleU", true);
        }
        else if (playerState == "IdleD")
        {
            setAnimation(m_player, "IdleD", true);
        }

        /*if (playerState == "RunR")
        {
            setAnimation(m_player, "Emily_runR", true);
        }
        else if (playerState == "RunL")
        {
            setAnimation(m_player, "Emily_runR", true);
        }
        else if (playerState == "RunU")
        {
            setAnimation(m_player, "Emily_runU", true);
        }
        else if (playerState == "RunD")
        {
            setAnimation(m_player, "Emily_runD", true);
        }
        else if (playerState == "IdleR")
        {
            setAnimation(m_player, "Emily_idleR", true);
        }
        else if (playerState == "IdleL")
        {
            setAnimation(m_player, "Emily_idleR", true);
        }
        else if (playerState == "IdleU")
        {
            setAnimation(m_player, "Emily_idleU", true);
        }
        else if (playerState == "IdleD")
        {
            setAnimation(m_player, "Emily_idleD", true);
        }*/
    }
    else if (currentState == GameState::Dialogue)
    {
        if (playerState == "RunR")
        {
            setAnimation(m_player, "IdleR", true);
        }
        else if (playerState == "RunL")
        {
            setAnimation(m_player, "IdleR", true);
        }
        else if (playerState == "RunU")
        {
            setAnimation(m_player, "IdleU", true);
        }
        else if (playerState == "RunD")
        {
            setAnimation(m_player, "IdleD", true);
        }
    }

    for (auto& e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CAnimation>())
        {
            e->getComponent<CAnimation>().animation.update();
        }
    }
}

void OfficeScene::sCollision() 
{
    // TODO: Remove this testing code. Imaginary platform
    auto& playerTransform = m_player->getComponent<CTransform>();
    auto& playerState = m_player->getComponent<CState>().state;

    for (auto& e : m_entityManager.getEntities())
    {
        if (!e->hasComponent<CBoundingBox>() || e == m_player || e->tag() == "Gui") continue;

        if (m_physics.isIntersect(m_player, e))
        {
            playerTransform.pos += playerTransform.velocity * -1;
            break;
        }
        /*auto overLap = m_physics.getOverlap(m_player, e);
        auto prevOverLap = m_physics.getPrevOverlap(m_player, e);
        auto ePos = e->getComponent<CTransform>().pos;


        if (overLap.x > 0 && overLap.y > 0)
        {
            if (prevOverLap.y > 0 && playerTransform.pos.x > ePos.x)
            {
                playerTransform.pos += Vec2(overLap.x, 0);
            }
            else if (prevOverLap.y > 0 && playerTransform.pos.x < ePos.x)
            {
                playerTransform.pos += Vec2(-overLap.x, 0);
            }
            
            else if (prevOverLap.x > 0 && playerTransform.pos.y < ePos.y)
            {
                playerTransform.pos += Vec2(0, -overLap.y);
            }
            else if (prevOverLap.x > 0 && playerTransform.pos.y > ePos.y)
            {
                playerTransform.pos += Vec2(0, overLap.y);
            }
            break;
        }
        */
    }
}

bool OfficeScene::checkCollision(Vec2 ca, Vec2 cb, float ra, float rb)
{
    float dist = ca.dist(cb);
    return dist <= (ra + rb);
}

void OfficeScene::drawLine(const Vec2& p1, const Vec2& p2)
{
    sf::Vertex line[] = { sf::Vector2f(p1.x, p1.y), sf::Vector2f(p2.x, p2.y) };
    m_game->window().draw(line, 2, sf::Lines);
}

void OfficeScene::sDragAndDrop()
{
    for (auto& e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CDraggAble>() && e->getComponent<CDraggAble>().dragging)
        {
            e->getComponent<CTransform>().pos = m_pos;
        }
    }
}
void OfficeScene::sRender()
{
    // TODO: remove this testing code
    auto& window = m_game->window();
    auto date = m_game->getDateSystem();

    m_game->window().setView(CameraView);


    if (!m_paused)
    {
        // color the background darker when paused
        window.clear(sf::Color::Black); 
    }
    else
    {
        window.clear(sf::Color::Black);
    }
    //draw all tiles
    if (m_drawTextures)
    {
        for (auto e : m_entityManager.getEntities("Tile"))
        {
            auto& pos = e->getComponent<CTransform>().pos;
            auto& scale = e->getComponent<CTransform>().scale;
            auto& sprite = e->getComponent<CAnimation>().animation.getSprite();
            auto& size = e->getComponent<CAnimation>().animation.getSize();


            sprite.setPosition(pos.x, pos.y);
            sprite.setScale(scale.x, scale.y);

            if (!m_rect.contains(pos.x, pos.y)) continue;
            if (e->getComponent<CDraggAble>().dragging) continue;

            window.draw(sprite);
        }
    }
    // Set the viewport of the window to be centered on the player if it's far enough right

    /*if (m_drawGrid)
    {
        sf::IntRect rect(CameraView.getCenter().x - CameraView.getSize().x / 2,
            CameraView.getCenter().y - CameraView.getSize().y / 2,
            CameraView.getSize().x,
            CameraView.getSize().y);

        int col = -1, row = -1;
        int maxCol = rect.width / 64, maxRow = rect.height / 64;
        while (col <= maxCol && row <= maxRow)
        {
            int x = (col * 64) + rect.left - (rect.left % 64);
            int y = (row * 64) + rect.top - (rect.top % 64);

            sf::Text text;
            text.setFont(m_game->assets().getFont("Liberty"));
            text.setCharacterSize(20);
            text.setString(std::to_string(x/64) + " : " + std::to_string(y/64));
            text.setFillColor(sf::Color::White);
            text.setPosition(x,y); 

            sf::RectangleShape rect;
            rect.setSize(sf::Vector2f(64, 64));
            rect.setPosition(x, y);
            rect.setFillColor(sf::Color(0, 0, 0, 0));
            rect.setOutlineColor(sf::Color(255, 255, 255, 255));
            rect.setOutlineThickness(0.5f);
            window.draw(rect);
            window.draw(text);

            col++;
            if (col >= maxCol+1)
            {
                col = -1;
                row++;
            }
        }

    }*/
    if (m_drawTextures)
    {
        for (auto e : m_entityManager.getEntities("Wall"))
        {
            auto& pos = e->getComponent<CTransform>().pos;
            auto& scale = e->getComponent<CTransform>().scale;
            auto& sprite = e->getComponent<CAnimation>().animation.getSprite();
            auto& size = e->getComponent<CAnimation>().animation.getSize();


            sprite.setPosition(pos.x, pos.y);
            sprite.setScale(scale.x, scale.y);

            if (!m_rect.contains(pos.x, pos.y)) continue;
            if (e->getComponent<CDraggAble>().dragging) continue;

            window.draw(sprite);
        }
    }
    // draw all entity textures / animations
    auto& entities = m_entityManager.getEntities("Render"); 
    std::sort(entities.begin(), entities.end(), [](std::shared_ptr<Entity>& a, std::shared_ptr<Entity>& b) 
        {
            auto& aCT = a->getComponent<CTransform>();
            auto& aBB = a->getComponent<CAnimation>().animation;

            auto& bCT = b->getComponent<CTransform>();
            auto& bBB = b->getComponent<CAnimation>().animation; 
            return aCT.pos.y + (aBB.getSize().y * aCT.scale.y)/2 < bCT.pos.y + (bBB.getSize().y * bCT.scale.y)/2;
        
        }
    );
    if (m_drawTextures)
    {
        for (auto e : m_entityManager.getEntities("Render"))
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

    // draw bounding box
    if (m_drawCollision)
    {
        for (auto e : m_entityManager.getEntities())
        {
            if (e->hasComponent<CBoundingBox>())
            {
                if (e->tag() == "Gui") continue;
                auto& box = e->getComponent<CBoundingBox>();
                auto& anim = e->getComponent<CAnimation>().animation;
                auto& transform = e->getComponent<CTransform>();
                auto animSize = Vec2(anim.getSize().x * abs(transform.scale.x), anim.getSize().y * abs(transform.scale.y));
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x, box.size.y));
                rect.setPosition((transform.pos.x - animSize.x / 2) + box.offRect.x, (transform.pos.y - animSize.y / 2) + box.offRect.y);
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                rect.setOutlineColor(sf::Color::Red);
                rect.setOutlineThickness(1);
                window.draw(rect);
            }
        }
    }
    m_game->window().setView(window.getDefaultView());

    //draw dialogue box into the screen
    if (currentState == GameState::Dialogue) 
    {
        /*auto& sprite = m_guiObjectMap["Dialogue_Box"].img; 
        auto& pos = m_guiObjectMap["Dialogue_Box"].pos; 
        sprite.setPosition(pos.x, pos.y); 
        window.draw(sprite);*/

        for (auto& e : m_entityManager.getEntities("Gui"))
        {
            auto& anim= e->getComponent<CAnimation>().animation; 
            auto& sprite = anim.getSprite();
            auto& eCT = e->getComponent<CTransform>(); 

            sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2); 
            sprite.setPosition(eCT.pos.x, eCT.pos.y); 

            window.draw(sprite); 

            if (anim.getName() == "dialogue_box")
            {
                for (auto& e : m_entityManager.getEntities("NPC"))
                {
                    auto& cDialog = e->getComponent<CDialogue>();
                    if (cDialog.isOn)
                    {

                        auto& txt = cDialog.dialogue_map[cDialog.dialogueState][cDialog.dialogIndex];
                        //draw dialogue text
                        sf::Text text;
                        text.setFont(m_game->assets().getFont("Times_New_Roman"));
                        text.setString(dl::getDialogueText(txt));
                        text.setFillColor(sf::Color::Black);
                        text.setPosition(sprite.getGlobalBounds().left + 192, sprite.getGlobalBounds().top + 32);

                        window.draw(text);

                        //cDialog.dialogueState = date.getDaysName();
                        //draw potrait
                        //draw emotion in dialogue box
                        sf::Sprite exp;
                        auto& texture = m_game->assets().getTexture(e->name() + dl::getExpression(txt));

                        exp.setTexture(texture);
                        exp.setScale(2.50f, 2.50f);
                        exp.setPosition(sprite.getGlobalBounds().left + 8, sprite.getGlobalBounds().top + 8);
                        window.draw(exp);

                        break;
                    }
                }
            }
        }
    }

    // Draw Stats
    /*
    if (false)
    {
        m_stats.clear();

        //m_stats.push_back("Zoom Scale\t\t : x " + std::to_string(m_zoomScale));
        //m_stats.push_back("Camera Center\t:  " + std::to_string((int)CameraView.getCenter().x) + ", " + std::to_string((int)CameraView.getCenter().y));
        //m_stats.push_back("World Size\t\t  : " + std::to_string((int)m_worldSize.x) + ", " + std::to_string((int)m_worldSize.y) + " [ tiles ]");
        //m_stats.push_back("Player State " + m_player->getComponent<CState>().state);
        m_stats.push_back(formatNumber(date.getDate().day) + " / " + formatNumber(date.getDate().month) + " / " + formatNumber(date.getDate().year));
        m_stats.push_back(formatNumber(date.getDate().hour) + " : " + formatNumber(date.getDate().minute) + " : " + formatNumber(date.getDate().second));
        m_stats.push_back(date.getDaysName());
        m_stats.push_back(currentState == GameState::Dialogue ? "Dialogue State" : "Play State"); 

        if (m_debugMode)
        {
            m_stats.push_back("Debug Mode On");
        }
        else
        {
            m_stats.push_back("Debug Mode Off");
        }
        int row = 0;

        for (auto e : m_stats)
        {
            m_text.setCharacterSize(30);
            m_text.setFont(m_game->assets().getFont("Ostrich_Regular"));
            m_text.setString(e);
            m_text.setFillColor(sf::Color::Red);
            m_text.setOutlineColor(sf::Color::Black);
            m_text.setOutlineThickness(2.0f);
            m_text.setStyle(sf::Text::Style::Bold);
            m_text.setScale(m_zoomScale, m_zoomScale);

            m_text.setPosition(5, (row * (m_text.getGlobalBounds().height + (5*m_zoomScale))));
            window.draw(m_text);
            row++;
        }
    }
    */

    
    sDrawImGui();
    ImGui::SFML::Render(window);
    window.display();
}

Vec2 OfficeScene::windowToWorldPos(Vec2 pos)
{
    int cameraLeft = CameraView.getCenter().x - CameraView.getSize().x / 2;
    int cameraTop = CameraView.getCenter().y - CameraView.getSize().y / 2;

    return Vec2((pos.x + cameraLeft), pos.y + cameraTop);
}

void OfficeScene::screenShot()
{
    std::ifstream fin("res/config/log.ini");
    std::string name, token;
    int index = 0;
    
    fin >> token;

    if (token == "Last_Screenshot")
    {
        fin >> index;
        index++;
        name = "Screenshot" + std::to_string(index) + ".png";
    }
    else
    {
        name = "Screenshot.png";
    }

    auto& window = m_game->window();
    sf::Texture texture;
    texture.create(window.getSize().x, window.getSize().y);
    texture.update(window);
    texture.copyToImage().saveToFile("res/images/screenshot/" + name);

    std::ofstream save;
    save.open("res/config/log.ini");

    save << "Last_Screenshot " + std::to_string(index) + " " + name;
    save.close();
}

void OfficeScene::checkCollisionMouse(Vec2 pos, const std::string& name, Action action)
{
    for (auto& e : m_entityManager.getEntities(name))
    {
        auto& ePos = e->getComponent<CTransform>().pos;
        auto& pPos = m_player->getComponent<CTransform>().pos;
        if (isInside(pos, e) && abs(ePos.x - pPos.x) <= 96 && abs(ePos.y - pPos.y ) <= 96)
        {
            sDoAction(action);

            if (name == "NPC")
            {
                auto& dialogue = e->getComponent<CDialogue>();
                dialogue.isOn = true;
            }
        }
    }
}

void OfficeScene::dragAndDrop(Vec2 pos)
{
    if (m_zoomScale != 1.00000f || !m_debugMode) return;
    for (auto e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CDraggAble>() && isInside(pos, e) && !m_dragging)
        {
            e->getComponent<CDraggAble>().dragging = true;
            m_dragging = true;
            m_lastDragPos = e->getComponent<CTransform>().pos;
            break;
        }
        else if (m_dragging && e->getComponent<CDraggAble>().dragging)
        {
            auto& spriteSize = e->getComponent<CAnimation>().animation.getSize();
            auto& scale = e->getComponent<CTransform>().scale;

            Vec2 size(spriteSize.x * scale.x, spriteSize.y * scale.y);
            //e->getComponent<CTransform>().pos = Vec2((wPos.x - ((int)wPos.x % (int)size.x))+size.x/2, wPos.y - ((int)wPos.y % (int)size.y) + size.y/2);
            e->getComponent<CTransform>().pos = Vec2(pos.x, pos.y);
            e->getComponent<CDraggAble>().dragging = false;
            m_dragging = false;
        }
    }
}
