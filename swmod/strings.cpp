#pragma once
#include "strings.h"
#include <iostream>

void printHelpMessage() {
    std::cout << "--------------------------------------\n";
    std::cout << "| command |  description  |   args   |\n";
    std::cout << "|    ?    |   help page   |          |\n";
    std::cout << "|    g    |    god mode   |          |\n";
    std::cout << "|    x    |     exit      |          |\n";
    std::cout << "|    i    |      inf      | a,u,e,f  |\n";
    std::cout << "|   am    |   admin menu  |          |\n";
    std::cout << "|   nc    |    no clip    |          |\n";
    std::cout << "|   vd    |   veh damage  |          |\n";
    std::cout << "|  sp,mp  | show map plrs |          |\n";
    std::cout << "|  heal   |   heals plr   |          |\n";
    std::cout << "|  kill   |   kills plr   |          |\n";
    std::cout << "|  reload |reloads weapons|          |\n";
    std::cout << "|  give   |  gives items  |          |\n";
    std::cout << "|   rf    |  rapid  fire  |rifle only|\n";
    std::cout << "|   ns    |   no spread   |rifle only|\n";
    std::cout << "| projid  |  change proj  |rifle only|\n";
    std::cout << "|   fix   |  fixes items  |          |\n";
    std::cout << "|  rifle  |  gives rifle  |          |\n";
    std::cout << "|   loa   |    loadout    |          |\n";
    //std::cout << "|   al    |  auto loadout |          |\n";
    std::cout << "--------------------------------------\n";
}