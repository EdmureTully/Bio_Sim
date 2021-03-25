#include "stdafx.h"
#include "CreatureProps.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

/* Lieste die Datei zeilenweise ein und lässt sie überprüfen.
Enspricht die Zeile den Anforderungen, wird die erzeugte
Kreatur in einem Vector gespeichert.
*/
std::vector <CreatureProps> readInData(const std::string &adress){
	std::vector<CreatureProps>cVector;
	std::ifstream file(adress);
	std::string line;
	file.exceptions(std::ifstream::badbit);
	try{
		if (file.is_open()){
			int lines = 0;
			int correct_lines = 0;
			int incorrect_lines = 0;
			while (!file.eof()){
				getline(file, line);
				lines++;
				bool checkLine = checkInput(line, lines, cVector);
				if (line.empty() ){
					std::cout << "Zeile " << lines << " ist leer." << std::endl;
					incorrect_lines++;
				}
				if (checkLine){
					std::cout << "Zeile " << lines << " wurde korrekt eingelesen." << std::endl;
					correct_lines++;
				}
				else if (!checkLine ) incorrect_lines++;
			}
			std::cout << "Anzahl der richtig gelesenen Zeilen: " << correct_lines << std::endl;
			std::cout << "Anzahl der fehlerhaften Zeilen: " << incorrect_lines << std::endl;
			file.close();
		}
		else std::cout << "Das öffnen der Datei ist nicht moeglich!";
	}
	catch (std::exception const& e){
		std::cout << "Ein Fehler ist aufgetreten: " << e.what() << std::endl;
	}return cVector;
}
/*Durchläuft einen übergebenen String und unterteilt in Substrings.
Entsprichter 000forderungen wird der Zustandszähler bis 5
0rhöht und dort wird eine Kreatur erzeugt.
*/
bool checkInput(std::string &s, int linecounter, std::vector<CreatureProps> &cVector){
	int pos = 0, state = 0;
	static int number = 0;
	std::string creatureName, properties, path;
	int strength, speed, lifetime;
	for (unsigned int i = 0; i < s.length(); i++){
		if ((state == 0 || state == 4) && ((s.at(i) == ','))){
			std::string substring(s.substr(pos, i - pos));
			if (checkLetter(substring, state) == true){
				if (state == 0)creatureName = substring;
				if (state == 4)properties = substring;
				++state;
				pos = i + 1;
				++i;
			}
			else {
				printError(linecounter, state);
				return false;
			}
		}
		if ((state == 1 || state == 2 || state == 3) && s.at(i) == ','){
			std::string substring = s.substr(pos, i - pos);
			if (checkNumber(substring) == true){
				if (state == 1)strength = atoi(substring.c_str());
				if (state == 2)speed = atoi(substring.c_str());
				if (state == 3)lifetime = atoi(substring.c_str());
				++state;
				pos = i + 1;
			}
			else{
				printError(linecounter, state);
				return false;
			}
		}
		if (state == 5 && i == s.length() - 1){
			std::string substring(s.substr(pos, i - pos - 3));
			std::string end(s.substr(i - 3, 4));
			if (checkLetter(substring, state) == true && checkEnding(end) == true){
				path = s.substr(pos, i);
				CreatureProps c(creatureName, strength, speed, lifetime, properties, path);
				c.setSelectionnumber(number);
				cVector.push_back(c);
				number++;
				c.getMovement(1);
				return true;
			}
			else {
				printError(linecounter, state);
				return false;
			}
		}
		else if (state != 5 && i == s.length() - 1){
			printError(linecounter, state);
			return false;
		}
	}return false;
}

/*Überprüft je nach übergebenem Status den übergebenen String.
Zustand 0: nur Buchstaben, Zustand 4:Buchstaben und Unterstriche,
Zustand 5: Buchstaben und Slash.
*/
bool checkLetter(const std::string &s, int state){
	bool check = false;
	switch (state){
	case 0:
		for (unsigned int i = 0; i < s.length(); i++){
			char act = s.at(i);
			if (i < s.length() - 1){
				char next = s.at(i + 1);
				if (next == ' ' && act == ' ')return false;
			}
			if ((act >= 'a' && act <= 'z') || (act >= 'A' && act <= 'Z') || act == ' ' || checkUmlaut(act) == true){
				check = true;
			}
			else return false;
		}break;
	case 4:
		for (unsigned int i = 0; i < s.length(); i++){
			char act = s.at(i);
			if (i < s.length() - 1){
				char next = s.at(i + 1);
				if (next == ' ' && act == ' ')return false;
			}
			if (((act >= 'a' && act <= 'z') || (act >= 'A' && act <= 'Z') || act == ' ' || act == '_' || checkUmlaut(act) == true)){
				check = true;
			}
			else return false;

		}break;
	case 5:
		for (unsigned int i = 0; i < s.length(); i++){
			char act = s.at(i);
			if (i < s.length() - 1){
				char next = s.at(i + 1);
				if (next == ' ' && act == ' ' || act == '/' && next == '/')return false;
			}
			if (((act >= 'a' && act <= 'z') || (act >= 'A' && act <= 'Z') || act == '/' || checkUmlaut(act) == true)){
				check = true;
			}
			else return false;
		}break;
	default:
		return false;
	}return check;
}

bool checkUmlaut(char c){
	if (c == 'ö' || c == 'Ö' || c == 'ä' || c == 'Ä' || c == 'ü' || c == 'Ü'){
		return true;
	}return false;
}

bool checkNumber(const std::string &s){
	char c;
	bool check = false;
	for (unsigned int i = 0; i < s.length(); i++){
		c = s.at(i);
		if (isdigit(c)){
			check = true;
		}
		else return false;
	}return check;
}
/*Prüft, ob der übergebene String die Endung
tga oder jpg besitzt.
*/
bool checkEnding(const std::string &s){
	std::string ending_one(".tga");
	std::string ending_two(".jpg");
	if (!(s == ending_one) && !(s == ending_two)){
		return false;
	}
	else return true;
}
// Gibt den jeweiligen Fehler in Zeile und Spalte aus.
void printError(int linecounter, int column){
	std::cout << "Fehler in Zeile " << linecounter << " in der Spalte:" << column << std::endl;
}


void CreatureProps::createMoveTable(){
	std::size_t found = properties.find("Wasser");
	if (found != std::string::npos){
		moveTable.push_back(3);
		moveTable.push_back(1);
		moveTable.push_back(-1);
		moveTable.push_back(-1);
		moveTable.push_back(-1);
		moveTable.push_back(-1);
		
	}
	else{
		moveTable.push_back(-1);
		moveTable.push_back(-1);
		moveTable.push_back(1);
		moveTable.push_back(1);
		moveTable.push_back(4);
		moveTable.push_back(2);
	}
}

void CreatureProps::setZustand(Zustand zustand1){
	zustand = zustand1;
}

Zustand CreatureProps::GetZustand(){
	return zustand;
}

GLuint CreatureProps::getTexture(){
	return texture;
}

void CreatureProps::setTexture(GLuint textures){
	texture = textures;
	
}

int CreatureProps::getMovement(int terrain){
	int temp = moveTable[terrain];
	return temp;
}

int CreatureProps::getSpeed(){
	return speed;
}

int CreatureProps::getStrength(){
	return strength;
}

std::string CreatureProps::getName(){
	return creatureName;
}

std::string CreatureProps::getProperties(){
	return properties;
}

void CreatureProps::setSelectionnumber(int num){
	 selectionnumber = num;
}

int CreatureProps::getSelectionnumber(){
	return selectionnumber;
}

void CreatureProps::setX(int x){
	xpos = x;
}

void CreatureProps::setY(int y){
	ypos = y;
}

int CreatureProps::getX(){
	return xpos;
}

int CreatureProps::getY(){
	return ypos;
}

int CreatureProps::getLastReproduction(){
	return lastReproduction;
}

void CreatureProps::setReproduction(int time){
	lastReproduction = time;
}

int CreatureProps::getId(){
	return id;
}

void CreatureProps::setId(int ID){
	id = ID;
}

void CreatureProps::setActualLifetime(int time){
	actualLifetime = actualLifetime - time;
}

int CreatureProps::getActualLifetime(){
	return actualLifetime;
}

int CreatureProps::getLifetime(){
	return lifetime;
}

int CreatureProps::getSleep(){
	return sleep;
}

bool CreatureProps::getWandering(){
	return wandering;
}

void CreatureProps::setSleep(int time){
	sleep = time;
}

void CreatureProps::setWandering(bool wander){
	wandering = wander;
}

void CreatureProps::setDeath(int time){
	death = time;
}

int CreatureProps::getDeath(){
	return death;
}

void CreatureProps::setGoal(int x, int y){
	goal = Vector2D(x, y);
}

Vector2D CreatureProps::getGoal(){
	return goal;
}

CreatureProps& CreatureProps::operator=( CreatureProps& element){
	if (this == &element)
		return *this;

	std::swap (element.pray, this->pray);
	std::swap(element.actualLifetime, this->actualLifetime);
	std::swap(element.xpos, this->xpos);
	std::swap(element.ypos, this->ypos);
	std::swap(element.death, this->death);
	std::swap(element.lastReproduction, this->lastReproduction);
	std::swap(element.zustand, this->zustand);
	std::swap(element.id, this->id);
	std::swap(element.texture, this->texture);
	std::swap(element.lifetime, this->lifetime);
	std::swap(element.speed, this->speed);
	std::swap(element.strength, this->strength);
	std::swap(element.selectionnumber, this->selectionnumber);
	std::swap(element.properties, this->properties);
	std::swap(element.creatureName, this->creatureName);
	

	return *this;
	
}