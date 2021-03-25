#ifndef BIOSIM_KI_H
#define BIOSIM_KI_H

#include "Map.h"
/*
Klasse steuert die Simulationslogik. Sie enth�lt einen Pointer
f�r die Map, einen Z�hler f�r die Schritte. Au�erdem einen 2-
dimensionalen Vektor um das setzen der Pflanzen zu erleichtern.
Sie enth�lt MEthoden f�r einen Simulationsschritt und dazugeh�rige
Methoden wie: die Suche nach BEute, das �berpr�fen der ZEit, die Suche
nach Fortpflanzungspartnern. Au�erdem Eine Funktion f�r das erstellen 
einer Zufallszahl.
*/
class Ki{
public:
	
	void simulatePlant(std::list<CreatureProps> & cVec);
	void simulateAnimals(std::list<CreatureProps> & cVect);
	~Ki(){};
	Ki();
	Ki(Map &map);
	void makeMove();
	int getSteps();
private:
	Map* map;
	int steps;
	bool checkAnimalTime(CreatureProps &c);
	void checkEnvironment(CreatureProps &c);
	
	bool checkSorroundingPlants(std::list<CreatureProps> &vec, CreatureProps &c);
	bool checkTime(CreatureProps &c);
	bool checkPray(std::list<CreatureProps> &vec, CreatureProps &c);
	bool checkMateAnimal(std::list<CreatureProps> &vec, CreatureProps &c);
	void createPlantMap();
	int getRandomNumber(int range);
	std::vector<std::vector<int>> plantMap;
};



#endif