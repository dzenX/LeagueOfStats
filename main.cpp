#include "Server.h"

#include <iostream>

int main(int argc, char** argv) {
    Server s(std::string("euw1"));  
    s.addPlayer(std::string("qwellow"));
    s.downloadStats(std::string("qwellow"));
    //auto response = cpr::Get(cpr::Url{"https://httpbin.org/get"});
    //auto json = nlohmann::json::parse(response.text);
    //std::cout << json.dump(4) << std::endl;
    return 0;
}
