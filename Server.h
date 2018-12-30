#pragma once

#include "Player.h"
#include "Game.h"

#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

class Server 
{
private:
    string m_name; // server name
    time_t m_lastUpdate;
    map<string, string> m_dns; // player name, accountID
    map<string, Player> m_data; // accoutID, Player obj
    string m_url; //request url
public:
    Server() = default;
    //name of server
    Server(string name) : m_name(name) {}; 
    ~Server() = default;

    void addPlayer(string playerName); //add player by name to map
    void downloadStats(string playerName); //download vector<Game>
};