#ifndef __C_BATTLE_CHAR
#define __C_BATTLE_CHAR

#include "c_char_data.h"

class Char_battle_data : public Char_data {
public:
	int turns;
	float baseSpeed;
	float battleBaseSpeed;

	void displayData();
	void calcSpeed(unsigned int characters);
};


#endif
