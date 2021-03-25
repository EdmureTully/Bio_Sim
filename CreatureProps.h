#ifndef BIOSIM_H
#define BIOSIM_H

#include "Vector2D.h"
#include <string>
#include <vector>
#include <gl\gl.h>
#include <gl\glu.h>  
/* Die Klasse enthält als Member die eigenschaften einer Kreature. Diese 
sind Eigenschaft Stärke, (aktuelle)Lebensdauer, Geschwindigkeit, Zustand,
auf Wanderung, Dauer für das Ausruhen,einen Pointer für mögliche Beute,
Todeszeitpunkt, Wanderziel und Textur.
Für diese Member besitzt sie Get- und Set-Methoden.
*/

enum Zustand{
	BIOSIM_WARTEN,
	BIOSIM_WACHSEN,
	BIOSIM_AUSRUHEN,
	BIOSIM_WANDERN,
	BIOSIM_JAGEN,
	BIOSIM_PAARUNG,
	BIOSIM_ANGRIFF,
	BIOSIM_TOT
};


struct CreatureProps{
public:
	CreatureProps(const std::string &name, int strength, int speed, int lifetime, const std::string &properties, const std::string &path) :
		creatureName(name), strength(strength), speed(speed), lifetime(lifetime), properties(properties), path(path), actualLifetime(lifetime), lastReproduction(0)
	{
		pray = NULL;
		wandering = false;
		createMoveTable();
	};

	~CreatureProps(){};
	CreatureProps(){};
	CreatureProps& operator=(CreatureProps& element);

	std::vector<Vector2D> wayToGo;
	const std::string path;
	CreatureProps* pray;
	GLuint getTexture();
	void setX(int x);
	void setY(int y);
	int getX();
	int getY();
	void setTexture(GLuint texture);
	int getSpeed();
	int getStrength();
	void setId(int ID);
	int getId();
	void setZustand(Zustand zustand);
	Zustand GetZustand();
	void setReproduction(int time);
	int getLastReproduction();
	std::string getName();
	std::string getProperties();
	int getMovement(int terrain);
	void setSelectionnumber(int num);
	int getSelectionnumber();
	int getActualLifetime();
	void setActualLifetime(int time);
	int environment;
	int getLifetime();
	void setSleep(int time);
	void setWandering(bool wander);
	int getSleep();
	bool getWandering();
	void setDeath(int time);
	void setGoal(int x, int y);
	Vector2D getGoal();
	int getDeath();
	
private:
	Vector2D goal;
	int death;
	bool wandering;
	int sleep;
	int actualLifetime;
	int lifetime;
	int id;
	int selectionnumber;
	void createMoveTable();
	std::vector<int> moveTable;
	GLuint texture;
	int strength;
	int speed;
	std::string creatureName;
	std::string properties;
	int xpos;
	int ypos;
	int lastReproduction;
	Zustand zustand;
};
bool checkLetter(const std::string &s, int state);
bool checkInput(std::string &s, int n, std::vector<CreatureProps> &cVector);
bool checkUmlaut(char c);
bool checkNumber(const std::string &s);
bool checkEnding(const std::string &s);
void printCreatures(std::vector<CreatureProps> &v);



std::vector <CreatureProps> readInData(const std::string &adress);
void printError(int linecounter, int column);
#endif