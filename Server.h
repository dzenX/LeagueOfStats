#pragma once

#include "Player.h"
#include "Game.h"

#include <map>
#include <string>

using std::map;
using std::string;

class Server 
{
private:
    string m_name; ///< Server name
    time_t m_lastUpdate;
    map<string, string> m_dns; ///< map of <Player name, accountID>
    map<string, Player> m_data; ///< map of <accoutId, Player obj>
public:
    explicit Server(const string& serverName) : m_name(serverName) {};
    ~Server() = default;

    void addPlayer(const string& playerName); ///< Add player by name to map
    void downloadStats(const string& playerName); ///< Download vector<Game>
};