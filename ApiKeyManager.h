#pragma once

#include <string>
#include <chrono>
#include <thread>

class ApiKeyManager
{
private:
    std::string m_apiKey; ///< Actually api key
    std::chrono::hours m_updateTime; ///< Frequency of api key updates
    std::thread m_thread;

    // TODO: Implement and change param list 
    void getNewApiKey(); ///< Function to uptade current api key
public:
    /// Constructor with default value of api key update time
    explicit ApiKeyManager(const std::chrono::hours& updateTime);
    ~ApiKeyManager() =  default;

    /// Function to changed frequency of api key updating
    void setUpdateTime(const std::chrono::hours& updateTime) { m_updateTime = updateTime; };
    /// Function to get current update frequency
    std::chrono::hours getUpdateTime() { return m_updateTime; };

    /// Function to get current api key
    std::string getApiKey() { return m_apiKey; };
};