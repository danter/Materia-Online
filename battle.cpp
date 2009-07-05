#include "battle.h"
#include "c_battle_char.h"
#include <iostream>
#include <cstdlib>
#include <stdexcept>

using std::cerr;
using std::cin;
using std::cout;
using std::domain_error;
using std::endl;
using std::vector;
using std::string;

// The actual battle goes here
void startBattle(vector<Char_data> &base_characters) {

	int target, damageDone, battle = 1, command = 1;
	float attackPenalty = 0;
	Char_battle_data active_char;
	vector<Char_battle_data> battle_characters;

	srand(time(NULL));

	// Prepare the battling characters vector.
	init_BattleCharacters(battle_characters, base_characters);

	// Init speed to fit for the current amount of players (no overflows!)
	for(vector<Char_battle_data>::iterator iter = battle_characters.begin(); iter != battle_characters.end(); ++iter) {
		iter->battleBaseSpeed = iter->baseSpeed / battle_characters.size();
		iter->speed = iter->battleBaseSpeed;
	}

	battle = updateSpeedAndFactions(battle_characters);

	while(battle) {
		// Print the stats, for debug and general testing
		cout << "Name\t\tFaction\tLevel\tHP\tSP\tPwr\tDex\tAgi\tVit\tInt\tSpi\tLuck\t\tAttack\tDefense\tHit\tEvade\tbBspeed\tSpeed" << endl;
		for(vector<Char_battle_data>::iterator iter = battle_characters.begin(); iter != battle_characters.end(); ++iter)
			iter->displayData();
		cout << endl;

		// Returns whos next in line, and sorts the character list after speeds.
		active_char = calcNextInQue(battle_characters);

		cout << active_char.name << ", it's your " << active_char.turns+1 << " turn, make your move." << endl;
		cout << "1:Attack, 2:Skill, 3:Pass, 4:Flee Battle, 5:Quit" << endl;
		cout << "> ";

		cin >> command;

		switch(command) {
			case 1:

				target = targetSelect(battle_characters);
				cout << active_char.name << " attacks " << battle_characters[target].name << ",";

				damageDone = battleAttack(active_char, battle_characters[target]);
				attackPenalty = NORMAL_ATK_PENALTY;
				if( damageDone != 0)
					cout << " attack hit's for " << damageDone << " damage." << endl;
				else
					cout << " attack misses!" << endl;

				// check Game Over and battle end
				if(battle_characters[target].HP <= 0) {
					cout << battle_characters[target].name << " dies, Game Over!" << endl;
					battle_characters.erase(battle_characters.begin() + target);

					battle = updateSpeedAndFactions(battle_characters);
				}

			break;

			case 2:
				cout << "Not used yet, turn passed." << endl;
			break;

			case 3:
				cout << active_char.name << " passes this turn." << endl;
			break;

			case 4:
				cout << active_char.name << " attempts to flee the battle." << endl;
				// roll if successful, then save(for players) and delete if character is.
				for(vector<Char_battle_data>::iterator iter = battle_characters.begin(); iter != battle_characters.end(); ++iter) {
					if(active_char.id == iter->id) {
						iter = battle_characters.erase(iter);
						break;
					}
				}

				battle = updateSpeedAndFactions(battle_characters);

			break;

			case 5:
				battle = 0;
			break;

			default:
				cerr << "Something went wrong!" << endl;
				exit(1);
		}

		// Reset the active chars speed, with skill penalty.
		// And mark that the character has made an attack this turn
		// Also increase the other characters speed state.
		for(vector<Char_battle_data>::iterator iter = battle_characters.begin(); iter != battle_characters.end(); ++iter) {
			if(active_char.id == iter->id) {
				iter->speed = iter->battleBaseSpeed - ( attackPenalty / battle_characters.size());
				iter->turns++;
			}
			else
				iter->speed += iter->battleBaseSpeed;
		}

	}

	cout << "The winners are: the '" << battle_characters[0].faction << "' faction, Congratulations!" << endl;

}


// returns the damage done if the attack is successful, otherwise 0
unsigned int battleAttack(Char_battle_data &attacker, Char_battle_data &defender) {

	int roll = nrand(DICE_MAX);
	// Check for hit
	if(attacker.hit + roll - defender.evade > 0) {
		cout << " and rolls " << roll << " totaling on " << attacker.hit+roll << " hit,";
		// takes damage
		roll = nrand(DICE_MAX);
		if( attacker.attack + roll - defender.defense > 0) {
			defender.HP = defender.HP - (int)(attacker.attack - defender.defense);

			return (int)(attacker.attack - defender.defense);
		}
		// crit, will come later.. but adds extra damage to the damage already done
	}
	else {
		cout << " and rolls " << roll << " totaling on " << attacker.hit+roll << " hit,";
		return 0;
	}

	return 0;
}



unsigned int factionsAlive(const vector<Char_battle_data> &battle_characters) {

		int addFaction = 1;
		vector<string> factions;

		for(vector<Char_battle_data>::const_iterator iter = battle_characters.begin(); iter != battle_characters.end(); ++iter) {
			addFaction = 1;
			for(vector<string>::iterator jiter = factions.begin(); jiter != factions.end(); ++jiter) {
				if(*jiter == iter->faction) {
					addFaction = 0;
					break;
				}
			}

			if(addFaction)
				factions.push_back(iter->faction);
		}

	return factions.size();
}

unsigned int targetSelect(vector<Char_battle_data> &battle_characters) {
	int target = 0;

	cout << "Chose target: " << endl;
	for(vector<Char_battle_data>::size_type i = 0; i != battle_characters.size(); ++i) {
		cout << i+1 << ":" << battle_characters[i].name << " ";
	}
	target=-1;
	while(target <= 0 || target >= (int)battle_characters.size()+1 ) {
		cout << endl;
		cout << "> ";
		cin >> target;
		if(target <= 0 || target >= (int)battle_characters.size()+1 )
			cout << "Invalid Target, please choose again." << endl;
	}

	target--;
	return target;
}


// Returns the player next in line
Char_battle_data calcNextInQue(vector<Char_battle_data> &chars) {
	Char_battle_data active_char;
	active_char = chars[0];

	// Check character speeds, and return the highest one
	for(vector<Char_battle_data>::const_iterator iter = chars.begin(); iter != chars.end(); ++iter) {
		if(active_char.speed < iter->speed )
			active_char = *iter;
	}

	return active_char;
}


unsigned int updateSpeedAndFactions(vector<Char_battle_data> &battle_characters) {

	// Check awailable players and factions
	if(battle_characters.size() <= 1 || factionsAlive(battle_characters) == 1) {
		return 0;
	}

	// Recalc speeds, for new ammount of players
	for(vector<Char_battle_data>::iterator iter = battle_characters.begin(); iter != battle_characters.end(); ++iter) {
		iter->calcSpeed(battle_characters.size());
	}

	return 1;
}


void init_BattleCharacters(vector<Char_battle_data> &battle_characters, const vector<Char_data> &base_characters) {

	Char_battle_data input_BattleContainer;

	for(vector<Char_data>::const_iterator base_char = base_characters.begin(); base_char != base_characters.end(); ++base_char) {
		input_BattleContainer.id = base_char->id;
		input_BattleContainer.name = base_char->name;
		input_BattleContainer.faction = base_char->faction;
		input_BattleContainer.lvl = base_char->lvl;
		input_BattleContainer.HP = base_char->HP;
		input_BattleContainer.SP = base_char->SP;
		input_BattleContainer.pwr = base_char->pwr;
		input_BattleContainer.dex = base_char->dex;
		input_BattleContainer.agi = base_char->agi;
		input_BattleContainer.vit = base_char->vit;
		input_BattleContainer.intel = base_char->intel;
		input_BattleContainer.spi = base_char->spi;
		input_BattleContainer.luk = base_char->luk;
		input_BattleContainer.attack = base_char->attack;
		input_BattleContainer.defense = base_char->defense;
		input_BattleContainer.hit = base_char->hit;
		input_BattleContainer.evade = base_char->evade;
		input_BattleContainer.baseSpeed = base_char->speed;

		input_BattleContainer.turns = 0;

		battle_characters.push_back(input_BattleContainer);
	}

}


// return random integer in the range [0, n)
unsigned int nrand(int n) {
	if(n <= 0 || n > RAND_MAX)
		throw domain_error("Argument to nrand is out of range");

	const int bucket_size = RAND_MAX / n;
	int r;

	do r = rand() / bucket_size;
	while (r >= n);

	return r;
}

bool compareSpeed(const Char_battle_data& x, const Char_battle_data& y) {
	return x.speed > y.speed;
}
