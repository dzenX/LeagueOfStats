#include "Server.h"
#include "ApiKeyManager.h"

#include <iostream>

int main(int argc, char** argv) {
    Server s("euw1"); 
    using namespace std::string_literals; 
    s.addPlayer("qwellow"s);
    s.downloadStats("qwellow"s);
    //ApiKeyManager api(std::chrono::hours(1));
    return 0;
}
