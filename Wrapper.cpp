#include "Wrapper.h"

#include "Utils.h"

#include <cpr/cpr.h>
#include "picojson.h"


/**
 *  Function to send get request to given url using cpr::Get
 *  it will throw MyException if request timeout or got wrong response
 * 
 *  @param url String with url to send request to
 *  @return string String with response in json format 
 */ 
string Wrapper::get(string url)
{
    /// Send get request using cpr::Get
    auto response = cpr::Get(cpr::Url{url}, cpr::Timeout(m_timeout));
    /// status_code == 200 means OK
    if (response.status_code == 200)
        return response.text;
    /// We got invalid response
    switch (response.status_code)
    {
        /// If request timed out.
        case   0:
            throw std::runtime_error("Wrapper::get error:\n"
            "Request to url: \"" + response.url + "\" failed.\n"
            "[" + std::to_string(response.status_code) + "] "
            "Timeout set to: " + std::to_string(m_timeout.count()) + " milliseconds. "
            "Elapsed in: " + std::to_string(response.elapsed));
        /// If server wont give us access
        case 403:
            throw std::runtime_error("Wrapper::get error:\n"
            "Request to url: \"" + response.url + "\" failed.\n"
            "[" + std::to_string(response.status_code) + "] "
            "Forbidden: Wrong api key or url.");
        /// If we got another server error
        default:
            throw std::runtime_error("Wrapper::get error:\n"
            "Request to url: \"" + response.url + "\" failed.\n"
            "[" + std::to_string(response.status_code) + "] " + getJson(response.text).get("status").get("message").to_str());
    }
};

/**
 *  Function to request summoner info from api
 * 
 * @param serverName String with name of server where summoner is.
 * @param summonerName Path parameter.
 * @param query_parameters Map with addional parameters of request.
 * @return string String with response from server.
 */
string Wrapper::getSummonerInfoByName(const string& serverName, const string& summonerName, const std::map<string, string>& query_parameters)
{
    /// Form url with needed path parameters and add apikey
    string url = "https://" + serverName + '.' + m_apiUrl + "/lol/summoner/v4/summoners/by-name/" + summonerName + "?api_key=" + m_apiKey;
    /// Adding all passed query parameters to request url like "&param=<some-val>"
    for (auto it = query_parameters.begin(); it != query_parameters.end(); ++it)
        url += '&' + it->first + '=' + it->second;
    return get(url);
};

/**
 *  Function to request matchlist of given account from api
 * 
 * @param serverName String with name of server where summoner is.
 * @param accountId Path parameter.
 * @param query_parameters Map with addional parameters of request.
 * @return string String with response from server.
 */
string Wrapper::getMatchlistsByAccountId(const string& serverName, const string& accountId, const std::map<string, string>& query_parameters)
{
    /// Form url with needed path parameters and add apikey
    string url = "https://" + serverName + '.' + m_apiUrl + "/lol/match/v4/matchlists/by-account/" + accountId + "?api_key=" + m_apiKey;
    /// Adding all passed query parameters to request url like "&param=<some-val>"
    for (auto it = query_parameters.begin(); it != query_parameters.end(); ++it)
        url += '&' + it->first + '=' + it->second;
    return get(url);
};

/**
 *  Function to request match info by matchId from api
 * 
 * @param serverName String with name of server where summoner is.
 * @param matchId Path parameter.
 * @param query_parameters Map with addional parameters of request.
 * @return string String with response from server.
 */
string Wrapper::getMatchByMatchId(const string& serverName, const string& matchId, const std::map<string, string>& query_parameters)
{
    /// Form url with needed path parameters and add apikey
    string url = "https://" + serverName + '.' + m_apiUrl + "/lol/match/v4/matches/" + matchId + "?api_key=" + m_apiKey;
    /// Adding all passed query parameters to request url like "&param=<some-val>"
    for (auto it = query_parameters.begin(); it != query_parameters.end(); ++it)
        url += '&' + it->first + '=' + it->second;
    return get(url);
};

/**
 *  Function to request match timelines by matchId from api
 * 
 * @param serverName String with name of server where summoner is.
 * @param matchId Path parameter.
 * @param query_parameters Map with addional parameters of request.
 * @return string String with response from server.
 */
string Wrapper::getTimelinesByMatchId(const string& serverName, const string& matchId, const std::map<string, string>& query_parameters)
{
    /// Form url with needed path parameters and add apikey
    string url = "https://" + serverName + '.' + m_apiUrl + "/lol/match/v4/timelines/by-match/" + matchId + "?api_key=" + m_apiKey;
    /// Adding all passed query parameters to request url like "&param=<some-val>"
    for (auto it = query_parameters.begin(); it != query_parameters.end(); ++it)
        url += '&' + it->first + '=' + it->second;
    return get(url);
};