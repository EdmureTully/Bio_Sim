#include "stdafx.h"
#include "Map.h"
#include "PerlinNoise.h"
#include "LoadImage.h"
#include <algorithm>
#include <ctime>

//Wählt die Textur für übergebenen Perlinwert aus
Terrain selectTextureID(std::vector <Terrain> &vec, double perlin);
void pushBackEnum(std::vector <Terrain> &eVec);
//Konstruktor für die Karte
Map::Map(int width, int height) :height(height), width(width),xpos(0),ypos(0){
	Cursor* cursor(false);
	vCreat = (readInData("Liste.txt"));
	createMap(width, height, map);
	creatureID = 0;
	vCreature = readInData("Liste.txt");
}
Map::~Map(){}

//Erstellt die Karte mittels PerlinNoise und
//sie im übergebenen 2-dimensionalen Vektor.
void Map::createMap(int width, int height, std::vector<std::vector<int>> &mapVec){
	srand(time(NULL));
	unsigned int randomNumber = rand() % 1000;
	PerlinNoise pn(randomNumber);
	double n;
	std::vector <Terrain> terrainEnum;
	pushBackEnum(terrainEnum);
	mapVec.resize(width);
	
	for ( int i = 0; i < width; i++){
		std::vector <std::vector<int>> temp;
		mapVec[i].resize(height);
		for ( int j = 0; j < height; j++){
			
			Terrain id ;
			double x = (double)j / (double)width;
			double y = (double)i / (double)height;
			n = pn.noise(10 * x, 10 * y, 0.8);
			id = selectTextureID(terrainEnum, n);
			mapVec[i][j] = (id) ;
		}
		
	}
}
//Speichert die übergebenen Texturen im Member von Map
void Map::setTextures(std::vector <GLuint> &textures){
	for (unsigned int i = 0; i < 6 ; i++){
		texture.push_back(textures[0]);
		textures.erase(textures.cbegin());
	}
	
}

void Map::setCreatureTextures(std::vector <GLuint> &textures){
	for (unsigned int i = 0; i < vCreat.size(); i++){
		vCreat[i].setTexture(textures[0]);
		textures.erase(textures.cbegin());
	}
}

Cursor& Map::getCursor(){
	return cursor;
}

void Map::setXpos(int x){
	xpos = x;
}

void Map::SetCursorTextur(Cursor &cursor, std::vector <GLuint> &textures){
	cursor.setTexture(textures);
}

int Map::getNextID(){
	creatureID++;
	return creatureID;
}

void Map::setYpos(int y){
	ypos = y;
}

void Map::setSteps(int step){
	steps = step;
}

int Map::getSteps(){
	return steps;
}

int Map::getXpos(){
	return xpos;
}

int Map::getYpos(){
	return ypos;
}

int Map::getMapHeight(){
	return height;
}

int Map::countTextures(){
	return texture.size();
}

int Map::getMapWidth(){
	return width;
}

int Map::getPreselection(){
	return preselection;
}

void Map::setPreselection(int sel){
	preselection = sel;
}

std::vector<std::vector<int>> Map::getMap(){
	return map;
}


std::list<CreatureProps>::iterator Map::deleteLandplant(CreatureProps &c){
	for (std::list<CreatureProps>::iterator it = landplants.begin(), end = landplants.end(); it != end;){
		if (c.getId() == (*it).getId()){

			it = landplants.erase(it);
			return it;
		}
		else{
			++it;
		}
	}
	std::list<CreatureProps>::iterator it;
	return it;
}

std::list<CreatureProps>::iterator Map::deleteWaterplant(CreatureProps &c){
	for (std::list<CreatureProps>::iterator it = waterplants.begin(), end = waterplants.end(); it != end;){
		if (c.getId() == (*it).getId()){

			it = waterplants.erase(it);
			return it;
		}
		else{
			++it;
		}
	}
	std::list<CreatureProps>::iterator it;
	return it;
}

std::list<CreatureProps>::iterator Map::deleteLandanimal(CreatureProps &c){
	for (std::list<CreatureProps>::iterator it = landanimals.begin(), end = landanimals.end(); it != end;){
		if (c.getId() == (*it).getId()){

			it = landanimals.erase(it);
			return it;
		}
		else{
			++it;
		}
	}
	std::list<CreatureProps>::iterator it;
	return it;
}

std::list<CreatureProps>::iterator Map::deleteWateranimal(CreatureProps &c){
	for (std::list<CreatureProps>::iterator it = wateranimals.begin(), end = wateranimals.end(); it != end;){
		if (c.getId() == (*it).getId()){

			it = wateranimals.erase(it);
			return it;
		}
		else{
			++it;
		}
	}
	std::list<CreatureProps>::iterator it;
	return it;
}

std::vector<GLuint>Map::getTextures(){
	return texture;
}



void Map::setPathTexture(std::vector <GLuint> &textures){
	pathTexture = textures[0];
	textures.erase(textures.cbegin());
}

GLuint Map::getPathTexture(){
	return pathTexture;
}

void Map::setSelection(int selected){
	selection = selected;
}

int Map::getSelection(){
	return selection;
}

void Map::setDeadTexture(std::vector <GLuint> &textures){
	deadTexture = textures[0];
	
}

GLuint Map::getDeadTexture(){
	return deadTexture;
}

void pushBackEnum(std::vector <Terrain> &eVec){
	eVec.push_back(BIOSIM_DEEP_SEA);
	eVec.push_back(BIOSIM_SHALLOW_WATER);
	eVec.push_back(BIOSIM_SAND);
	eVec.push_back(BIOSIM_EARTH);
	eVec.push_back(BIOSIM_ROCKS);
	eVec.push_back(BIOSIM_SNOW);
}

Terrain Map::getField(int x, int y){
	return static_cast<Terrain>(map[y][x]);
}
//Ordnet Perlinwerte den Texturen zu
Terrain selectTextureID(std::vector <Terrain> &vec,double perlin){
	Terrain id;
	if (perlin < 0.33)
		id = vec[0];
	if (perlin < 0.44 && perlin >= 0.33)
		id = vec[1];
	if (perlin < 0.49 && perlin >= 0.44)
		id = vec[2];
	if (perlin < 0.57 && perlin >= 0.49)
		id = vec[3];
	if (perlin < 0.65 && perlin >= 0.57)
		id = vec[4];
	if (perlin >= 0.65)
		id = vec[5];
	return id;
}

Cursor::Cursor(){};

Cursor::Cursor(bool isSet):isSet(isSet){}

Cursor::~Cursor(){}

void Cursor::setTexture(std::vector <GLuint> &textures){
	cursorTextur = textures[0];
	textures.erase(textures.cbegin());
}

void Cursor::setActiv(bool activ){
	isSet = activ;
	
}

void Cursor::setXpos(int x){
	xPos = x;
}

void Cursor::setYpos(int y){
	yPos = y;
}

void Cursor::setOldXpos(int x){
	oldXpos = x;
}

void Cursor::setOldYpos(int y){
	oldYpos = y;
}

int Cursor::getXpos(){
	return xPos;
}

int Cursor::getYpos(){
	return yPos;
}

int Cursor::getOldXpos(){
	return oldXpos;
}

int Cursor::getOldYpos(){
	return oldYpos;
}

bool Cursor::checkActiv(){
	return isSet;
}

void Cursor::setAStar(bool star){
	astar = star;
}

bool Cursor::getAStar(){
	return astar;
}


GLuint Cursor::getTexture(){
	return cursorTextur;
}

void Cursor::setAStarActiv(bool activ){
	aStarActiv = activ;
}

bool Cursor::getAStarActiv(){
	return aStarActiv;
}

