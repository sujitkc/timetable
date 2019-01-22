#include "Subject.h"

void Subject::addParticipant(Participant& p)
{	
	participants.push_back(p);	

	Unavailable = Unavailable || p.getUnavailable();
}

void Subject::print()
{
	cout << "subjectID : "<< Id << "." << endl;
	if(participants.size() > 0)
	{
		cout << "Participants : ";
		for (int i = 0; i < participants.size(); i++)
		{
			cout << participants[i].getId() << '\t';
		}
	
		cout << "I am unavailable for the following:" << endl;
		cout << Unavailable;
	}	

}	
