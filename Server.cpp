#include "Server.h"

#include "picojson.h"
#include <cpr/cpr.h>

string apiKey = "RGAPI-22c37a55-b8bd-4f20-9f44-dac1b550080e";

void Server::addPlayer(string playerName)
{
    auto response = cpr::Get(cpr::Url{"https://" + m_name + ".api.riotgames.com/lol/summoner/v4/summoners/by-name/" + playerName + "?api_key=" + apiKey});
    picojson::value v;
    auto json = picojson::parse(v, response.text);

    string accountId = v.get("accountId").to_str();

    Player player(playerName, accountId, {});

    m_dns.insert(std::make_pair(playerName, accountId));
    m_data.insert(std::make_pair(accountId, player));

    std::cout << accountId << std::endl;
}