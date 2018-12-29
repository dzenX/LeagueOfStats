#pragma once

#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;

class Game{
private:
    int m_gameId;
    map<string, string> m_data;
public:
    Game();
    //TODO: make <string,string>* args
    Game(map<string, string> data):m_data(data) {};
    ~Game();

    map<string, string> getGameData() { return m_data;};
};