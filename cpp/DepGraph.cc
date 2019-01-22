#include <iostream>
#include "DepGraph.h"
#include "Global.h"

using namespace std;
DepGraph::DepGraph()
{
	size = 0;
}

void DepGraph::buildGraph(vector<Subject>& s)
{

/*
	for each subject
		check all the previous subjects in Subjects
		if there is any subjects shared by these two subjects
			make the corresponding array elements true
	end for
*/

	//build the dependency graph rightaway
	size = s.size();
	array = new bool * [size];

	for (int i = 0; i < size; i++)
	{
		array[i] = new bool [size];
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			for (int k = 0; 
			k < Subjects[i].getParticipants().size(); k++)
			{
				for (int l = 0; 
				l < Subjects[j].getParticipants().size(); l++)
				{
					if(Subjects[i].getParticipants()[k].
							getId() == 
					Subjects[j].getParticipants()[l].getId())
					{
						array[i][j] = true;
						array[j][i] = true;
						break;
					}
				}
				if(array[i][j] == true)
				{
					break;
				}
			}
		}
	}

	Nodes = vector<Node> (size);
	for (int i = 0; i < size; i++)
	{
		Nodes[i].Colour = 0;
		Nodes[i].subject = &s[i];
	}
}

DepGraph::~DepGraph()
{
	for (int i = 0; i < size; i++)
	{
		delete[] array[i];
	}
	if (size != 0)
	{
		delete[] array;
	}
}

void DepGraph::print()
{
	cout << "dependency graph size : " << size << " X " << size 
		<< "!" << endl;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			cout << "[" << i << "][" << j << "] = " ;
			if (array [i][j] == true)
			{
				cout << "true" << endl;
			}
			else
			{
				cout << "false" << endl;
			}
		}
	}
}	

int DepGraph::colour()
{
/*
 * for each node
 * 	if there is there exists any colour from 1 to MaxColour that is not
 * 	used by any of the neighbours
 * 		assign that colour
 * 	else
 * 		assign MaxColour + 1 to the current node
 * 		MaxColour++
 * end for
 */
	MaxColour = 1;
	for(int i = 0; i < size; i++)
	{
		int j;
		for (j = 1; j <= MaxColour; j++)
		{
			int k;
			for (k = 0; k < i; k++)
			{
				if (array[k][i] == true && 
					Nodes[k].Colour == j)
				{
					break;
				}
			}
			if(k == i)
			{
				Nodes[i].Colour = j;
				break;
			}
			else
			{
				continue;
			}
		}		
		if(j >= MaxColour)
		{
			Nodes[i].Colour = MaxColour = j;
		} 	
	}
	IsColoured = true;
	return MaxColour;
}
