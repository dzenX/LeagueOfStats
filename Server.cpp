#include "Server.h"

#include "picojson.h"
#include <cpr/cpr.h>

namespace
{

string apiKey = "RGAPI-4ce6cdcd-aa7a-4f78-8c61-d43d9355375a";
int32_t timeOut = 1000;

/// Custom exception struct with support of std::string
struct MyException : public std::exception
{
    string error_message;
    MyException(std::string message) : error_message(message) { }
    ~MyException() throw() {}
    const char* what() const throw() { return error_message.c_str(); }
};

/**
 *  Send get request to server
 * 
 * @param url Full URL of get request include "?api_key=<api-key>" in the end.
 * @param timeout Positive value in miliseconds to wait until request timeout.
 * @return string String with json-formatted text.
*/
string getResponse(const string& request_url, const int32_t& timeout)
{
    cpr::Response response = cpr::Get(cpr::Url{request_url}, cpr::Timeout(timeout));
    //std::cout << response.status_code << std::endl;
    /// status_code == 0 means we got empty respone.text
    // TODO: Do we need check if response.text is empty, so response is truly valid
    // || response.text.empty(). We`ll get runtime in getting parsing json if empty
    
    if (response.status_code == 200)
        return response.text;
    /// We got invalid response
    string error_str = "Request to url: \"" + response.url + "\" failed.\n";
    error_str += '[' + std::to_string(response.status_code) + "] ";
    switch (response.status_code)
    {
        /// If request timed out.
        case   0:
            error_str += "Timeout set to: " + std::to_string(timeout) + " miliseconds. ";
            error_str += "Elapsed in: " + std::to_string(response.elapsed);
            break;
        /// If server wont give us access
        case 403:
            error_str += "Forbidden: Wrong api key.";
            break;
    }   
    throw MyException(error_str);
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

string getRequest(const string& m_url, const int& type, const string& parameter, const int32_t& timeout)
{
    /// Produse request url
    /// https://<server name>.api.riotgames.com/lol/
    string request_url = m_url;
    
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
            std::cout << "U passed wrong type of get request.";
            break;
    }

    request_url = request_url + parameter + + "?api_key=" + apiKey;

    /// Declaration of result variable for get request
    string response;
    try
    {
        /// Getting reponse from server with given timeout
        response = getResponse(request_url, timeout); 
    }
    /// If we got invalid response
    catch ( MyException& )
    {
        throw;
    }
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
    picojson::value jsonValue = getJson(response);
    /// Getting accountId value from json formated without ""
    string accountId = jsonValue.get("accountId").to_str();

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
    string accountId = searchPlayerId->second;
    
    // TODO: Should we check if Player exist in map if he`s exist in m_dns 
    // For now ill accept that he always present if exist in m_dns
    auto searchPlayer = m_data.find(accountId);
    Player player = searchPlayer->second;


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

    picojson::value jsonValue = getJson(response);
    picojson::object jsonObject = jsonValue.get<picojson::object>();

    picojson::array jsonArray =  jsonObject["matches"].get<picojson::array>();
    std::cout << "Last game champion id: " << jsonArray[0].get("champion") << std::endl;

    
    
    // picojson::array list =jsonValue.get("matches").get<picojson::array>();
    // for  (auto i = list.begin(); i!= list.end(); i++)
    //     std::cout << i << std::endl;
    // //jsonValue = jsonValue.get("matches");
    int totalGames = std::atoi(jsonValue.get("totalGames").to_str().c_str());
    std::cout << "Total games downloaded: " << totalGames << std::endl;
}

// /**
//  *  Function to get accountId of Player saved in Server by his name
//  * 
//  *  @param string Name of player to process.
// */
// string Server::m_getAccIddByName(string playerName)
// {
//     auto search = m_dns.find(playerName);
//     if (search == m_dns.end())
//         throw MyException("Player \"" + playerName + "\" not found.");
//     return search->second;
// }