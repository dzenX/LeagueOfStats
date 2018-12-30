#include "Server.h"

#include "picojson.h"
#include <cpr/cpr.h>

namespace
{

string apiKey = "RGAPI-04c82482-8b8f-44bd-abe0-864bcb58dedd";

using std::endl;
using std::cout;

typedef struct
{
    string message;
    int code;
} error_type;


/**
 *  Send get request to server
 * 
 * @param url Full URL of get request include "?api_key=<api-key>" in the end
 * @return picojson::value object parsed from request or error
*/
picojson::value getJsonByURL(const string request_url)
{   
    /// Sending get request    
    auto response = cpr::Get(cpr::Url{request_url});
    /// If we got smth wrong from request
    if (response.status_code != 200)
    {
        error_type error;
        error.message = "Wrong request:\n" + response.text;
        error.code = response.status_code;
        throw error;
    }
    /// Variable with json parse result
    picojson::value jsonValue;
    /// Parsing responce json
    string jsonError = picojson::parse(jsonValue, response.text);
    /// If parse error occurred
    /// League API is strange thing... I guess we should be prepared for all possible shit
    if (!jsonError.empty())
    {
        error_type error;
        error.message = "Json parser error:\n\t" + jsonError + '\n';
        error.code = -1;
        throw error;
    }
    return jsonValue;
};
}

void Server::addPlayer(string playerName)
{  
    // TODO: Should we add api key in function or here? This string is too long
    /// form url of get request
    string url = m_url + "summoner/v4/summoners/by-name/" + playerName + "?api_key=" + apiKey;

    picojson::value jsonValue;
    try
    {
        /// send get request to server and parse result to json
        jsonValue = getJsonByURL(url);
    }
    catch (const error_type error)
    {
        cout << "Error with code [" + std::to_string(error.code) + "] occurred!" << endl;
        cout << error.message << endl;
        return;
    }
    /// get accountid value without ""
    string accountId = jsonValue.get("accountId").to_str();

    /// create Player object and save it to container
    Player player(playerName, accountId, {});
    m_dns.insert(std::make_pair(playerName, accountId));
    m_data.insert(std::make_pair(accountId, player));

    std::cout << accountId << std::endl;
}