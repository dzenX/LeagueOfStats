#include "Server.h"

#include "picojson.h"
#include <cpr/cpr.h>

string apiKey = "RGAPI-2232dce3-0542-44b7-9bbf-00ae436778dd";

void Server::addPlayer(string playerName)
{
    auto response = cpr::Get(cpr::Url{"https://" + m_name + ".api.riotgames.com/lol/summoner/v4/summoners/by-name/" + playerName + "?api_key=" + apiKey});
    picojson::value v;
    auto json = picojson::parse(v, response.text);
    std::cout << v.serialize() << std::endl;
}