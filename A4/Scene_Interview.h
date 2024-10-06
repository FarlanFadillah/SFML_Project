#pragma once
#include "Scene.hpp"
#include "SFML/Graphics.hpp"
#include "Scene_Menu.hpp"

class SceneInterview :
    public Scene
{
private:
	DialogueScript m_ds;
	EntityManager m_entityManager;
	bool typing = false;
	bool m_drawStat = false;

	void init(std::string path, const std::string& dialogueScript);
	void update()                        override;
	void onEnd()                         override;
	void sRender()                       override;
	void sDoAction(const Action& action) override;
	void sDrawImGui()					 override;
	void proceedDialogueInterview(DialogueScript& ds);
	void typeDialogueLineByLetter(DialogueScript& ds);
	void skipTypingDialogeLine(DialogueScript& ds);

public:
	SceneInterview(GameEngine* gameEngine, std::string path, const std::string& dialogueScript);
};

