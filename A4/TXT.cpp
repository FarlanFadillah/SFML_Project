#include "TXT.hpp" // Include the header file


namespace ln
{
    int getIntFromString(std::string& line, std::string hint, int digit)
    {
        int x = 0;
        std::size_t xPos = line.find(hint);
        if (xPos != std::string::npos) {
            x = std::stoi(line.substr(xPos + hint.length(), digit));
        }
        return x;
    }

    std::string getValueFromString(std::string& line, std::string start, std::string end)
    {
        std::size_t sPos = line.find(start);
        std::size_t ePos = line.find(end);
        if (sPos != std::string::npos && ePos != std::string::npos) {
            return line.substr(sPos + start.length(), ePos - (sPos + start.length()));
        }
        return "";
    }

    std::string removeSpacesAndNewlines(std::string str)
    {
        str.erase(std::remove_if(str.begin(), str.end(),
            [](unsigned char c) { return c == ' ' || c == '\n'; }), str.end());
        return str;
    }

    bool isWhitespaceOnly(const std::string& str)
    {
        return std::all_of(str.begin(), str.end(), [](unsigned char c) { return std::isspace(c); });
    }
}

namespace dl
{
    std::vector<std::string> getDialogueLine(std::string line)
    {
        std::stringstream test(line);
        std::string segment;
        std::vector<std::string> seglist;
        while (std::getline(test, segment, '*'))
        {
            seglist.push_back(segment);
        }
        return seglist;
    }
    std::map<std::string, std::vector<std::string>> setupdialogue(std::string path)
    {
        std::map<std::string, std::vector<std::string>> result;
        std::ifstream in(path);

        Json::Value val;
        Json::Reader reader;

        reader.parse(in, val);
        auto dialogue = val["contents"];

        for (Json::Value::const_iterator itr = dialogue.begin(); itr != dialogue.end(); ++itr)
        {
            const Json::Value& val = *itr;
            for (auto& txt : getDialogueLine(val.asString()))
            {
                result[itr.key().asString()].push_back(txt);
            }
        }
        return result;
    }
    void getDialogueInterviewScript(const std::string& path, DialogueScript& dialogueScript, const Assets& assets)
    {

        std::ifstream in(path);

        Json::Value val;
        Json::Reader reader;

        reader.parse(in, val);

        dialogueScript.title = val["scene"].asString();
        dialogueScript.npc = val["npc"].asString();
        auto& settings = val["settings"];
        auto& c = settings["font_color"];

        dialogueScript.text.setFont(assets.getFont(settings["font_name"].asString()));
        dialogueScript.text.setFillColor(sf::Color(c[0].asInt(), c[1].asInt(), c[2].asInt()));
        dialogueScript.text.setCharacterSize(settings["font_size"].asFloat());

        auto& dialogueArr = val["dialogue"];

        for (auto& item : dialogueArr)
        {
            DialogueEntry temp;

            temp.id = item["id"].asInt();
            temp.text = item["text"].asString();
            temp.npc_response = item["npc_response"].asString();
            temp.hasOption = item["has_choices"].asBool();
            if (temp.hasOption)
            {
                for (auto& choices : item["choices"])
                {
                    temp.option[choices["next"].asInt()] = choices["text"].asString();
                }
            }
            else {
                temp.next = item["next"].asInt();
            }
            dialogueScript.dialogue.push_back(temp);
        }

        dialogueScript.line = dialogueScript.dialogue[0].text;

    }
    std::string getDialogueText(std::string line)
    {
        return line.substr(0, line.find('%'));
    }
    std::string getExpression(std::string line)
    {
        int expIndex = line.find('%');
        int el = line.find('^');
        return line.substr(expIndex, el - expIndex);
    }
}

namespace emt
{
    void loadEntityPotrait(std::string path, const Assets& assets, std::map<std::string, sf::Texture>& exp_map)
    {
        std::ifstream in(path);
        std::string token, name, texture_name;
        while (in.good())
        {
            in >> token;
            if (token == "Emotion")
            {
                in >> texture_name;
                exp_map[texture_name] = assets.getTexture(texture_name);
            }
        }
    }
}
