#include "Server.h"

#include "picojson.h"
#include <cpr/cpr.h>

namespace
{

auto apiKey = "RGAPI-4ce6cdcd-aa7a-4f78-8c61-d43d9355375a";
auto timeOut = std::chrono::milliseconds(1000);

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
 *  Send get request to server
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
        case 403:
            throw MyException("Request to url: \"" + response.url + "\" failed.\n"
            "[" + std::to_string(response.status_code) + "] "
            "Forbidden: Wrong api key or url.");
            break;
    }   
    
};

/**
 *  Procces input string into picojson::value object
 * 
 * @param response String with json-formatted text
 * @return picojson::value Created picojson::value object
*/
picojson::value getJson(const string& response)
{
    picojson::value jsonValue;
    picojson::parse(jsonValue, response);
    return jsonValue;
}

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
        /// Get summoner match-list by accountId
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
 *  Function to create Player object for given playerName
 *  and add it to container.
 * 
 * @param string Name of player to add.
*/
void Server::addPlayer(string playerName)
{  
    /// Declaration of result variable for get request
    string response;
    try
    {
        response = getRequest(m_url, 0, playerName, timeOut);
    }  
    catch( MyException& error)
    {
        std::cout << "[ERROR] An error occurred while adding player." << std::endl;
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
 *  Function to download and form std::vector of Game objects
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


    string response;
    try
    {
        response = getRequest(m_url, 1, accountId, timeOut);
    } 
    catch ( MyException& error )
    {
        std::cout << "An error occurred while downloading stats:" << std::endl;
        std::cout << error.what() << std::endl;
        return;
    }

    auto jsonValue = getJson(response);
    auto jsonObject = jsonValue.get<picojson::object>();

    auto jsonArray =  jsonObject["matches"].get<picojson::array>();
    std::cout << "Last game champion id: " << jsonArray[0].get("champion") << std::endl;

    
    
    // picojson::array list =jsonValue.get("matches").get<picojson::array>();
    // for  (auto i = list.begin(); i!= list.end(); i++)
    //     std::cout << i << std::endl;
    // //jsonValue = jsonValue.get("matches");
    auto totalGames = std::atoi(jsonValue.get("totalGames").to_str().c_str());
    std::cout << "Total games downloaded: " << totalGames << std::endl;
}