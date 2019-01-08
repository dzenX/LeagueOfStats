#pragma once

#include "Player.h"
#include "Game.h"
#include "Wrapper.h"

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
    Wrapper m_wrap; ///< Current wrapper for requests
public:
    Server() = default;
    //name of server
    Server(string name, Wrapper wrap) : m_name(name), m_wrap(wrap) {};
    ~Server() = default;

    void addPlayer(string playerName); ///< Add player by name to map
    void downloadStats(string playerName); ///< Download vector<Game>
};