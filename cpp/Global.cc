#include <fstream>
#include <stdlib.h>

#include "Schedule.h"
#include "Global.h"
vector <Participant> Participants;
vector <Subject> Subjects;

FILE *fin;
extern int yylex();
void loadParticipants()
{
	void parseParticipant();
	system("cat *.p > Participant.dat");
	parseParticipant();
}

void loadSubjects()
{
	if((fin = fopen("Subject.dat", "r")) == 0)
	{
		cout << "Couldn't read Subject.dat!" << endl;
		exit (1);
	}
	while(yylex() == ID)
	{
		string id = input_string;
		float hours;
		if(yylex() == NUMBER)
		{
			hours = input_number;
		}
	
		else
		{
			cout << "Error in Subject.dat" << endl;
		}	
		Subject *s = new Subject(id, hours);
		Subjects.push_back(*s);
	}
	fclose(fin);
}

void creatAssoc()
{
	for (int i = 0; i < Participants.size(); i++)
	{
		vector<string> subject_list = Participants[i].getSubjects();
		for (int j = 0; j < subject_list.size(); j++)
		{
			for(int k = 0; k < Subjects.size(); k++)
			{
				if(subject_list[j] == Subjects[k].getId())
				{
					Subjects[k].addParticipant
						(Participants[i]);
					break;
				}
			}
		}
	}
}

void cleanup()
{
	Participants.clear();
	Subjects.clear();
}
