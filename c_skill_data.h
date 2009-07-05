#ifndef __C_SKILL_DATA
#define __C_SKILL_DATA

#include <string>
#include <vector>

class Skill_data {
public:
	std::string name;
	std::string type;
	float casttime;
	float duration;
	std::vector<std::string> effect;

	void clear();
	void displayData();
};

#endif
