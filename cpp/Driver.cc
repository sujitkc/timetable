#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "Participant.h"
#include "Subject.h"
#include "DepGraph.h"
#include "Global.h"
#include "Schedule.h"
#include "Scheduler.h"
#include <assert.h>

#define NDEBUG
int main()
{
	Scheduler S;
	S.saveSchedule(0, string("first.dat"));
	int i = S.createNewSchedule(0);
	i = S.createNewSchedule(1);
	S.saveSchedule(i, string("second.dat"));
	i = S.loadSchedule("second.dat");
	S.removeSchedule(i);
	S.saveSchedule(1, "third.dat");
	return 0;
}
