#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "Participant.h"
#include "Subject.h"
#include "Global.h"
#include "Scheduler.h"

Scheduler::Scheduler()
{
	::loadParticipants();
	::loadSubjects();
	::creatAssoc();
	Schedule theSchedule;
	theGraph.buildGraph(Subjects);
	theGraph.colour();
	OriginalSchedule.makeSchedule(theGraph);
}

Scheduler::~Scheduler()
{
	cout << "removing " << OtherSchedules.size() << " elements..." << 
		endl;
	OtherSchedules.clear();
}

int Scheduler::createNewSchedule(int aId)
{
	static int CurrentScheduleNo = 0;
	CurrentScheduleNo++;
	OtherSchedules[CurrentScheduleNo] = Schedule(CurrentScheduleNo);
	if (aId == 0)
	{
		OtherSchedules[CurrentScheduleNo] = OriginalSchedule;
	}
	else
	{
		OtherSchedules[CurrentScheduleNo] = OtherSchedules[aId];
	}
	return CurrentScheduleNo;
}

void Scheduler::saveSchedule(int aId, string aFileName)
{
	ofstream fos (aFileName.c_str());
	if (aId == 0)
	{
		fos << OriginalSchedule;
	}
	else
	{
		fos << OtherSchedules[aId];
	}
	fos.close();
}

int Scheduler::loadSchedule(string aFileName)
{
	int New = createNewSchedule(0);
	return loadSchedule(New, aFileName);
}

int Scheduler::loadSchedule(int aId, string aFileName)
{
	return OtherSchedules[aId].load(aFileName);	
}

void Scheduler::removeSchedule(int aId)
{
	OtherSchedules.erase(aId);
}

Schedule& Scheduler::getSchedule(int aId)
{
	return OtherSchedules[aId];
}
