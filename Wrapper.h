#pragma once

#include <map>
#include <string>
#include <chrono> /// To use std::chrono::milliseconds

using std::string;

class Wrapper
{
private:
    const string m_apiUrl = "api.riotgames.com";
    string m_apiKey; ///< String to store current apiKey
    std::chrono::milliseconds m_timeout; ///< Variable to store duration of timeout for requests

    string get(string url); ///> Function to response from the server by sending get request
public:
    /// Constructor with deafault timeout value set to 2000 milliseends
    Wrapper(const string& apiKey)
        : Wrapper{apiKey, std::chrono::milliseconds(2000)} {};
    /// Constructor to set custom timeout by passing unsigned int value
    Wrapper(const string& apiKey, const unsigned int& timeout)
        : Wrapper{apiKey, std::chrono::milliseconds(timeout)} {};
    Wrapper(const string& apiKey, const std::chrono::milliseconds& timeout) :
            m_apiKey(apiKey),
            m_timeout(timeout) {};
    ~Wrapper() = default;

    /// Functions to change timeout in runtime
    void setTimeout(const std::chrono::milliseconds& milliseconds) { m_timeout = milliseconds; }
    void setTimeout(const unsigned int& milliseconds) { m_timeout = std::chrono::milliseconds(milliseconds); };
    /// Function to get current timeout value
    std::chrono::milliseconds getTimeout() { return m_timeout; };
    /// Funtion to change apiKey in runtime
    void setApiKey(const string& apiKey){ m_apiKey = apiKey; };
    /// Bunch of interface functions to form request url in different ways and
    /// send get requests using get()
    string getSummonerInfoByName(const string& serverName, const string& summonerName, const std::map<string, string>& query_parameters);
    string getMatchlistsByAccountId(const string& serverName, const string& accountId, const std::map<string, string>& query_parameters);
    string getMatchByMatchId(const string& serverName, const string& matchId, const std::map<string, string>& query_parameters);
    string getTimelinesByMatchId(const string& serverName, const string& matchId, const std::map<string, string>& query_parameters);
};