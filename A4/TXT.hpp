#ifndef TXT_H
#define TXT_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <Assets.hpp>
#include <json/json.h>
#include <fstream>

class DialogueEntry
{
public:
    int id = 0;                         // id of dialogue
    std::string text = "";              // the player line
    std::string npc_response = "";      // npc line
    int next = 0;                       // pointer to the next index of dialogue
    bool hasOption = false;             // if the dialogue has option
    std::map<int, std::string> option;  // map of dialogue option
    DialogueEntry()
    {

    }
};


class DialogueScript {
public:
    sf::Text text;                          // rendered text from sfml
    int i = 0;                              // index of dialogues
    int t = 0;                              // typing lenght
    int state = 0;                          // state dialogue, 0 is the player line, 1 is the npc line
    std::string title;                      // the title of the conversation/ interview
    std::string npc;                        // the name of npc
    std::string line;                       // line to show
    std::string typing = "";                // string to render, taken from line letter by letter or can skip and get the entire line
    std::vector<DialogueEntry> dialogue;    // vector of dialogue
    DialogueScript()
    {

    }
};


namespace ln
{
    int getIntFromString(std::string& line, std::string hint, int digit);
    std::string getValueFromString(std::string& line, std::string start, std::string end);
    std::string removeSpacesAndNewlines(std::string str);
    bool isWhitespaceOnly(const std::string& str);
}

namespace dl
{
    std::vector<std::string> getDialogueLine(std::string line);
    std::map<std::string, std::vector<std::string>> setupdialogue(std::string path);
    void getDialogueInterviewScript(const std::string& path, DialogueScript& dialogueScript, const Assets& assets);
    std::string getDialogueText(std::string line);
    std::string getExpression(std::string line);
}

namespace emt
{
    void loadEntityPotrait(std::string path, const Assets& assets, std::map<std::string, sf::Texture>& exp_map);
}

#endif // TXT_H
