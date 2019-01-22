#ifndef SUBJECT_H
#define SUBJECT_H

#include <string>
#include <vector>

#include "Period.h"
#include "Participant.h"

class Participant;

class Subject
{
private:
	string Id;
	vector <Participant> participants;
	float Hours;	//2, 3 or 4.5 hours a week
	Period Unavailable; //the period for which this subject is going
			//to be unavailable 
public:
	Subject() : Id("b1"){}
	Subject(string aId) : Id(aId){}
	Subject(string aId, float aHour) : Id(aId), Hours(aHour){};
	string getId(){	return Id;	}
	float getHours(){	return Hours;	}
	int getNoOfParticipants(){	return participants.size();	}
	void addParticipant(Participant&);
	vector <Participant> getParticipants(){	return participants;	}
	Period& getUnavailable(){	return Unavailable;	}
	void print();
};
#endif
