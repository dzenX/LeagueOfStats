#include "Server.h"

#include <iostream>

int main(int argc, char** argv) {
    Server s("euw1"); 
    using namespace std::string_literals; 
    s.addPlayer("qwellow"s);
    s.downloadStats("qwellow"s);
    return 0;
}
