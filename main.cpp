#include "Server.h"
#include "Wrapper.h"

#include <iostream>


int main(int argc, char** argv) {

    auto wrap = Wrapper(std::string("RGAPI-6326bcd7-4230-4857-898b-24b3a1573102"));
    Server s(std::string("euw1"), wrap);  
    s.addPlayer(std::string("qwellow"));
    s.downloadStats(std::string("qwellow"));
    //auto wrap = Wrapper("apikey", "euw1", 1000);
    //std::cout << wrap.getSummonerInfoByName("qwellow");

    return 0;
}
