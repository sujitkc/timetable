#include <stdio.h>
#include <iostream>
#include "Period.h"
#include "Timer.h"
#include "Participant.h"

FILE *fin;
extern int yylex();
extern void parseParticipant();
int main()
{
	fin = fopen ("Participant.dat", "r");
	int i;
	while ((i = yylex()) != 0)
	{
		printf("i = %d\n", i);
	}
	cout << "i on exit is " << i << endl;

/*
	Period p1, p2;
	Duration d1(1, 3);
	Duration d2(2, 4);
	p1.addDuration(d1);
	p2.addDuration(d2);

	cout << "p1 : " << p1 << endl;

	p2 = p2 || p1;
	cout << "p2 : " << p2 << endl;
 	Duration d2(3, 4);
	p1.addDuration(d2);
	Duration d3(5, 6);
	p1.addDuration(d3);
	Duration d4(7, 8);
	p1.addDuration(d4);

	Duration d5(9, 20);
	p2.addDuration(d5);
	Duration d6(11, 12);
	p2.addDuration(d6);
	Duration d7(13, 14);
	p2.addDuration(d7);
	Duration d8(15, 16);
	p2.addDuration(d8);
	Period p;
	p = p1 || p2;

*/
/*
	Timer t;
	Duration d(1, 4);	
	cout << "d is ";
	if(t.isContiguous(d))
	{
		cout << "contiguous" << endl;
	}
	else
	{
		cout << "not contiguous" << endl;
	}	

	float n = 21;
	cout << n << " means ";
	Time tm = t.FloatToTime(n);
	cout << "day " << tm.WeekDay << "Hour " << tm.Hour << endl;
	tm = Time(3, 1400);
	n = t.TimeToFloat(tm);
	cout << "converts to " << n << endl;

	fin = fopen ("Participant.dat", "r");
	parseParticipant();
	return 0;
*/
}

