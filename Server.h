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
    string m_name; ///< Server name
    time_t m_lastUpdate;
    map<string, string> m_dns; ///< map of <Player name, accountID>
    map<string, Player> m_data; ///< map of <accoutId, Player obj>
    string m_url; ///< Request url

    //string m_getAccIddByName(string playerName); ///<
public:
    Server() = default;
    //name of server
    Server(string name) : m_name(name), m_url("https://" + name + ".api.riotgames.com/lol/") {}; 
    ~Server() = default;

    void addPlayer(string playerName); ///< Add player by name to map
    void downloadStats(string playerName); ///< Download vector<Game>
};