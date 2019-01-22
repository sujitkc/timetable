#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <vector>
#include "Participant.h"
#include "Subject.h"

enum 
{
	MON = 1,
	TUE,
	WED,
	THU,
	FRI,

	ERROR,
	NUMBER,
	ID	
};

extern FILE *fin;

extern vector <Participant> Participants;
extern vector <Subject> Subjects;


extern string input_string;
extern float input_number;
//Prototypes of top-level global functions
void loadParticipants();
void loadSubjects();
void creatAssoc();
void cleanup();
#endif
