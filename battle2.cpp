#include "battle.h"
#include <iostream>
#include <cstdlib>
#include <stdexcept>

using std::cerr;
using std::cin;
using std::cout;
using std::domain_error;
using std::endl;
using std::vector;


bool compareSpeed(const Char_battle_data& x, const Char_battle_data& y) {
	return x.speed > y.speed;
}

// return random integer in the range [0, n)
int nrand(int n) {
	if(n <= 0 || n > RAND_MAX)
		throw domain_error("Argument to nrand is out of range");

	const int bucket_size = RAND_MAX / n;
	int r;

	do r = rand() / bucket_size;
	while (r >= n);

	return r;
}


// Returns the player next in line
Char_battle_data calcNextInQue(vector<Char_battle_data> &chars) {
	Char_battle_data active_char;
	active_char = chars[0];

	// Check character speeds, and return the highest one
	for(vector<Char_battle_data>::size_type i = 0; i != chars.size(); ++i) {
		if(active_char.speed < chars[i].speed )
			active_char = chars[i];
	}

	return active_char;
}


// returns the damage takend if the attack is successful, otherwise 0
int battleAttack(Char_battle_data &attacker, Char_battle_data &defender) {
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


// All name lookups in this function needs to be replaced with a battle ID,
// so characters and monsters don't get mixed up.
void startBattle(vector<Char_data> base_characters) {

	srand(time(NULL));

	// Prepare the battling characters vector.
	Char_battle_data input_BattleContainer;
	vector<Char_battle_data> battle_characters;
	for(vector<Char_data>::size_type i = 0; i != base_characters.size(); ++i) {
//		input_BattleContainer = base_characters[i];
		input_BattleContainer.id = base_characters[i].id;
		input_BattleContainer.name = base_characters[i].name;
		input_BattleContainer.level = base_characters[i].level;
		input_BattleContainer.HP = base_characters[i].HP;
		input_BattleContainer.SP = base_characters[i].SP;
		input_BattleContainer.pwr = base_characters[i].pwr;
		input_BattleContainer.dex = base_characters[i].dex;
		input_BattleContainer.agi = base_characters[i].agi;
		input_BattleContainer.vit = base_characters[i].vit;
		input_BattleContainer.intel = base_characters[i].intel;
		input_BattleContainer.spi = base_characters[i].spi;
		input_BattleContainer.luk = base_characters[i].luk;
		input_BattleContainer.attack = base_characters[i].attack;
		input_BattleContainer.defense = base_characters[i].defense;
		input_BattleContainer.hit = base_characters[i].hit;
		input_BattleContainer.evade = base_characters[i].evade;
		input_BattleContainer.speed = base_characters[i].speed;
		input_BattleContainer.turns = 0;

		battle_characters.push_back(input_BattleContainer);
	}
	// Modify speed to fit for the current players (no overflows!)
	for(vector<Char_battle_data>::size_type i = 0; i != battle_characters.size(); ++i) {
		battle_characters[i].speed = battle_characters[i].speed / battle_characters.size();
	}


	int battle = 1, command=1;
	Char_battle_data active_char;
	while(battle) {
		// Print the stats, for debug and general testing
		cout << "Name\t\tLevel\tHP\tSP\tPwr\tDex\tAgi\tVit\tInt\tSpi\tLuck\tAttack\tDefense\tHit\tEvade\tSpeed" << endl;
		for(vector<Char_battle_data>::size_type i = 0; i != battle_characters.size(); ++i)
			cout << battle_characters[i].name << "\t\t" << battle_characters[i].level << "\t"
					<< battle_characters[i].HP << "\t" << battle_characters[i].SP << "\t"
					<< battle_characters[i].pwr << "\t" << battle_characters[i].dex << "\t"
					<< battle_characters[i].agi << "\t" << battle_characters[i].vit << "\t"
					<< battle_characters[i].intel << "\t" << battle_characters[i].spi << "\t"
					<< battle_characters[i].luk << "\t" << battle_characters[i].attack << "\t"
					<< battle_characters[i].defense << "\t" << battle_characters[i].hit << "\t"
					<< battle_characters[i].evade << "\t" << battle_characters[i].speed << endl;


		// Returns whos next in line, and sorts the character list after speeds.
		active_char = calcNextInQue(battle_characters);

		cout << active_char.name << ", it's your " << active_char.turns+1 << " turn, make your move." << endl;
		cout << "1:Attack, 2:Skill, 3:Pass, 4:Flee Battle, 5:Quit" << endl;
		cout << "> ";

		cin >> command;


		// Reset the active chars speed, with skill penalty.
		// And mark that he has made an attack this turn
		// Also increase the other characters speed state.
		for(vector<Char_battle_data>::size_type i = 0; i != battle_characters.size(); ++i) {
			if(active_char.id == battle_characters[i].id) {
				battle_characters[i].speed = (base_characters[i].speed / battle_characters.size() ) - ( NORMAL_ATK_PENALTY / battle_characters.size());
				battle_characters[i].turns++;
			}
			else
				battle_characters[i].speed += base_characters[i].speed / battle_characters.size();
		}

		int target;
		int tmp_var;
		switch(command) {
			case 1:
				cout << "Chose target: " << endl;
				for(vector<Char_battle_data>::size_type i = 0; i != battle_characters.size(); ++i) {
					cout << i << ":" << battle_characters[i].name << " ";
				}
				target=-1;
				while(target <= -1 || target >= battle_characters.size() ) {
					cout << endl;
					cout << "> ";
					cin >> target;
					if(target < 0 || target >= battle_characters.size() )
						cout << "Invalid Target, please choose again." << endl;
				}
				cout << active_char.name << " attacks " << battle_characters[target].name << ",";
				tmp_var = battleAttack(active_char, battle_characters[target]);
				if( tmp_var != 0)
					cout << " attack hit's for " << tmp_var << " damage." << endl;
				else
					cout << " attack misses!" << endl;

				if(battle_characters[target].HP <= 0) {
					cout << battle_characters[target].name << " dies, Game Over!" << endl;
					battle_characters.erase(battle_characters.begin() + target);
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
				// roll if successful, then delete if character is.
				for(vector<Char_battle_data>::size_type i = 0; i != battle_characters.size(); ++i) {
					if(active_char.id == battle_characters[i].id) {
						battle_characters.erase(battle_characters.begin() + i);
					}
				}
			break;

			case 5:
				battle = 0;
			break;

			default:
				cerr << "Something went wrong!" << endl;
				exit(1);
		}
	}
}
