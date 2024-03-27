#pragma once
#include "strings.h"
#include <iostream>

void printHelpMessage() {
    std::cout << "--------------------------------------------------------------------------\n";
    std::cout << "| command |    description         |    usage <required> (optional)      |\n";
    std::cout << "|    ?    |     help page          |             ?                       |\n";
    std::cout << "|    g    |      god mode          |             g                       |\n";
    std::cout << "|    x    |       exit             |             x                       |\n";
    std::cout << "|   ia    |      inf ammo          |             ia                      |\n";
    std::cout << "|   am    |     admin menu         |             am                      |\n";
    std::cout << "|   nc    |      no clip           |             nc                      |\n";
    std::cout << "|   vs    |   vehicle spawning     |             vs                      |\n";
    std::cout << "|  sp,mp  |   show map plrs        |             sp                      |\n";
    std::cout << "|  heal   |     heals plr          |             heal                    |\n";
    std::cout << "|  kill   |     kills plr          |             kill                    |\n";
    std::cout << "|  reload |  reloads weapon        |           reload (slot)             |\n";
    std::cout << "|  give   |    gives items         |give <itemid> <slot> (charge) (ammo) |\n";
    std::cout << "|   rf    |    rapid  fire         |               rf                    |\n";
    std::cout << "| projid  | change proj(rifle)     |       projid <projectile id>        |\n";
    std::cout << "|  sip    | set invalid projectile |             sip                     |\n";
    std::cout << "|  fix    |  fixes items           |             fix <slot>              |\n";
    std::cout << "|  rifle  |  gives rifle           |             rifle                   |\n";
    std::cout << "|  torch  |  gives welding torch   |             torch                   |\n";
    std::cout << "|  auth   |  authorizes you        |             auth                    |\n";
    std::cout << "--------------------------------------------------------------------------\n";
}



