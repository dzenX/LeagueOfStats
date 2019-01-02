#include "Server.h"

#include <iostream>

int main(int argc, char** argv) {
    Server s(std::string("euw1"));  
    s.addPlayer(std::string("qwellow"));
    s.downloadStats(std::string("qwellow"));
    return 0;
}
