#ifndef PARTICIPANT_H
#define PARTICIPANT_H
using namespace std;
#include <string>
#include <vector>
#include "Period.h"

using namespace std;

class Participant
{
private:
	string Id;
	vector <string> subjects; //only the list of subject ids
	Period Unavailable; //the period for which this participant is 
				//unavailable.
public:
	Participant() : Id("a1"){}
	~Participant();
	Participant(string aId) : Id(aId){}
	string& getId(){	return Id;	}
	vector<string>& getSubjects(){	return subjects;	}
	void addSubject(string& s){	subjects.push_back(s);	}
	void makeUnavailable(Duration&);
	Period& getUnavailable(){	return Unavailable;	}
	void print();
};

#endif
