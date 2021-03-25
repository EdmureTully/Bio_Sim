#ifndef BIOSIM_NODE_H
#define BIOSIM_NODE_H

#include <math.h>
#define WORLD_SIZE 100
/*Die Klasse Node enthält öffentliche Member für x- und y-Postion,
einen Pointer auf den Elterknoten, einen Member für die Kosten des 
Knotens sowie einen Meber für die Manhattandistanz und eine Knoten_ID.
Außerdem eine Funktion um die Gesamtkosten und eine für die Berechnung 
der Manhattandistanz.
*/


class Node{
public:
	int node_x, node_y;
	int node_id;
	Node* parent;
	float G;
	float H;

	Node() : parent(0){};
	Node(int x, int y, Node* _parent = 0) :node_x(x), node_y(y), parent(_parent), node_id(y * WORLD_SIZE + x), G(0), H(0){};

	float GetF(){ return G + H; }
	float ManhattanDisance(Node* end){
		float x = (float)(abs(this->node_x - end->node_x));
		float y = (float)(abs(this->node_y - end->node_y));
		float manhattan = static_cast<float> (sqrt(x*x + y*y));
		return manhattan;
	}


};

#endif