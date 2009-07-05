#include <iostream>
#include "c_battle_char.h"

using std::cout;
using std::endl;

void Char_battle_data::displayData() {
		cout << name << "\t\t" << faction << "\t" << lvl << "\t" << HP << "\t" << SP << "\t"
				<< pwr << "\t" << dex << "\t" << agi << "\t" << vit << "\t"
				<< intel << "\t" << spi << "\t" << luk << "\t" << attack << "\t"
				<< defense << "\t" << hit << "\t" << evade << "\t" << battleBaseSpeed << "\t"
				<< speed << endl;
}

void Char_battle_data::calcSpeed(unsigned int characters) {
	battleBaseSpeed = baseSpeed / characters;
}
