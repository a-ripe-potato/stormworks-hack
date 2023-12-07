#pragma once
#include "strings.h"
#include <iostream>

void printHelpMessage() {
    std::cout << "----------------------------------\n";
    std::cout << "| command | description |  args  |\n";
    std::cout << "|    ?    |  help page  |        |\n";
    std::cout << "|    g    |   god mode  |        |\n";
    std::cout << "|    x    |    exit     |        |\n";
    std::cout << "|    i    |     inf     |   a,u  |\n";
    std::cout << "|   am    |  admin menu |   t,f  |\n";
    std::cout << "|   nc    |   no clip   |   t,f  |\n";
    //std::cout << "|   sp    |  show plrs  |   t,f  |\n";
    std::cout << "|   set   |  set vars   | plr,nc |\n";
    std::cout << "|   smg   |  gives smg  |        |\n";
    std::cout << "|   loa   |   loadout   |        |\n";
    std::cout << "|   al    | auto loadout|        |\n";
    std::cout << "----------------------------------\n";
}