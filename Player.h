#pragma once

#include "Game.h"

#include <vector>

using std::vector;
using std::string;

class Player{
private:
    string m_name, m_accId;
    vector<Game> m_games;
public:
    Player() = default;
    Player(const string& name, const string& accID, const vector<Game>& games)
        : m_name(name), m_accId(accID), m_games(games) {};
    ~Player() = default;  

    void addGame(Game new_game) { m_games.push_back(new_game); }; 
};