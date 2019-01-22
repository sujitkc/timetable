#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <map>
#include "Schedule.h"
#include "DepGraph.h"

using namespace std;
/*
 * Class acting as a facade for the admin UI. This will contain
 * multiple schedules. It will manage them and will provide the UI an
 * interface to the UI to do the following:
 * 	* Getting relevant information from the Scheduler
 * 	* Validating a change made by the UI 
 * 	* Making changes to a schedule.
*/

class Scheduler
{
private:
	Schedule OriginalSchedule;
	DepGraph theGraph;
	map <int, Schedule> OtherSchedules;

public:
	Scheduler();
	~Scheduler();
	int createNewSchedule(int); //creates a new copy of the given schedule and 
															//returns its id.
	void saveSchedule(int, string); //saves a schedule into the file of the
																	//given name.
	int loadSchedule(string); //loads a schedule from the file of given name and
										//returns its id.
	int loadSchedule(int, string); //loads a schedule from the file of given 
																//name into the existing schedule of
																//given id.
	void removeSchedule(int); //removes the schedule of the given number from the
														//the list.
	Schedule& getSchedule(int);	//returns the Schedule with the given id. 
};
#endif
