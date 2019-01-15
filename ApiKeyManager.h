#pragma once

#include <string>
#include <chrono>

class ApiKeyManager
{
private:
    std::string m_apiKey; ///< Actually api key
    std::chrono::hours m_updateTime; ///< Frequency of api key updates

    // TODO: Implement and change param list 
    void getNewApiKey(); ///< Function to uptade current api key
public:
    /// Constructor with default value of api key update time
    ApiKeyManager(const std::chrono::hours& updateTime = std::chrono::hours(12)) : m_updateTime(updateTime) {};
    ~ApiKeyManager() =  default;

    void mainThread(); ///< Function to update api key every X hours

    /// Function to changed frequency of api key updating
    void setUpdateTime(const std::chrono::hours& updateTime) { m_updateTime = updateTime; };
    /// Function to get current update frequency
    std::chrono::hours getUpdateTime() { return m_updateTime; };

    /// Function to get current api key
    std::string getApiKey() { return m_apiKey; };
};