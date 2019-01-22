#ifndef DEPGRAPH_H
#define DEPGRAPH_H

#include "Subject.h"
#include "DepGraph.h"

class Node
{
	public:
		int Colour;
		Subject *subject;
};

class DepGraph
{
private:
	bool **array; //contains the dependence status of the nodes
	int size;
	int MaxColour; //Maximum number of colours required to colour the 
			//graph
	bool IsColoured;
	
public:
	vector <Node> Nodes;
	
	DepGraph();
	void buildGraph(vector <Subject>&);
	~DepGraph();
	void print();	
	int colour();
	bool isColoured(){	return IsColoured;	}
	int getMaxColour(){	return MaxColour;	}
};
#endif
