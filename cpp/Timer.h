#ifndef TIMER_H
#define TIMER_H

#include <vector>
#include "Period.h"

/*This class Timer is intended to encapsulate all the information about the 
 * timing
 * of the business in the institute. The rest of the entities work with time as
 * just number lines. This module houses the rules that will be used to convert
 * these numbers back and forth to the time representation. The main reason for
 * having this class is that we thus incorporate a lot of flexibility in 
 * deciding the institute timings. All that business knowledge is centralised
 * here in this class.
*/

class Time
{
public:
	int WeekDay;
	int Hour;
	Time(int w = 1, int h = 800) : WeekDay(w), Hour(h){}
};

ostream& operator << (ostream&, const Time&);

class TimeDuration
{
public:
	int time;
	float length;
	TimeDuration(){}
	TimeDuration(int t, float l) : time(t), length(l){};
};

class FloatDuration
{
public:
	float Start;
	float Length;
}; 

class Timer
{
private:
	static float HoursPerDay; //Number of working hours per day, lunch time not
				//counted.
	static int StartOfDay;
	static int EndOfDay;
	static TimeDuration Lunchbreak;
	static vector<FloatDuration> breaks;	//List of break durations
	static void init();
	static bool Initialised;	
public:
	Timer();
	static float TimeToFloat(Time);
	static Time FloatToTime(float);
	static Time FloatToTime(float, bool);
	static bool isContiguous(Duration);
	static Duration getNextContiguous(Duration);
	static Duration getPrevContiguous(Duration); 
	static float getStartOfDay(int ad){ return (ad - 1) * HoursPerDay;	}
	static float getEndOfDay(int ad){	return ad * HoursPerDay;	}
};

#endif
