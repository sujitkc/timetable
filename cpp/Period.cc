#include "Timer.h"
#include "Period.h"
#include <vector>

ostream& operator << (ostream& o, const Duration& d)
{
	o << Timer::FloatToTime(d.Start, true);
	o << " - " << Timer::FloatToTime(d.End, false) << endl;
	return o;
}

bool Duration::isOverlap(Duration d)
{
	if ((d.Start >= Start && d.Start < End) ||
			Start >= d.Start && Start < d.End)
       	{
		return true;
	}
	return false;
}
struct Dur
{
	float Inst;
	bool isStart;
};
typedef struct Dur Dur;

Period::~Period()
{
	durations.clear();
}
void Period::addDuration(Duration d)
{	
	for (int i = 0; i < durations.size(); i++)
	{
		if (d == durations[i])
		{
			return;
		}
	}
	durations.push_back(d);	
}

Period& Period::operator || (Period& P)
{
/*	create a vector v that contains all the Starts and Ends of all the
 *	durations in both this and P. The list is formed in a sorted manner.
 *	Each element of v also contains if it is a Start or End in any
 *	of the Durations of the two Periods.
 *
 *	Now scan v. Keep a counter (Level) such that Level increments on
 *	seeing a start and decrements on seeing an End.
 *
 *	Wherever Level changes from 0 -> 1 marks the Start of one Duration
 *	of the resulting Period.
 *	Wherever Level changes from 1 -> 0 marks the End of one Duration of 
 *	the resulting Period.
 *
 *	There are four three conditions
 *	all of this's points have been added to v.
 *	all of P's points have been added to v.
 *	Some points of both remain to be added. 
 *	As per above three we see the three conditions below.
 *	
 */
	vector<Dur> v;

	int d1 = 0, d2 = 0;
	bool isS1 = true;
	bool isS2 = true;

	float f1, f2;

	while (d1 < durations.size() || d2 < P.durations.size())
	{
		Dur *D;
		if(d1 == durations.size() && isS1 == true)
		{
			D = new Dur;
			if(isS2 == true)
			{
				D->Inst = P.durations[d2].Start;
				D->isStart = true;
				isS2 = false;
			}
			else
			{
				D->Inst = P.durations[d2].End;
				D->isStart = false;
				isS2 = true;
				d2++;
			}
			v.push_back(*D);
			continue;
		}
		else if(d2 == P.durations.size() && isS2 == true)
		{
			D = new Dur;
			if(isS1 == true)
			{
				D->Inst = durations[d1].Start;
				D->isStart = true;
				isS1 = false;
			}
			else
			{
				D->Inst = durations[d1].End;
				D->isStart = false;
				isS1 = true;
				d1++;
			}
			v.push_back(*D);
			continue;
		}
		f1 = (isS1 == true)? durations[d1].Start : durations[d1].End;
		f2 = (isS2 == true)? 
			P.durations[d2].Start : P.durations[d2].End;

		if(f1 <= f2)
		{
			if(isS1 == true)
			{
				D = new Dur;
				D->Inst = f1;
				D->isStart = true;				
				isS1 = false;
			}
			else
			{
				D = new Dur;
				D->Inst = f1;
				D->isStart = false;
				isS1 = true; d1++;
			}
		}	
		else
		{
			if(isS2 == true)
			{
				D = new Dur;
				D->Inst = f2;
				D->isStart = true;				
				isS2 = false;
			}
			else
			{
				D = new Dur;
				D->Inst = f2;
				D->isStart = false;
				isS2 = true; d2++;
			}
		}
		v.push_back(*D);
	}

	Period *p = new Period;
	int Level = 0;
	Duration *dn;
	for (int i = 0; i < v.size(); i++)
	{
		if(v[i].isStart == true)
		{
			if(Level++ == 0)
			{
				dn = new Duration;
				dn->Start = v[i].Inst;
			}
		}
		else
		{
			if(--Level == 0)
			{
				dn->End = v[i].Inst;
				p->addDuration(*dn);
			}
		}
	}
	return *p;
}

bool Period::isFree(Duration d)
{
	for (int i = 0; i < durations.size(); i++)
	{
		if (durations[i].isOverlap(d))
		{
			return false;
		}
	}
	return true;
}

Duration Period::getNextFree(Duration ad)
{
	Duration d = ad;
	for (int i = 0; i < durations.size(); i++)
	{
		if(durations[i].isOverlap(d))
		{
			d.End += durations[i].End - d.Start;
			d.Start = durations[i].End;
		}
	}
	return d;
}

ostream& operator << (ostream& o, const Period& p)
{
	for (int i = 0; i < p.durations.size(); i++)
	{
		o << p.durations[i];
	}
	return o;
}
