#pragma once

#include <map>
#include <string>
#include <chrono> /// To use std::chrono::milliseconds

using std::string;

class Wrapper
{
private:
    static constexpr const char* m_apiUrl = "api.riotgames.com";

    /// Function to get a response from the server by sending get request
    static string get(const string& url, const std::chrono::milliseconds& timeout);
public:
    /// Bunch of interface functions to form request url in different ways and
    /// send get requests using get()
    /// For each function setted up a default value of timeout im milliseconds
    static string getSummonerInfoByName(const string& serverName,
                                        const string& summonerName,
                                        const std::map<string, string>& queryParameters,
                                        const std::chrono::milliseconds& timeout = std::chrono::milliseconds(5000));
    static string getMatchlistsByAccountId(const string& serverName,
                                           const string& accountId,
                                           const std::map<string, string>& queryParameters,
                                           const std::chrono::milliseconds& timeout = std::chrono::milliseconds(5000));
    static string getMatchByMatchId(const string& serverName,
                                    const string& matchId,
                                    const std::map<string, string>& query_parameters,
                                    const std::chrono::milliseconds& timeout = std::chrono::milliseconds(5000));
    static string getTimelinesByMatchId(const string& serverName,
                                        const string& matchId,
                                        const std::map<string, string>& query_parameters,
                                        const std::chrono::milliseconds& timeout = std::chrono::milliseconds(5000));
};