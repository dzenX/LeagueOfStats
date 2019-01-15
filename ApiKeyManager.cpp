#include "ApiKeyManager.h"

#include <thread>

namespace
{
    auto apiKey = "<api-key>"; // Temp variable to simulate downloading of new api key
}

// TODO: Make comments after implementati
void ApiKeyManager::getNewApiKey()
{
    m_apiKey = apiKey;
};

/**
 *  Function to execute update of api key with setted frequency.
 */
void ApiKeyManager::mainThread()
{
    while(true)
    {
        getNewApiKey();
        std::this_thread::sleep_for(m_updateTime);
    }
};