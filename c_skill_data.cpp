#include "c_skill_data.h"
#include <iostream>

using std::cout;
using std::endl;
using std::string;
using std::vector;

void Skill_data::clear() {
	name.clear();
	type.clear();
	effect.resize(0);
	casttime = 0;
	duration = 0;
}

void Skill_data::displayData() {
	cout << "Skill Name: " << name << endl;
	cout << "Skill Type: " << type << endl;
	cout << "Casttime: " << casttime << endl;
	cout << "Duration: " << duration << endl;
	cout << "Effects: " << endl;
	for(vector<string>::const_iterator iter = effect.begin(); iter != effect.end(); ++iter)
		cout << "\t" << *iter << endl;

	cout << endl;
}
