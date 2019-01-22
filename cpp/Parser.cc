#include <stdlib.h>

#include "Participant.h"
#include "Global.h"
#include "Schedule.h"
#include "Period.h"
#include "Timer.h"

static int token;
extern int yylex();
extern string input_string;
extern float input_number;

static void SubList(vector<string>& slist)
{
	while ((token = yylex ()) == ID)
	{
		slist.push_back(input_string);
	}
	if (token == ';')
	{
		return;
	}
	//else something is wrong
	cout << "Error in SubList!" << endl;
}

static void ConstrList(Period&);
static void Constraint(Period& p)
{
	switch(token)
	{
	case MON: case TUE: case WED: case THU: case FRI:
		Time t1, t2;
		t1.WeekDay = token;
		if ((token = yylex())!= NUMBER)
		{
			cout << "Error in Constr!" << endl;
			exit(1);
		}
		t1.Hour = (int)input_number;
		if ((token = yylex())!= '-')
		{
			cout << "Error in Constr!" << endl;
			exit(1);
		}
		token = yylex();
		switch (token)
		{
		case MON: case TUE: case WED: case THU: case FRI:
			t2.WeekDay = token;
			break;
		default:
			cout << "Error in Constr!" << endl;
			exit(1);
		}
		if ((token = yylex())!= NUMBER)
		{
			cout << "Error in Constr!" << endl;
			exit(1);
		}
		t2.Hour = (int)input_number;
		Duration *d = new Duration(Timer::TimeToFloat(t1), 
				Timer::TimeToFloat(t2));
		p.addDuration(*d);
		token = yylex(); ConstrList(p);
		break;	
	}
	return;
	
}

static void ConstrList(Period& p)
{
	switch (token)
	{
	case MON: case TUE: case WED: case THU: case FRI:
		Constraint(p); break;
		
	case ';':
		return;
	default:
		cout << "Error after ConstrList!" << endl;
		exit (1);
	}
}

static void PartList();
static void Part()
{
	string s = input_string;
	Participant *p = new Participant(s);
	SubList(p->getSubjects());
	token = yylex();
	ConstrList(p->getUnavailable());
	Participants.push_back(*p);
	if ((token = yylex()) == ID)
	{
		PartList();
	}
}

static void PartList()
{
	while (token == ID)
	{
		Part();
	}
	if (token == 0)
	{
		return;
	}
	//else something is wrong
	cout << "Error after PartList!" << endl;
	exit(1);
}

extern FILE *fin;
void parseParticipant()
{
	fin = fopen("Participant.dat" , "r");
	token = yylex();
	PartList();
	fclose(fin);
}
