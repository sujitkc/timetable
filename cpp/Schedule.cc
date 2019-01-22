#include <stdlib.h>

#include "Schedule.h"
#include "Global.h"
#include "Timer.h"

Duration Slot::getNextAvailable(Duration ad)
{
	return Unavailable.getNextFree(ad);
}

float Slot::schedule(Duration d)
{
	duration = d;
}

ostream& operator << (ostream& o, const Slot& s)
{
	o << "Subjects : " << endl;
	for (int j = 0; j < s.subjects.size(); j++)
	{
		o << '\t' << s.subjects[j];
	}
	o << endl;

	o << "Lectures : " << endl ;
	for (int i = 0; i < s.Lectures.size(); i++)
	{
		o << s.Lectures[i];
	}
	o << "Unavailable for :" << endl;
	o << s.Unavailable;
	return o;
}

Schedule::Schedule()
{
	Id = -1;
}

void Schedule::makeSchedule(DepGraph &G)
{
	//form the slots with subjects allocated to them.
	//shift the slots to maintain availability constraints
	//shift to maintain the continuity constraints
	//done	
	theSchedule = vector <Slot> (G.getMaxColour());
	for (int i = 0; i < theSchedule.size(); i++)
	{
		Slot *s = new Slot();
		theSchedule[i] = *s;
	}

	for (int i = 0; i < G.Nodes.size(); i++)
	{
		if(G.Nodes[i].subject->getNoOfParticipants())
		{
			theSchedule[G.Nodes[i].Colour - 1].
				subjects.push_back(G.Nodes[i].subject->getId());
			theSchedule[G.Nodes[i].Colour - 1].
				Unavailable = 
				theSchedule[G.Nodes[i].Colour - 1].
				Unavailable || G.Nodes[i].subject->getUnavailable();
			if (G.Nodes[i].subject->getHours() > 
					theSchedule[i].duration.End)
			{
				theSchedule[i].duration.Start = 0;
				theSchedule[i].duration.End = 
					G.Nodes[i].subject->getHours();
			}
		}
	}

	for (int i = 0; i < theSchedule.size(); i++)
	{
		if(theSchedule[i].getNoOfHours() == 4.5)
		{
			for4_5(theSchedule[i]);
		}
		else if (theSchedule[i].getNoOfHours() == 3.0)
		{
			for3_0(theSchedule[i]);
		}
		else
		{
			for2_0(theSchedule[i]);
		}
	}
}

void Schedule::for4_5(Slot &s)
{
	for3Days(s);
}

void Schedule::for3Days(Slot &s)
{
	float offset;
	if (s.getNoOfHours() == 4.5)
	{
		offset = 1.5;
	}
	else
	{
		offset = 1.0;
	}
	Duration 	*d1 = new Duration(0, offset), 
			*d2 = new Duration(Timer::TimeToFloat(Time(3, 800)), 
				Timer::TimeToFloat(Time(3, 800)) + offset), 
			*d3 = new Duration(Timer::TimeToFloat(Time(5, 800)), 
				Timer::TimeToFloat(Time(5, 800)) + offset);

	while (!((	Timer::isContiguous(*d1) &&
			Timer::isContiguous(*d2) &&
			Timer::isContiguous(*d3)) &&

			(s.isAvailable(*d1) && s.isAvailable(*d2) &&
			s.isAvailable(*d2)) &&

			(Busy.isFree(*d1) && 
			 Busy.isFree(*d2) && 
			 Busy.isFree(*d3))))
	{
		if(!	Timer::isContiguous(*d1) &&
			Timer::isContiguous(*d2) &&
			Timer::isContiguous(*d3)) 
		{
			*d1 = Timer::getNextContiguous(*d1);
			*d2 = Timer::getNextContiguous(*d2);
			*d3 = Timer::getNextContiguous(*d3);
		}

		if(!	(s.isAvailable(*d1) && s.isAvailable(*d2) &&
			s.isAvailable(*d2)))
		{
			*d1 = s.getNextAvailable(*d1);
			*d2 = s.getNextAvailable(*d2);
			*d3 = s.getNextAvailable(*d3);	
		}

		if(!	(Busy.isFree(*d1) && 
			 Busy.isFree(*d2) && 
			 Busy.isFree(*d3)))
		{
			*d1 = Busy.getNextFree(*d1);
			*d2 = Busy.getNextFree(*d2);
			*d3 = Busy.getNextFree(*d3);
		}

		if(!	(d1->End <= Timer::getEndOfDay(1)) &&
			(d2->End <= Timer::getEndOfDay(3)) &&
			(d3->End <= Timer::getEndOfDay(5)))
		{
			cout << "Scheduling Failed at " << 
				offset * 3 << "! Sorry! " << endl;
			cout << "d1 = " << d1->End << "\td2 = " << d2->End
				<< "\td3 = " << d3->End << endl;
			
			delete(d1); delete(d2); delete(d3);
			exit (1);
		}
	}

	s.addLectureSlot(*d1);
	s.addLectureSlot(*d2);
	s.addLectureSlot(*d3);

	Busy.addDuration(*d1);
	Busy.addDuration(*d2);
	Busy.addDuration(*d3);
}


void Schedule::for3_0(Slot &s)
{
	if (for3_0_Mon_Wed_Fri(s))
	{
		return;
	}
	if (for3_0_Tue_Thu(s))
	{
		return;
	}
	if (for3_0_Mon_Fri(s))
	{
		return;
	}
	if (for3_0_Mon_Wed(s))
	{
		return;
	}

	cout << "Scheduler failed at 3! Sorry!" << endl;
	exit (1);
}	
			
bool Schedule::for3_0_Mon_Wed_Fri(Slot &s)
{
	for3Days(s);
}

bool Schedule::for3_0_Tue_Thu(Slot &s)
{
	return for2Days(s, 1);
}

bool Schedule::for2Days(Slot &s, int choice)
{
	float offset = (s.getNoOfHours() == 3)? 1.5 : 1;
	int day1, day2;
	switch (choice)
	{
	case 1:
		day1 = TUE; day2 = THU;
		break;
	case 2:
		day1 = MON; day2 = FRI;
		break;
	case 3:
		day1 = MON; day2 = WED;
		break;
	}

	Duration 	*d1 = new Duration(Timer::TimeToFloat(Time(day1, 800)), 
				Timer::TimeToFloat(Time(2, 800))+ offset), 
			*d2  = new Duration(Timer::TimeToFloat(Time(day2, 800)), 
				Timer::TimeToFloat(Time(3, 800)) + offset); 

	while (!((	Timer::isContiguous(*d1) &&
			Timer::isContiguous(*d2)) &&

			(s.isAvailable(*d1) && 
			s.isAvailable(*d2)) &&

			(Busy.isFree(*d1) && 
			 Busy.isFree(*d2))))
	{
		if(!	Timer::isContiguous(*d1) &&
			Timer::isContiguous(*d2)) 
		{
			*d1 = Timer::getNextContiguous(*d1);
			*d2 = Timer::getNextContiguous(*d2);
		}

		if(!	(s.isAvailable(*d1) && 
			s.isAvailable(*d2)))
		{
			*d1 = s.getNextAvailable(*d1);
			*d2 = s.getNextAvailable(*d2);
		}

		if(!	(Busy.isFree(*d1) && 
			 Busy.isFree(*d2)))
		{
			*d1 = Busy.getNextFree(*d1);
			*d2 = Busy.getNextFree(*d2);
		}

		if(!	(d1->End <= Timer::getEndOfDay(day1)) &&
			(d2->End <= Timer::getEndOfDay(day1)))
		{
			delete(d1); delete(d2);
			return false;
		}
	}

	s.addLectureSlot(*d1);
	s.addLectureSlot(*d2);
	
	Busy.addDuration(*d1);
	Busy.addDuration(*d2);
	return true;
}
bool Schedule::for3_0_Mon_Fri(Slot &s)
{
	return for2Days(s, 2);
}

bool Schedule::for3_0_Mon_Wed(Slot &s)
{
	return for2Days(s, 3);
}

void Schedule::for2_0(Slot &s)
{
	if (for2_0_Tue_Thu(s))
	{
		return;
	}
	if (for2_0_Mon_Fri(s))
	{
		return;
	}
	if (for2_0_Mon_Wed(s))
	{
		return;
	}

	cout << "Scheduler failed at 2! Sorry!" << endl;
	exit (1);
}
	
bool Schedule::for2_0_Tue_Thu(Slot &s)
{
	return for2Days(s, 1);
}

bool Schedule::for2_0_Mon_Fri(Slot &s)
{
	return for2Days(s, 2);
}

bool Schedule::for2_0_Mon_Wed(Slot &s)
{
	return for2Days(s, 3);
}

ostream& operator << (ostream& o, const Schedule& s)
{
	for (int i = 0; i < s.theSchedule.size(); i++)
	{
		o << endl << "Slot " << i + 1 << " : " << endl;
		o << s.theSchedule[i];
	}
	return o;
}

Schedule& Schedule::operator = (const Schedule& source)
{
	if(Id == -1)
	{
		Id = source.Id;
	}
	theSchedule = source.theSchedule;
	return *this;
}

extern int yylex();
int Schedule::load(string aFileName)
{
	if ((fin = fopen (aFileName.c_str(), "r")) == 0)
	{
		cerr << "File " << aFileName << " not found!" << endl;
		return -1;
	}

	theSchedule.clear();
	token = yylex();
	parseSlotList();
	return Id;
}

void Schedule::parseSlotList()
{
	while (token == ID && input_string == "Slot")
	{
		parseSlot();
	}
	if (token == 0)
	{
		return;
	}
	//else something is wrong
	cout << "Error after SlotList!" << endl;
	exit(1);
}

void Schedule::parseSlot()
{
	Slot *slot = new Slot();

	if ((token = yylex()) != NUMBER)
	{
		cerr << "Error in Slot!" << endl;
		exit (1);
	}
	if ((token = yylex()) != ':')
	{
		cerr << "Error in Slot!" << endl;
		exit (1);
	}
	if ((token = yylex()) != ID || input_string != "Subjects")
	{
		cerr << "Error in Slot!" << endl;
		exit (1);
	}
	if ((token = yylex()) != ':')
	{
		cerr << "Error in Slot!" << endl;
		exit (1);
	}
	parseSubList(*slot);
	
	if ((token = yylex()) != ':')
	{
		cerr << "Error in Slot!" << endl;
		exit (1);
	}
	token = yylex();
	parseLectureList(*slot);
	
	if ((token = yylex()) != ID || input_string != "for")
	{
		cerr << "Error in Slot!" << endl;
		exit (1);
	}
	if ((token = yylex()) != ':')
	{
		cerr << "Error in Slot!" << endl;
		exit (1);
	}
	token = yylex();
	parseUnavailableList(*slot);

	theSchedule.push_back(*slot);
}

void Schedule::parseSubList(Slot &aSlot)
{
	while (true)
	{
		if ((token = yylex()) != ID)
		{
			cerr << "Error in Subject List!" << endl;
			exit(1);
		}
		else if (input_string == "Lectures")
		{
			return;
		}
		else
		{
			aSlot.addSubject(input_string);
			continue;
		}
	}
}

void Schedule::parseLectureList(Slot &aSlot)
{
	if ((token == ID) && input_string == "Unavailable")
	{
		return;
	}
	parseLecture(aSlot);
}

void Schedule::parseUnavailableList(Slot &aSlot)
{
	if ((token == ID) && input_string == "Slot")
	{
		return;
	}
	parseUnavailable(aSlot);
}

void Schedule::parseLecture(Slot &aSlot)
{
	Time t1, t2;
	switch (token)
	{
	case MON: case TUE: case WED: case THU: case FRI:
		t1.WeekDay = token;
		break;
	default:
		cerr << "Error in Lecture!" << endl;
		exit(1);
	}
	if ((token = yylex()) != NUMBER)
	{
		cerr << "Error in Lecture!" << endl;
		exit (1);
	}
	t1.Hour = (int)input_number;
	if ((token = yylex()) != '-')
	{
			cerr << "Error in Lecture!" << endl;
			exit (1);
	}			
	token = yylex();
	switch (token)
	{
		case MON: case TUE: case WED: case THU: case FRI:
			t2.WeekDay = token;
			break;
		default:
			cerr << "Error in Lecture!" << endl;
			exit (1);
	}
	if ((token = yylex()) != NUMBER)
	{
		cerr << "Error in Lecture!" << endl;
	}
	t2.Hour = (int)input_number;
	
	Duration *d = new Duration(Timer::TimeToFloat(t1), 
			Timer::TimeToFloat(t2));
	aSlot.addLectureSlot(*d);
	if ((token = yylex()) == ID && input_string == "Unavailable")
	{
		return;
	}
	else 
	{
		parseLectureList(aSlot);
	}
}

void Schedule::parseUnavailable(Slot &aSlot)
{
	Time t1, t2;
	switch (token)
	{
	case MON: case TUE: case WED: case THU: case FRI:
		t1.WeekDay = token;
		break;
	case 0:
		return;
	default:
		cerr << "Error in Unavailable!" << endl;
		exit(1);
	}
	if ((token = yylex()) != NUMBER)
	{
		cerr << "Error in Unavailable!" << endl;
		exit (1);
	}
	t1.Hour = (int)input_number;
	if ((token = yylex()) != '-')
	{
			cerr << "Error in Unavailable!" << endl;
			exit (1);
	}			
	token = yylex();
	switch (token)
	{
		case MON: case TUE: case WED: case THU: case FRI:
			t2.WeekDay = token;
			break;
		default:
			cerr << "Error in Unavailable!" << endl;
			exit (1);
	}
	if ((token = yylex()) != NUMBER)
	{
		cerr << "Error in Unavailable!" << endl;
	}
	t2.Hour = (int)input_number;
	
	Duration *d = new Duration(Timer::TimeToFloat(t1), 
			Timer::TimeToFloat(t2));
	aSlot.addUnavailable(*d);
	if ((token = yylex()) == ID && input_string == "Slot")
	{
		return;
	}
	else 
	{
		parseUnavailableList(aSlot);
	}
}
