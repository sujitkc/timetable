#include "Timer.h"


float Timer::HoursPerDay;
int Timer::StartOfDay;
int Timer::EndOfDay;
TimeDuration Timer::Lunchbreak;
vector<FloatDuration> Timer::breaks;	
bool Timer::Initialised = false;	

void Timer::init()
{
	HoursPerDay = 8.0; 
	StartOfDay = 800; 
	EndOfDay = 1700; 
	Lunchbreak = TimeDuration(1300, 1.0);

	for (int i = 0; i < 5; i++)
	{
		FloatDuration d1, d2;
		d1.Start = i * HoursPerDay + 5;
		d1.Length = 1;
		breaks.push_back(d1);
		
		d2.Start = (i + 1) * HoursPerDay;
		d2.Length = 0;
		breaks.push_back(d2);
	}
	Initialised = true;
}

float Timer::TimeToFloat(Time aTime)
{
	if(!Initialised)
	{
		init();
	}
	float fltm = (aTime.WeekDay - 1) * HoursPerDay;
	int TimeToday = aTime.Hour - StartOfDay;
	fltm += (int)(TimeToday / 100.0);
	fltm += (TimeToday % 100) / 60.0;
	if (aTime.Hour >= Lunchbreak.time)
	{
		fltm -= Lunchbreak.length;
	}
}

Time Timer::FloatToTime(float aFloatTime)
{
	if(!Initialised)
	{
		init();
	}
	Time time;
	float fltm = aFloatTime;
	for (time.WeekDay = 1; ; time.WeekDay++)
	{
		fltm = fltm - HoursPerDay;
		if (fltm <= 0)
		{
			break;
		}
	}
	float NoOfHoursToday = aFloatTime - (time.WeekDay - 1)*HoursPerDay;
	time.Hour = StartOfDay + (int)NoOfHoursToday * 100 + 
		(NoOfHoursToday - (int)NoOfHoursToday) * 60;
	if (time.Hour >= Lunchbreak.time)
	{
		time.Hour += (int)Lunchbreak.length * 100 +
			(Lunchbreak.length - (int)Lunchbreak.length) * 60;
	}

	return time;
}

Time Timer::FloatToTime(float aFloatTime, bool flag)
{
	if(!Initialised)
	{
		init();
	}
	Time time;
	if ((aFloatTime / HoursPerDay) == ((int)(aFloatTime / HoursPerDay)))
	{
		if (flag == false)
		{
			time.WeekDay = (int)aFloatTime / HoursPerDay;
			time.Hour = EndOfDay;
			return time;
		}
		else
		{
			time.WeekDay = (int)aFloatTime / HoursPerDay + 1;
			time.Hour = StartOfDay;
			return time;
		}

	}	
	float fltm = aFloatTime;
	for (time.WeekDay = 1; ; time.WeekDay++)
	{
		fltm = fltm - HoursPerDay;
		if (fltm <= 0)
		{
			break;
		}
	}
	float NoOfHoursToday = aFloatTime - (time.WeekDay - 1)*HoursPerDay;
	time.Hour = StartOfDay + (int)NoOfHoursToday * 100 + 
		(NoOfHoursToday - (int)NoOfHoursToday) * 60;
	if (time.Hour >= Lunchbreak.time)
	{
		time.Hour += (int)Lunchbreak.length * 100 +
			(Lunchbreak.length - (int)Lunchbreak.length) * 60;
	}

	return time;
}
//Is there any break in this duration?
bool Timer::isContiguous(Duration d)
{
	if(!Initialised)
	{
		init();
	}
	for (int i = 0; i < breaks.size(); i++)
	{
		if(d.isContained(breaks[i].Start))
		{
			return false;
		}
	}
	return true;		
}

//Returns the next earliest duration of the same length with no breaks
Duration Timer::getNextContiguous(Duration d)
{
	if(!Initialised)
	{
		init();
	}
	while (!isContiguous(d))
	{
		for (int i = 0; i < breaks.size(); i++)
		{
			if (breaks[i].Start > d.Start)
			{
				d.End += breaks[i].Start - d.Start;
				d.Start = breaks[i].Start;
				break;
			}
		}
	}

	return d;
}

//Returns the previous latest duration of the same length with no breaks
Duration Timer::getPrevContiguous(Duration d) 
{
	if(!Initialised)
	{
		init();
	}
	while (!isContiguous(d))
	{
		for (int i = breaks.size() - 1; i >= 0; i--)
		{
			if (breaks[i].Start < d.End)
			{
				d.Start -= d.End - breaks[i].Start;
				d.End = breaks[i].Start;
				break;
			}
		}
	}

	return d;

}

ostream& operator << (ostream& o, const Time& t)
{
	char * days[] =
	{
		"mon", "tue", "wed", "thu", "fri"
	};

	o << days[t.WeekDay - 1] << ' ' << t.Hour;
	return o;
}
