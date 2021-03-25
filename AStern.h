
#include "Vector2D.h"
#include "Node.h"
#include "CreatureProps.h"
#include "Map.h"
#include <vector> 

#ifndef BIOSIM_ASTERN_H
#define BIOSIM_ASTERN_H
#define MAX_CONST 100
/*Klasse Pathfinder enth�lten einen �ffentlichen Member f�r die Karte auf welcher der Pfad
gefunden werden soll, zwei boolesche Member f�r den Fall, dass die Suche initialisiert wurde 
und einen, f�r den Fall, dass sie erfolgreich war. Au�erdem einen Vektor welcher die Knoten
bis zum Ziel enth�lt. Als private Member enth�lt sie die Textur zur Anzeige des Pfades, einen
Startknoten und einen Endknoten. Au�erdem einen Vektor f�r die noch zu besuchenden Knoten und
einen f�r die schon besuchten. Als �ffentliche Methoden enth�lt sie Methoden zum Leeren der 
Vektoren, die Hauptmethode zum Finden des Pfades. Als private Mehoden enth�lt sie eine f�r 
das Setzen des Startpunktes, eine zum �berpr�fen, ob ein neuer Knoten betrachtet werden sollte,
eine zum Setzen der Pfadtextur, eine um den n�chsten Knoten der Openlist zu erhalten und eine,
welche den n�chsten Teil des Pfades berechnet.
*/
class PathFinder{
public:
	PathFinder(Map &map);
	~PathFinder(void);
	bool findPath(CreatureProps &creature, Vector2D goal);
	Vector2D NextPathPos();

	void clearOpenList(){ openList.clear(); }
	void clearVisitedList(){ visitedList.clear(); }
	void clearPathToGoal(){ pathToGoal.clear(); }
	bool initializedStart;
	bool foundGoal;
	std::vector<Vector2D*> pathToGoal;
	Map map;
private:
	void setStart(Node start, Node end);
	void pathOpen(int x, int y, float nCost, Node* parent);
	void setTexture(std::vector <GLuint> &textures);
	Node* getNextNode();
	void ContinuePath(CreatureProps &creature);
	GLuint astarTexture;
	Node* startNode;
	Node* goalNode;
	std::vector<Node*> openList;
	std::vector<Node*> visitedList;
	

};


#endif