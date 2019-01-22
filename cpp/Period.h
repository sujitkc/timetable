#ifndef PERIOD_H
#define PERIOD_H
#include<iostream>
#include <vector>

using namespace std;
class Duration
{
public:
	float Start;
	float End;
	
	Duration(float s = 0, float e = 0) : Start(s), End(e){};
	bool isContained(float f)
	{	return ((f > Start) && (f < End));	}

	bool isOverlap(Duration d);

	bool operator == (Duration ad){ return ((ad.Start == Start) && 
		(ad.End == End));	}
};

ostream& operator << (ostream&, const Duration&);

class Period
{
private:
	friend ostream& operator << (ostream&, const Period&);
	vector <Duration> durations;

public:
	~Period();
	void addDuration(Duration d);
	Period& operator || (Period&);
	bool isFree(Duration);
	Duration getNextFree(Duration); //the next duration of the same length
			//as the argument which is not inside this
};

ostream& operator << (ostream&, const Period&);
#endif
