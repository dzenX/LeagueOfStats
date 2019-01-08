#include "Server.h"

#include "Utils.h"

#include "picojson.h"
#include <cpr/cpr.h>

/**
 *  Function to download player data for given playerName,
 *  create Player object depends on it and add object to container.
 * 
 * @param string Name of player to add.
*/
void Server::addPlayer(string playerName)
{
    /// Form path parameters for get request
    std::map<string, string> pathParams;
    pathParams.insert(std::make_pair("playerName", playerName));
    /// Declaration of result variable for "getRequest()"
    string response;
    try
    {
        response = m_wrap.getSummonerInfoByName(m_name, playerName, {});
    }
    catch( std::runtime_error& error)
    {
        std::cout << "[ERROR] An error occurred while adding player \"" + playerName + "\"." << std::endl;
        std::cout << error.what() << std::endl;
        return;
    }

    /// Parsing response to picojson::value object
    auto jsonValue = getJson(response);
    /// Getting accountId value from json formated without ""
    auto accountId = jsonValue.get("accountId").to_str();

    /// Player object creation
    Player player(playerName, accountId, {});
    /// Saving relation between playerName and accountId to std::map
    m_dns.insert(std::make_pair(playerName, accountId));
    /// Saving Player object to std::map using accountId
    m_data.insert(std::make_pair(accountId, player));

    std::cout << "Player \"" << playerName << "\" succesfully added." << std::endl;
}

/**
 *  Function to download games data, produse Game objects 
 *  and pass it to given player.
 * 
 *  @param string Name of player to process.
*/
void Server::downloadStats(string playerName)
{
    auto searchPlayerId = m_dns.find(playerName);
    // TODO: Idk if we need this check cause we still have no interface
    if (searchPlayerId == m_dns.end())
        return;
    /// Getting Player object to save downloaded games in
    auto accountId = searchPlayerId->second;
    auto player = m_data.find(accountId)->second;
    /// Form path parameters for get request
    std::map<string, string> pathParams;
    pathParams.insert(std::make_pair("accountId", accountId));
    /// Declaration of result variable for get request
    string response;
    try
    {
        response = m_wrap.getMatchlistsByAccountId(m_name, accountId, {});
    }
    catch ( std::runtime_error& error )
    {
        std::cout << "An error occurred while downloading stats for \"" + playerName + "\" :" << std::endl;
        std::cout << error.what() << std::endl;
        return;
    }
    /// Getting picojson::value from response
    auto jsonValue = getJson(response);
    /// Transform it to picojson::object
    auto jsonObject = jsonValue.get<picojson::object>();
    /// Get picojson::array of matches form our jsonObject
    auto matchesArray =  jsonObject["matches"].get<picojson::array>();
    /// For each "match" as picojson::value
    for (auto it = matchesArray.begin(); it != matchesArray.end(); ++it)
    {
        /// Get picojson::object form current match picojson::value
        auto matchObject = it->get<picojson::object>();
        /// Declaration of map to pass in Game object
        map<string, string> gameMap;
        //std::cout << matchObject["queue"] << std::endl;
        /// For each parameter from match picojson::object
        for (auto jt = matchObject.begin(); jt != matchObject.end(); ++jt)
        {
            /// Form map of match parameters
            gameMap.insert(std::make_pair(jt->first,jt->second.to_str()));
        }
        /// Create of Game object and pass gameMap to it
        auto game  = Game(gameMap);
        /// Setting m_gameId of created Game object from json
        game.setGameId(matchObject["gameId"].to_str());
        /// Adding Game object to player(saving in m_games by .push_back())
        player.addGame(game);
    }
    std::cout << "For player \"" << playerName << "\" downloaded "
    << std::stoi(jsonValue.get("totalGames").to_str()) << " games." << std::endl;
}