#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <string>
#include "Period.h"
#include "DepGraph.h"
class Slot
{
public:
	
	vector<string> subjects;
	Duration duration;
	Period Unavailable;

	vector<Duration> Lectures;

	~Slot(){	subjects.clear(); Lectures.clear();	}
	bool isAvailable(Duration d){	return Unavailable.isFree(d);	}	
	Duration getNextAvailable(Duration);
	float getNoOfHours(){ return duration.End - duration.Start;	}
	float schedule(Duration d);
	void addSubject(string s){	subjects.push_back(s);	}
	void addUnavailable(Duration d){	Unavailable.addDuration(d);	}
	void addLectureSlot(Duration d){	Lectures.push_back(d);	}
};

ostream& operator << (ostream&, const Slot&);

class Schedule
{
private:
	friend ostream& operator << (ostream&, const Schedule&);
	
	int Id;
	vector<Slot> theSchedule;
	
	
	bool for2Days(Slot &, int);
	Period Busy;

public: 
	Schedule();
	Schedule(int aId) : Id(aId){}
	~Schedule(){	theSchedule.clear();	}
	void addBusy(Duration);
	bool isFree(Duration d){	return Busy.isFree(d);	}
	void makeSchedule(DepGraph &);
private:
	void for4_5(Slot &);
	void for3_0(Slot &);
	bool for3_0_Mon_Wed_Fri(Slot &);
	bool for3_0_Tue_Thu(Slot &);
	bool for3_0_Mon_Fri(Slot &);
	bool for3_0_Mon_Wed(Slot &);

	void for2_0(Slot &);
	bool for2_0_Tue_Thu(Slot &);
	bool for2_0_Mon_Fri(Slot &);
	bool for2_0_Mon_Wed(Slot &);

	void for3Days(Slot &);
public:
	Schedule& operator = (const Schedule&);
	
	int getId(){	return Id;	}

//interface with UI
	int load(string);
private:
//for parsing schedule file
	int token;
	void parseSlotList();
	void parseSlot();
	void parseSubList(Slot&);
	void parseLectureList(Slot&);
	void parseUnavailableList(Slot&);
	void parseLecture(Slot&);
	void parseUnavailable(Slot&);
};

ostream& operator << (ostream&, const Schedule&);
#endif
