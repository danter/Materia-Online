#ifndef __MAIN_H
#define __MAIN_H

#include "c_char_data.h"
#include "c_skill_data.h"
#include <vector>
#include <string>

int readCharacterData(std::vector<Char_data> &, std::string);
int readSkillData(std::vector<Skill_data>&, std::string);

#endif
