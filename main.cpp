#include "main.h"
#include "constants.h"
#include "battle.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

//#include <cstdio>
//#include <cstring>
//#include "simpleNet.h"

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;


// Read Character data
int readCharacterData(vector<Char_data> &characters, string datafile) {
	Char_data input_container;

	ifstream infile(datafile.c_str());

	if(infile == NULL) {
		cerr << "Failed to open file: " << datafile << endl;
		return -1;
	}

	string strLine;
	istringstream ssLine;

	while(getline(infile, strLine)) {
		ssLine.clear();
		ssLine.str(strLine);

		ssLine >> strLine;

		if(strLine.size() > 1) {
			if(strLine[0] == '/' && strLine[1] == '/')
				continue;
		}

		input_container.id = atoi(strLine.c_str());

		// Name / faction / level / pwr / dex / agi / vit / intel / spi / luk / wep_train
		ssLine >> input_container.name >> input_container.faction >> input_container.lvl >> input_container.pwr
				>> input_container.dex >> input_container.agi >> input_container.vit
				>> input_container.intel >> input_container.spi >> input_container.luk
				>> input_container.wep_train;

		input_container.calcActiveStats();

		characters.push_back(input_container);
	}
	return 0;
}

int readSkillData(vector<Skill_data> &skills, string datafile) {
	typedef string::size_type string_size;
	string_size i;
	Skill_data input_container;

	ifstream infile(datafile.c_str());

	if(infile == NULL) {
		cerr << "Failed to open file: " << datafile << endl;
		return -1;
	}

	string strLine;
	istringstream ssLine;


	int createskill = 0;
	while(getline(infile, strLine)) {
		ssLine.clear();
		ssLine.str(strLine);

		ssLine >> strLine;

		if(strLine.size() == 0)
			continue;

		if(strLine.size() > 1) {
			if(strLine[0] == '/' && strLine[1] == '/')
				continue;
		}

		if(strLine == "<skill>") {
			createskill = 1;
			input_container.clear();
			continue;
		}

		if(createskill && strLine == "name:") {
			getline(ssLine,strLine);
			// Eat spaces!
			for(i = 0; i != strLine.size() && isspace(strLine[i]); ++i);
			input_container.name = strLine.substr(i, strLine.size() - i);
			continue;
		}

		if(createskill && strLine == "type:") {
			getline(ssLine,strLine);
			// Eat spaces!
			for(i = 0; i != strLine.size() && isspace(strLine[i]); ++i);
			input_container.type = strLine.substr(i, strLine.size() - i);
			continue;
		}

		// Needs modification, one string object should be done for each effect that occurs,
		// ie one string for mana cost and one string for damage done
		if(createskill && strLine == "effect:") {
			while(getline(ssLine,strLine, '&')) {
				// Eat spaces!
				for(i = 0; i != strLine.size() && isspace(strLine[i]); ++i);
				input_container.effect.push_back(strLine.substr(i, strLine.size() - i));
				// Clear up the double && with this...
				// Maybe I should change the way battle scripts works,
				// but I want it too be a bit like c++ without compile.
				getline(ssLine,strLine, '&');
			}
			continue;
		}

		if(createskill && strLine == "casttime:") {
			ssLine >> input_container.casttime;
			continue;
		}

		if(createskill && strLine == "duration:") {
			ssLine >> input_container.duration;
			continue;
		}

		if(strLine == "</skill>") {
			if(!createskill) {
				cerr << "A skill was closed without beeing started" << endl;
				return -1;
			}
			createskill = 0;
			if(input_container.name.size() == 0) {
				cerr << "Skill name unawailable" << endl;
				if(input_container.type.size() == 0)
					cerr << "Skill type unawailable" << endl;
				if(input_container.effect.size() == 0)
					cerr << "Skill effect unawailable" << endl;
				return -1;
			}
			if(input_container.type.size() == 0) {
				cerr << "Skill type unawailable" << endl;
				if(input_container.effect.size() == 0)
					cerr << "Skill effect unawailable" << endl;
				return -1;
			}
			if(input_container.effect.size() == 0) {
				cerr << "Skill effect unawailable" << endl;
				return -1;
			}

			cout << "loaded '" << input_container.name << "' skill" << endl;
			skills.push_back(input_container);
		}

	}

	if(createskill) {
		cerr << "A skill in initiation mode was not closed." << endl;
		return -1;
	}

	return 0;
}


int main(int argc, char *argv[]) {

	vector<Char_data> characters;
	vector<Skill_data> skills;

	if(readCharacterData(characters, "data/char_data.txt") != 0) {
		cerr << "Character load failed." << endl;
		return -1;
	}

	if(readSkillData(skills, "data/skill_data.txt") != 0) {
		cerr << "Skill load failed." << endl;
		return -1;
	}
	for(vector<Skill_data>::iterator iter = skills.begin(); iter != skills.end(); ++iter)
		iter->displayData();


	startBattle(characters);

    return 0;
}
