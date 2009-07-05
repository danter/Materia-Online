#ifndef __BATTLE_H
#define __BATTLE_H

#include "constants.h"
#include "c_battle_char.h"
#include <vector>

// Main battle function
void startBattle(std::vector<Char_data>&);

// Support functions
unsigned int battleAttack(Char_battle_data&, Char_battle_data&);
unsigned int factionsAlive(const std::vector<Char_battle_data>&);
unsigned int targetSelect(std::vector<Char_battle_data>&);
unsigned int updateSpeedAndFactions(std::vector<Char_battle_data>&);
Char_battle_data calcNextInQue(std::vector<Char_battle_data>&);

// Misc functions
void init_BattleCharacters(std::vector<Char_battle_data>&, const std::vector<Char_data>&);
bool compareSpeed(const Char_battle_data& x, const Char_battle_data& y);
unsigned int nrand(int);

#endif
