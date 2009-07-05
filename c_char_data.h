#ifndef __C_CHAR_DATA_H
#define __C_CHAR_DATA_H

#include <string>

class Char_data {
public:
	unsigned int id;
	std::string name;
	std::string faction;
	int lvl;
	int pwr;
	int dex;
	int agi;
	int vit;
	int intel;
	int spi;
	int luk;
	float speed;
	float attack;
	float defense;
	float hit;
	float evade;
	float wep_train;
	int HP;
	int SP;

	void calcActiveStats();
};


#endif
