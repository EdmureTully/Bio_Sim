#include "BioSim-Win32.h"
#include "CreatureProps.h"
#include "Vector2D.h"
#include <list>


#ifndef BIOSIM_MAP
#define BIOSIM_MAP


/*Klasse Karte enthält Member für die Breite und Höhe der Karte, ferner
Koordinaten x,y falls gescrollt wurde. Außerdem die für die Karte
und Kreaturen notwendigen Texturen und die Karte, gespeichert als 2 dimensionaler vektor.
Dazu einen Vektor, welcher die gesetzten Kreaturen beinhaltet und einen int-Wert, welcher
die momentan ausgewählten Kreaturen Typ beinhaltet, einen weiteren Vektor zum speichern der 
Koordinate und der Selektionsnummer einer während der Simulation hinzugekommenen Kreatur.
Ferner eine Methode zum Setzen der einzigartigen Kreatur-Id und eine zum Löschen von
Kreaturen.Und Methoden zum Setzen und Abfragen der Koordinaten,  der Maße der 
Karte, der Karte, der Texturen und zum Setzen dieser. Außerdem einen Member der Klasse
Cursor, der eine eigene Texture besitzt und Member und Methoden zum Setzen des Cursors
auf dem Bildschirm. 
Enum Terrain enthält die verschiedenen Geländetypen.
*/
enum Terrain{
	BIOSIM_DEEP_SEA,
	BIOSIM_SHALLOW_WATER,
	BIOSIM_SAND,
	BIOSIM_EARTH,
	BIOSIM_ROCKS, 
	BIOSIM_SNOW
};

class Cursor{
public:
	Cursor();
	Cursor(bool isSet);
	~Cursor();
	void setActiv(bool activ);
	void setXpos(int x);
	void setYpos(int y);
	int getOldXpos();
	int getOldYpos();
	void setOldXpos(int x);
	void setOldYpos(int y);
	int getXpos();
	int getYpos();
	bool checkActiv();
	void setTexture(std::vector <GLuint> &textures);
	GLuint getTexture();
	bool getAStarActiv();
	void setAStarActiv(bool activ);
	void setAStar(bool astar);
	bool getAStar();
private:
	bool aStarActiv = false;
	bool astar;
	bool isSet;
	int xPos;
	int yPos;
	int oldXpos = -1;
	int oldYpos = -1;
	GLuint cursorTextur;
};




class Map{
public:
	Map(int width, int height);
	~Map();
	std::list<CreatureProps> landplants;
	std::list<CreatureProps> waterplants;
	std::list<CreatureProps> landanimals;
	std::list<CreatureProps> wateranimals;
	std::vector <CreatureProps> vCreature;//Vektor zum Platzieren der Kreaturen
	std::vector <Vector2D> tempPos;
	void setSelection(int selected);
	int getSelection();
	void setTextures(std::vector <GLuint> &textures);
	void setCreatureTextures(std::vector <GLuint> &textures);
	void setPathTexture(std::vector <GLuint> &textures);
	void setXpos(int x);
	void setYpos(int y);
	int getYpos();
	int getXpos();
	int getMapHeight();
	int getMapWidth();
	std::vector <GLuint> getTextures();
	int countTextures();
	std::vector<std::vector<int>> getMap();
	void SetCursorTextur(Cursor &cursor, std::vector < GLuint> &textures);
	Cursor& getCursor();
	std::vector<CreatureProps> vCreat;
	void pushCreature( int x, int y, CreatureProps c);
	Terrain getField(int x, int y);
	void setPreselection(int sel);
	int getPreselection();
	GLuint getPathTexture();
	int getNextID();
	void setDeadTexture(std::vector <GLuint> &textures);
	GLuint getDeadTexture();
	int getonMap();
	std::vector<std::vector<int>> map;
	void deleteCreature(CreatureProps &c);
	std::list<CreatureProps>::iterator deleteLandplant(CreatureProps &c);
	std::list<CreatureProps>::iterator deleteLandanimal(CreatureProps &c);
	std::list<CreatureProps>::iterator deleteWateranimal(CreatureProps &c);
	std::list<CreatureProps>::iterator deleteWaterplant(CreatureProps &c);
	void setSteps(int step);
	int getSteps();
private:
	int creatureID;
	GLuint pathTexture;
	GLuint deadTexture;
	Cursor cursor;
	std::vector<GLuint> texture ;
	int preselection;
	int xpos;
	int ypos;
	int selection = -1;
	int width;
	int height;
	void createMap(int width, int height, std::vector<std::vector<int>> &mapVec);
	int steps;
};






#endif