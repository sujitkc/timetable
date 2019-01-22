#include "Participant.h"

Participant::~Participant()
{

}

void Participant::makeUnavailable(Duration& d)
{	
	Unavailable.addDuration(d);	
}

void Participant::print()
{
	cout << "I am a participant." << endl;
	cout << "My Id is " << Id << "." << endl;
	if(subjects.size() > 0)
	{
		cout << "Subjects are : ";
		for (int i = 0; i < subjects.size(); i++)
		{
			cout << subjects[i] << '\t';
		}
		cout << endl << endl;
	}
	cout << "I am unavailable for : " << endl;
	cout << Unavailable;
}
