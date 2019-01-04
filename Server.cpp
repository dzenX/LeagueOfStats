#include "Server.h"

#include "picojson.h"
#include <cpr/cpr.h>

namespace
{

auto apiKey = "RGAPI-4dd4835f-bb9b-4107-989c-3b899c254073";
auto timeOut = std::chrono::milliseconds(2000);

/// Custom exception class with support of std::string
class MyException: public std::exception
{
private:
    std::string m_message;
public:
    explicit MyException(const std::string& message) : m_message(message) {};
    ~MyException() throw() {};
    const char* what() const throw() { return m_message.c_str(); };
    //void addWrap(const std::string& messageTop) { m_message = messageTop + m_message; };
    //void addWrap(const std::string& messageTop, const std::string& messageBot) { m_message = messageTop + m_message + messageBot; };
};

/**
 *  Procces input string into picojson::value object.
 * 
 * @param response String with json-formatted text.
 * @return picojson::value Created picojson::value object.
*/
picojson::value getJson(const string& response)
{
    picojson::value jsonValue;
    picojson::parse(jsonValue, response);
    return jsonValue;
}

/**
 *  Send get request to server.
 * 
 * @param url Full URL of get request include "?api_key=<api-key>" in the end.
 * @param timeout Positive value in miliseconds to wait until request timeout.
 * @return string String with json-formatted text.
*/
string getResponse(const string& request_url, const std::chrono::milliseconds& timeout)
{
    auto response = cpr::Get(cpr::Url{request_url}, cpr::Timeout(timeout));
    //std::cout << response.status_code << std::endl;
    /// status_code == 0 means we got empty respone.text
    // TODO: Do we need check if response.text is empty, so response is truly valid
    // || response.text.empty(). We`ll get runtime in getting parsing json if empty
    if (response.status_code == 200)
        return response.text;
    /// We got invalid response
    switch (response.status_code)
    {
        /// If request timed out.
        case   0:
            throw MyException("Request to url: \"" + response.url + "\" failed.\n"
            "[" + std::to_string(response.status_code) + "] "
            "Timeout set to: " + std::to_string(timeout.count()) + " miliseconds. "
            "Elapsed in: " + std::to_string(response.elapsed));
            break;
        /// If server wont give us access
        case 400:
            throw MyException("Request to url: \"" + response.url + "\" failed.\n"
            "[" + std::to_string(response.status_code) + "] "
            "Forbidden: Wrong api key or url.");
            break;
        default:
            throw MyException("Request to url: \"" + response.url + "\" failed.\n"
            "[" + std::to_string(response.status_code) + "] " + getJson(response.text).get("status").get("message").to_str());
            break;
    }
};

/**
 *  Produse url for get request based on given type and parameter and call "getResponse".
 * 
 *  @param m_url Head of server url to send requests.
 *  @param type Int value to make choise between differnt types of requests.
 *              0 - "summoner/v4/summoners/by-name/" - Get summoner info by name.
 *              1 - "match/v4/matchlists/by-account/" - Get summoner matchlist by accountId.
 *  @param parameter Parameter requered by current type of request.
 *  @param timeout Timeout to pass to "getResponse".
 */
// TODO: Should this function get not a parameter but a vector of parameter dependes on request type
// TODO: Discover LOL api
string getRequest(const string& m_url, const int& type, const string& parameter, const std::chrono::milliseconds& timeout)
{
    /// Produse request url
    /// https://<server name>.api.riotgames.com/lol/
    auto request_url = m_url;
    switch (type)
    {
        /// Get summoner info by name
        case 0:
            request_url += "summoner/v4/summoners/by-name/";
            break;
        /// Get summoner matchlist by accountId
        case 1:
            request_url += "match/v4/matchlists/by-account/";
            break;
        default:
            throw MyException("Request type: " + std::to_string(type) + " failed.\n"
            "U passed wrong type of get request.");
            //std::cout << "U passed wrong type of get request.";
            break;
    }
    request_url += parameter + "?api_key=" + apiKey;
    /// Getting reponse from server with given timeout
    auto response = getResponse(request_url, timeout);
    return response;
}


} /// Anonimous namespace

/**
 *  Function to download player data for given playerName,
 *  create Player object depends on it and add object to container.
 * 
 * @param string Name of player to add.
*/
void Server::addPlayer(string playerName)
{
    /// Declaration of result variable for "getRequest()"
    string response;
    try
    {
        response = getRequest(m_url, 0, playerName, timeOut);
    }
    catch( MyException& error)
    {
        std::cout << "[ERROR] An error occurred while adding player \"" + playerName + "\"." << std::endl;
        std::cout << error.what() << std::endl;
        //throw;
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
    auto accountId = searchPlayerId->second;

    // TODO: Should we check if Player exist in map if he`s exist in m_dns 
    // For now ill accept that he always present if exist in m_dns
    //auto searchPlayer = m_data.find(accountId);
    //auto player = searchPlayer->second;
    auto player = m_data.find(accountId)->second;

    /// Declaration of result variable for "getRequest()"
    string response;
    try
    {
        response = getRequest(m_url, 1, accountId, timeOut);
    }
    catch ( MyException& error )
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
    for(auto i = matchesArray.begin(); i != matchesArray.end(); i++)
    {
        /// Get picojson::object form current match picojson::value
        auto matchObject = i->get<picojson::object>();
        /// Declaration of map to pass in Game object
        map<string, string> gameMap;
        std::cout << matchObject["queue"] << std::endl;
        /// For each parameter from match picojson::object
        for(auto j = matchObject.begin(); j != matchObject.end(); j++)
        {
            /// Form map of match parameters
            gameMap.insert(std::make_pair(j->first,j->second.to_str()));
            //std::cout << j->first << " : " << j->second.to_str() << std::endl;
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