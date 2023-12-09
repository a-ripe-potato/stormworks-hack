#pragma once
#include "strings.h"
#include <iostream>

void printHelpMessage() {
    std::cout << "-------------------------------------\n";
    std::cout << "| command |  description  |   args  |\n";
    std::cout << "|    ?    |   help page   |         |\n";
    //std::cout << "|    g    |    god mode   |         |\n";
    std::cout << "|    x    |     exit      |         |\n";
    //std::cout << "|    i    |      inf      | a,u,e,f |\n";
    std::cout << "|    i    |      inf      |  a,e,f  |\n";
    std::cout << "|   am    |   admin menu  |         |\n";
    std::cout << "|   nc    |    no clip    |         |\n";
    std::cout << "|   vd    |   veh damage  |         |\n";
    std::cout << "|   dw    |disable weapons|         |\n";
    std::cout << "|   ew    |enable  weapons|         |\n";
    std::cout << "|  sp,mp  | show map plrs |         |\n";
    std::cout << "|   set   |   set vars    |   plr   |\n";
    std::cout << "|   smg   |   gives smg   |         |\n";
    std::cout << "|   loa   |    loadout    |         |\n";
    std::cout << "|   al    |  auto loadout |         |\n";
    std::cout << "-------------------------------------\n";
}