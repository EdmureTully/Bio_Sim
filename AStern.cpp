#include "stdafx.h"
#include "AStern.h"
#include <math.h>
#include <list>
#include <vector>
#include <algorithm>

//Vergelicht zwei Knoten bez�glich ihrer Manhattandistanz
bool compareNodes(Node *a, Node *b){
	return(a->H < b->H);
}

PathFinder::PathFinder(Map &map):map(map){
	initializedStart = false;
	foundGoal = false;
}

PathFinder:: ~PathFinder(void){

}
/*Als �bergabeparamter erh�lt die Methode eine Referenz
auf eine Kreaturenisntanz sowie die Zielkachel.
L�scht die vorhandenen Vektoren und initiiert den Start.
Ruft k-mal(wobei k eine Konstante) die Methode ContinuePath
auf. Falls Pfad nicht gefunden wurde, wird eine Meldung ausgegeben.
*/

bool PathFinder::findPath(CreatureProps &creature, Vector2D goal){
	int k = 0;
	if (!initializedStart){
		for (unsigned int i = 0; i < openList.size(); i++){
			delete openList[i];
		}
		openList.clear();

		for (unsigned int i = 0; i < visitedList.size(); i++){
			delete visitedList[i];
		}

		visitedList.clear();
		for (unsigned int i = 0; i < pathToGoal.size(); i++){
			delete pathToGoal[i];
		}
		pathToGoal.clear();

		//Initialisiere Start
		Node start;
		start.node_x = static_cast<int>(abs(round(creature.getX())));
		start.node_y = static_cast<int>(abs(round(creature.getY())));

		//Initialisiere Start
		Node goalNode;
		goalNode.node_x = goal.x;
		goalNode.node_y = goal.y;

		setStart(start, goalNode);

		initializedStart = true;
	}
	//Solange Knoten in der Openlist und Konstante noch nicht erreicht wird weitergesucht.
	while (!openList.empty() && k < MAX_CONST){
		ContinuePath(creature);
		k++;
	}
	//Falls Konstante erreicht wird false zur�ckgegeben.
	if ((k == MAX_CONST) || !foundGoal ){
		//MessageBox(NULL, L"Weg konnte nicht gefunden werden.", L"Hinweis", MB_OK);
		return false;
	}
	return true;
}

//Setzt die Member der Start- und Zielknoten.
void PathFinder::setStart(Node start, Node end){
	startNode = new Node(start.node_x, start.node_y, 0);
	goalNode = new Node(end.node_x, end.node_y, &end);

	startNode->G = 0;
	startNode->H = startNode->ManhattanDisance(goalNode);
	startNode->parent = 0;

	openList.push_back(startNode);
}
//Gibt den Knoten zur�ck, welcher den kleinsten Wert der
//Summe aus Manhattandistanz und Kosten zur�ck. L�scht 
//diesen aus der Openlist.
Node* PathFinder::getNextNode(){
	float greatF = 9000.f;
	int nodeIndex = -1;
	Node* nextNode(NULL);

	for (unsigned int i = 0; i < openList.size(); i++){
		if (openList[i]->GetF() < greatF){
			greatF = openList[i]->GetF();
			nodeIndex = i;
		}
	}

	if (nodeIndex >= 0){
		nextNode = openList[nodeIndex];
		visitedList.push_back(nextNode);
		openList.erase(openList.begin() + nodeIndex);

	}

	return nextNode;
}

/*�berp�ft anhand de�s x-und y-Wertes der Knoten schon einmal besucht
wurde. Ist dies nicht der Fall, wird ein neuer Knoten mit den �ber-
gebenen Parametern erzeugt. Anschlie�end wird �berpr�ft, ob dieser 
schon in der Openlist enthalten ist. Ist dies der Fall, wird jener
mit den besseren Werten weiter genutzt.
*/
void PathFinder::pathOpen(int x, int y, float nCost, Node* parent){

	int id = y * WORLD_SIZE + x;
	for (unsigned int i = 0; i < visitedList.size(); i++){
		if (id == visitedList[i]->node_id){
			return;
		}
	}
	
	Node* nChild = new Node(x, y, parent);
	nChild->G = nCost;
	nChild->H = nChild->ManhattanDisance(goalNode);

	for (unsigned int i = 0; i < openList.size(); i++){
		if (id == openList[i]->node_id){
			float newF = nChild->G + nCost + openList[i]->H;

			if (openList[i]->GetF() > newF){
				openList[i]->G = nChild->G + nCost;
				openList[i]->parent = nChild;
			}
			else // F ist nicht besser
			{
				delete nChild;
				return;
			}
		}
	}
	openList.push_back(nChild);
}
/*F�hrt die Pfadsuche f�r die �bergebene Kreatur weiter, falls
die Openliste nicht leer ist. Entnimmt den n�chsten Knoten aus 
der Openlist und �berpr�ft, ob es sich um den Zielknoten  handelt.
Ist dies der Fall werden alle Vorg�ngerknoten in PathToGoal-Vektor
�bernommen. Ansonsten wird �berpr�ft, ob die umliegenden Kacheln f�r
die Kreatur erreichbar sind. Die erreichbaren werden dann gegebenenefalls
in die Openlist �bernommen. Abschlie�end wird noch �berpr�ft, ob ein
Knoten eventuell doppelt in der Openlist existiert. Ist dies der Fall,
wird er gel�scht.
*/
void PathFinder::ContinuePath(CreatureProps &creature){
	//for int i < 4??
	if (openList.empty()){
		return;
	}
	
	Node* currentNode = getNextNode();
	//Zur Sicherheit wird �berpr�ft, ob �bergebener Knoten existiert.
	if (currentNode == NULL) return;
	//Falls Knoten = Zielknoten wird der Pfad in den PathToGoal-Vektor gepusht.
	if (currentNode->node_id == goalNode->node_id){
		goalNode->parent = currentNode->parent;

		Node* getPath;

		for (getPath = goalNode; getPath != NULL; getPath = getPath->parent){
			pathToGoal.push_back(new Vector2D(getPath->node_x, getPath->node_y));
		}
		openList.clear();
		foundGoal = true;
		return;
	}
	else{
		bool rightUnblocked = false, leftUnblocked = false, upUnblocked = false, downUnBlocked = false;
		//umliegende Felder werden auf Begehbarkeit �berpr�ft.
		if (currentNode->node_x < map.getMapHeight() - 1 && currentNode->node_y < map.getMapWidth() - 1)
			rightUnblocked = true;
		if (currentNode->node_x - 1 >= 0 && currentNode->node_y < map.getMapWidth() - 1 && currentNode->node_x < map.getMapHeight() - 1)
			leftUnblocked = true;
		if (currentNode->node_y < map.getMapWidth() - 1 && currentNode->node_x < map.getMapHeight() - 1)
			upUnblocked = true;
		if (currentNode->node_y - 1 >= 0 && currentNode->node_x < map.getMapHeight() - 1 && currentNode->node_y < map.getMapWidth() - 1)
			downUnBlocked = true;
		//rechtes Feld wird �berpr�ft. Falls begehbar wird es mittels pathOpen-Funktion der Openlist hinzugef�gt.
		if (rightUnblocked){
			if (creature.getMovement(map.getField(currentNode->node_x + 1, currentNode->node_y))>0)
				pathOpen(currentNode->node_x + 1, currentNode->node_y, currentNode->G + creature.getMovement(map.getField(currentNode->node_x + 1, currentNode->node_y)), currentNode);
		}
		//linkes Feld wird �berpr�ft. Falls begehbar wird es mittels pathOpen-Funktion der Openlist hinzugef�gt.
		if (leftUnblocked){
			if (creature.getMovement(map.getField(currentNode->node_x - 1, currentNode->node_y)) > 0)
				pathOpen(currentNode->node_x - 1, currentNode->node_y, currentNode->G + (creature.getMovement(map.getField(currentNode->node_x - 1, currentNode->node_y))), currentNode);
		}
		//oberes Feld wird �berpr�ft. Falls begehbar wird es mittels pathOpen-Funktion der Openlist hinzugef�gt.
		if (upUnblocked){
			if (creature.getMovement(map.getField(currentNode->node_x, currentNode->node_y + 1)) > 0 && currentNode->node_y < map.getMapWidth() - 1 && currentNode->node_x < map.getMapHeight() - 1)
				pathOpen(currentNode->node_x, currentNode->node_y + 1, currentNode->G + creature.getMovement(map.getField(currentNode->node_x, currentNode->node_y + 1)), currentNode);
		}
		//unteres Feld wird �berpr�ft. Falls begehbar wird es mittels pathOpen-Funktion der Openlist hinzugef�gt.
		if (downUnBlocked){
			if (creature.getMovement(map.getField(currentNode->node_x, currentNode->node_y - 1)) > 0)
				pathOpen(currentNode->node_x, currentNode->node_y - 1, currentNode->G + creature.getMovement(map.getField(currentNode->node_x, currentNode->node_y - 1)), currentNode);
		}
		//Pr�ft, ob Knoten doppelt vorhanden. Falls ja, wird einer gel�scht.
		for (unsigned int i = 0; i < openList.size(); i++){
			if (currentNode->node_id == openList[i]->node_id){
				openList.erase(openList.begin() + i);
			}
		}
	}
}

//Setzt die Pfadtextur
void PathFinder::setTexture(std::vector <GLuint> &textures){
	astarTexture = textures[0];
	textures.erase(textures.cbegin());
}

