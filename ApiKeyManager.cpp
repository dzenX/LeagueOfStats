#include "ApiKeyManager.h"

#include <thread>

namespace
{
    auto apiKey = "<api-key>"; // Temp variable to simulate downloading of new api key
}

ApiKeyManager::ApiKeyManager(const std::chrono::hours& updateTime)
    : m_updateTime(updateTime),
      m_thread([this]()
               {
                    /// Function to execute update of api key with setted frequency.
                    while(true)
                    {
                        getNewApiKey();
                        std::this_thread::sleep_for(m_updateTime);
                    }
               })
    {
    };

// TODO: Make comments after implementati
void ApiKeyManager::getNewApiKey()
{
    m_apiKey = apiKey;
};