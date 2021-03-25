#include "stdafx.h"
#include "Ki.h"
#include "AStern.h"
#include <random>

Ki::Ki(Map &map) : map(&map){
	steps = 0;
	createPlantMap();
}
/*Führt einen Simulationsschritt aus. Dazu werden die Simulationsautomaten
mit den entsprechenden Kreaturenvektoren aufgerufen. 
*/
void Ki :: makeMove(){
	
	bool pflanze = false;
	bool placed = false;
	static int count = 0;
	simulatePlant(map->landplants);
	simulatePlant(map->waterplants);
	simulateAnimals(map->landanimals);
	simulateAnimals(map->wateranimals);
	count++;
	steps = count;
	map->setSteps(steps);
}

/*Überprüft die Umgebung der Pflanze und zieht dementsprechend
Lebenspunkte ab.
*/
void Ki::checkEnvironment(CreatureProps &c){
	if (c.environment == BIOSIM_SAND || c.environment == BIOSIM_SHALLOW_WATER || c.environment == BIOSIM_EARTH)
		c.setActualLifetime(10);
	if(c.environment == BIOSIM_DEEP_SEA || c.environment == BIOSIM_SNOW || c.environment == BIOSIM_ROCKS){
		c.setActualLifetime(25);
	}
}
//Untersucht den übergebenen Pflanzenvektor, um die Bedingungen für eine Fortpflanzung
//gegeben sind bei anderen Pflanzen gegeben sind.
bool Ki::checkSorroundingPlants(std::list<CreatureProps> &vec, CreatureProps &c){
	int plantcounter = 0;
	int index = -1;
	std::list<CreatureProps>::iterator it, end;
	for (it = vec.begin(),  end = vec.end(); it != end; ++it){
		
		int distance = abs((c.getX() + c.getY()) - ((*it).getX() +  (*it).getY()));
		bool name = c.getName() == (*it).getName();
		bool id = c.getId() != (*it).getId();
		bool time = (checkTime(c) && checkTime(*it));
		bool ready = ((*it).GetZustand() != BIOSIM_TOT);
		
		if (id && name && ready && time && (distance < 5)){
			plantcounter += 1;

		}
		if (plantcounter >= 10) return false;
	}
	if (plantcounter < 10 && plantcounter > 0 ){
		return true;
	}
	return false;
}
/*Überprüft, ob ein gewisser Zeitpunkt seit der letzten Fortpflanzung vergangen ist.
Gilt für Pflanzen.
*/
bool Ki::checkTime(CreatureProps &c){
	if ((c.getLifetime() / 100) < (steps - c.getLastReproduction()))
		return true;
	return false;
}
/*Überprüft, ob ein gewisser Zeitraum seit der letzten Paarung
vergangen ist.
*/
bool Ki::checkAnimalTime(CreatureProps &c){
	if ((c.getLifetime() / 50) < (steps - c.getLastReproduction()))
		return true;
	return false;
}
//Gibt eine Zufallszahl innerhalb eines gegebenen Rahmens zurück
int Ki::getRandomNumber(int range){
	int random = rand() % (2*range+1) +(-range);
	return random;
}

/*Überprüft zu Beginn, um welchen Kreaturentyp es sich handelt, anschließend
wird für den entsprechenden Typen falls vorhanden die Beute gesucht und als
Beute mittels Pointer gesetzt.
*/
bool Ki::checkPray(std::list<CreatureProps> &vec, CreatureProps &c){
	bool carnivore = false;
	bool land = false;
	std::string properties = c.getProperties();
	//Prüft, ob es sich um einen Fleischfresser handelt.
	if (properties.find("Fleisch") != std::string::npos){
		carnivore = true;
	}
	//Prüft, ob es sich um einen Landbewohner handelt.
	if (properties.find("Land") != std::string::npos){
		land = true;
	}
	//Sucht nach Beute für einen fleischfressenden Landbewohner. Falls eine gefunden wird, wird der Pointer gesetzt.
	if (carnivore && land && !map->landanimals.empty()){
		for (std::list<CreatureProps>::iterator it = map->landanimals.begin(), end = map->landanimals.end(); it != end; ++it){
			bool name = c.getName() != (*it).getName();
			bool id = c.getId() != (*it).getId();
			bool landpray = false;
			int distance = abs(c.getX() + c.getY() - ((*it).getX() + (*it).getY()));
			bool ndead = (*it).GetZustand() != BIOSIM_TOT;
			if (((*it).getProperties().find("Pflanzenfresser") != std::string::npos))//Stellt sicher, dass kein anderer Fleischfresser gejagt wird.
				landpray = true;
			if (name && id && ndead && landpray && abs(distance) <= 10){
				c.pray = &(*it);
				return true;
				}
			}
		}
	//Sucht nach Beute für einen fleischfressenden Wasserbewohner. Falls eine gefunden wird, wird der Pointer gesetzt.
	if (carnivore && !land && !map->wateranimals.empty()){
		for (std::list<CreatureProps>::iterator it = map->wateranimals.begin(), end = map->wateranimals.end(); it != end;++it){
			bool ndead = (*it).GetZustand() != BIOSIM_TOT;
			bool name = c.getName() != (*it).getName();
			bool id = c.getId() != (*it).getId();
			bool waterpray = false;
			int distance = abs(c.getX() + c.getY() - (*it).getX() - (*it).getY());
			if (((*it).getProperties().find("Pflanzenfresser") != std::string::npos))//Kein Fleischfresser soll gejagt werden.
				waterpray = true;
			if (ndead && name && id && waterpray && abs(distance) <= 10){
				c.pray = &(*it);
				return true;
			}
		}
	}
	//Sucht nach Beute für einen pflanzenfressenden Landbewohner. Falls eine gefunden wird, wird der Pointer gesetzt.
	if (!carnivore && land && !map->landplants.empty()){
		for (std::list<CreatureProps>::iterator it = map->landplants.begin(), end = map->landplants.end(); it != end; ++it){
			bool ndead = (*it).GetZustand() != BIOSIM_TOT;
			bool name = c.getName() != (*it).getName();
			bool id = c.getId() != (*it).getId();
			bool landpray = false;
			int distance = abs(c.getX() + c.getY() - (*it).getX() - (*it).getY());
			if (ndead && name && id  && abs(distance) <= 10){
				c.pray = &(*it);
				return true;
			}
		}
	}
	//Sucht nach Beute für einen pflanzenfressenden Wasserbewohner. Falls eine gefunden wird, wird der Pointer gesetzt.
	if (!carnivore && !land && !map->waterplants.empty()){
		for (std::list<CreatureProps>::iterator it = map->waterplants.begin(), end = map->waterplants.end(); it != end; ++it){
			bool ndead = (*it).GetZustand() != BIOSIM_TOT;
			bool name = c.getName() != (*it).getName();
			bool id = c.getId() != (*it).getId();
			int distance = abs(c.getX() + c.getY() - (*it).getX() - (*it).getY());
			if (ndead && name && id && abs(distance) <= 10){
				c.pray = &(*it);
				return true;
			}
		}
	}
	return false;
}
/*Der Funktion wird eine Kreatur und der Vektor aller bestehenden Kreaturen übergeben.
Durch den Vektor wird iteriert und mögliche Paarungspartner(gemessen an ID, Zeit, 
Name, Distanz und Zustand) für die übergebene Kreatur
herausgesucht. 
*/
bool Ki::checkMateAnimal(std::list<CreatureProps> &vec, CreatureProps &c){
	int count = 0;
	for (std::list<CreatureProps>::iterator it = vec.begin(), end = vec.end(); it != end; ++it){
		int distance = abs(c.getX() + c.getY() - ((*it).getX() + (*it).getY()));
		bool name = (*it).getName() == c.getName();
		bool id = (*it).getId() != c.getId();
		bool ready = (*it).GetZustand() != (BIOSIM_TOT  || BIOSIM_PAARUNG);
		bool time =  (checkAnimalTime(*it));
		if (name && id && time && ready && (abs(distance) <= 8)){
			count++;
		}
		if (count == 10)
			return false;
	}
	if (count >= 1 && count < 10){
		c.setReproduction(steps);
		return true;
	}
	else{
		return false;
	}
}
/*Erstellt eine Integerkarte mit Nullen
Auf dieser werden anschließend die verschiedenen Pflanzen
festgehalten.
*/
void Ki::createPlantMap(){
	plantMap.resize(map->getMapHeight());
	for ( int i = 0; i < map->getMapHeight(); i++){
		plantMap[i].resize(map->getMapWidth());
		for ( int j = 0; j < map->getMapWidth(); j++){
			plantMap[i][j] = 0;
		}
	}
}

int Ki::getSteps(){
	return steps;
}
/*Simuliert den Einzelschritt für Pflanzen. Geht dazu durch den übergebenen
Vektor iterativ und prüft, ob die Kriterien für einen Zustandswechsel erfüllt sind.
*/
void Ki::simulatePlant(std::list<CreatureProps>& cVec){
	//Iteration durch alle Kreaturen.
	for (std::list<CreatureProps>::iterator it = cVec.begin(), end = cVec.end(); it != end; ++it){
		CreatureProps &c = (*it);
		Zustand zus = c.GetZustand();
		//Setzt einen Vermerk auf der Pflanzenkarte
		if (plantMap[c.getX()][c.getY()] == 0)
			plantMap[c.getX()][c.getY()] = c.getTexture();
		switch (zus){
			case BIOSIM_WACHSEN:{
					//Falls eine Runde seit Wachstum vergangen wird in den Zustand Warten gewechselt.
					if ((steps - (c.getLastReproduction() + 1)) > 0)
						c.setZustand(BIOSIM_WARTEN);
					//Falls Lebenspunkte < 0 wird in Zustand Tot gewechselt.
					if (c.getActualLifetime() <= 0 && c.GetZustand() != BIOSIM_TOT){
						c.setZustand(BIOSIM_TOT);
						c.setDeath(steps);
					}
					//Sonst wird 5 mal ausprobiert ob es in der Umgebung möglich ist eine Pflanze zu setzen.
					//Falls ja, so wird diese gesetzt.
					if (c.GetZustand() == BIOSIM_WACHSEN ){
						bool plant1 = false;
						bool noPlacefound = false;
						Vector2D plantOne;
						int j = 0;//Counter
						while(!plant1 && !noPlacefound){
							int random = getRandomNumber(5);
							int random2 = getRandomNumber(5 - abs(random));
							if (c.getX() + random < 0)random = 0;
							if (c.getY() + random2 < 0)random2 = 0;
							if (c.getX() + random >= map->getMapWidth())random = 0;
							if (c.getY() + random2 >= map->getMapHeight())random2 = 0;
							int field = map->getField(c.getX() + random, c.getY() + random2);
							if (c.getMovement(field) > 0){
								//Überprüft, dass noch keine Pflanze auf Feld vorhanden.
								if (plantMap[c.getX() + random][c.getY() + random2] == 0){
									plant1 = true;
									plantOne.x = c.getX() + random;
									plantOne.y = c.getY() + random2;
									plantMap[c.getX() + random][c.getY() + random2] = c.getTexture();
								}		 
							}
							//Falls 5 Versuche des Platzierens fehlgeschlagen, wird abgebrochen. 
							if (j == 5)noPlacefound = true;
							j++;
						}
						//Falls Platz gefunden, wird eine neue Pflanze gesetzt.
						if (plant1){
							CreatureProps	creat(map->vCreat[c.getSelectionnumber()]);
							creat.setX(plantOne.x);
							creat.setY(plantOne.y);
							creat.setId(c.getId() + 1000 + steps);
							creat.setReproduction(steps);
							creat.environment = map->getField(c.getY(), c.getX());
							creat.setZustand(BIOSIM_WARTEN);
							cVec.push_back(creat);
						}
								 
					}
				break;
			}
			case BIOSIM_WARTEN:{
				//Zieht Lebenspunte nach Umgebung ab.
				checkEnvironment(c);
				//Falls entsprechend Pflanzen vorhanden, wird in den Zustand Wachsen gewechselt.
				if (checkSorroundingPlants(cVec, c)){
					c.setZustand(BIOSIM_WACHSEN);
					c.setReproduction(steps+1);
					plantMap[c.getX()][c.getY()] *= c.getTexture();
				}
				//Falls keine Lebenspunkte mehr, wird in Zustand Tot gewechselt. 
				if (c.getActualLifetime() <= 0 && c.GetZustand() != BIOSIM_TOT){
					c.setZustand(BIOSIM_TOT);
					c.setDeath( steps);
				}
				break;
			}
			case BIOSIM_TOT:{
				//Setzt die Tot-Textur für die Pflanze.
				if (c.getTexture() != map->getDeadTexture()){
					c.setTexture(map->getDeadTexture());
					plantMap[c.getX()][c.getY()] = -1;
				}

			break;
			}
		 }
		}
	//Iteriert über die Pflanzen und prüft, ob Kriterien zum löschen vorhanden sind.
	for (std::list<CreatureProps>::iterator it = cVec.begin(), end = cVec.end(); it != end; ){
		bool water = true;
		if ((*it).getProperties().find("Land") != std::string::npos) water = false;
		if ((steps - (*it).getDeath()) >= 10 && ((*it).GetZustand() == BIOSIM_TOT)){
			plantMap[(*it).getX()][(*it).getY()] = 0;
			if (!water)it = map->deleteLandplant(*it);
			if (water)it = map->deleteWaterplant(*it);
		}
		else{
			++it;
		}
		
	}
}

/*Simuliert den Einzelscchritt für Kreaturen. Geht dazu durch den übergebenen
Vektor iterativ und prüft, ob die Kriterien für einen Zustandswechsel erfüllt sind.
Falls ja wird die entsprechende Aktion ausgeführt.
*/
void Ki::simulateAnimals(std::list<CreatureProps> &cVec){
	bool water = false;
	bool placed = false;
	Vector2D aim;
	if (!cVec.empty() && cVec.front().getProperties().find("Wasser") != std::string::npos){
		water = true;
	}
	//Iteration durch alle Kreaturen. 
	for (std::list<CreatureProps>::iterator it = cVec.begin(), end = cVec.end(); it != end; ++it){
		CreatureProps &c = *it;
		std::string properties = c.getProperties();
		bool randomGoal = false;
		bool reachedGoal = false;
		if (c.getActualLifetime() <= 0 && c.GetZustand() != BIOSIM_TOT){
			c.setDeath(steps);
			c.setZustand(BIOSIM_TOT);
		}
		Zustand zus = c.GetZustand();
		switch (zus){
			case BIOSIM_WANDERN:{
				c.setActualLifetime(5);
				//Falls Ziel erreicht wird Zustand geändert. 
				if ((c.getX() == c.getGoal().x) && (c.getY() == c.getGoal().y) && c.getWandering()){
					c.setWandering(false);
					c.setSleep (((c.getActualLifetime() / c.getLifetime())) * 5);
					c.setZustand(BIOSIM_AUSRUHEN);
					break;
				}
				//Falls Kriterien für Paarung vorhanden wird in diesen Zustand gewechselt.
				if (c.GetZustand() == BIOSIM_WANDERN && checkAnimalTime(c) && !c.getWandering() && checkMateAnimal(cVec, c)){
					c.setZustand(BIOSIM_PAARUNG);
					break;
				}
				//Falls Beute in der Nähe und die Kreatur nicht umherwandert wird gejagt.
				if (c.GetZustand() == BIOSIM_WANDERN && !c.getWandering() && checkPray(map->landanimals, c)){
					c.setZustand(BIOSIM_JAGEN);
					break;
				}
				//Falls Kreatur umherwandert.
				if (c.getWandering()){
					PathFinder path(*map);//Pfad wird gesucht
					bool found = path.findPath(c, c.getGoal());
					int move = c.getSpeed() / 20;
					//Kacheln gemäß Geschwindigkeit zurückgelegt
					if ((path.pathToGoal.size()-1 <= (unsigned)move) && found)
						aim = *path.pathToGoal.front();
					if ((path.pathToGoal.size()-1 > (unsigned)move) && found)
						aim = *path.pathToGoal[path.pathToGoal.size()-1 - move];
					if (found){
						c.setX(aim.x);
						c.setY(aim.y);
					}
					//Falls kein Weg zur Zielkachel gefunden werden konnte.
					else{
						c.setWandering(false);
						randomGoal = true;
						}
					break;
					}
					//Sonst wird zufälliges Ziel gesetzt und zu diesem gewandert.
					else{
						//Zufälliges Ziel wird gesucht.
						if(randomGoal == false && !c.getWandering() && c.GetZustand() == BIOSIM_WANDERN){
							int random = getRandomNumber(10);
							int random2 = getRandomNumber(10 - abs(random));
							if (c.getX() + random < 0)random = 0;
							if (c.getY() + random2 < 0)random2 = 0;
							if (c.getX() + random > map->getMapWidth() - 1)random = 0;
							if (c.getY() + random2 > map->getMapHeight() - 1)random2 = 0;
							int field = map->getField(c.getX() + random, c.getY() + random2);
							if (c.getMovement(field) > 0){
								randomGoal = true;
								c.setWandering( true);
								c.setGoal(c.getX() + random, c.getY() + random2);
							}
						}
				}break;
			}
			case BIOSIM_JAGEN:{
				//Pfad zu Beute wird berrechnet und falls möglich gesetzt.
				c.setActualLifetime(10);
				if ((c.pray->getX() >= 0) && (c.pray->getY() >= 0)){
					PathFinder path(*map);
					aim.x = c.pray->getX();
					aim.y = c.pray->getY();
					bool found = path.findPath(c, aim);
					int move = c.getSpeed() / 10;
					//Bewegung in Richtung Beute.
					if ((path.pathToGoal.size()-1 <= (unsigned)move) && found)
						aim = *path.pathToGoal.front();
					if ((path.pathToGoal.size()-1 > (unsigned)move) && found)
						aim = *path.pathToGoal[path.pathToGoal.size()-1 - move];
					if (found){
						c.setX(aim.x);
						c.setY(aim.y);
					}
					//Falls Beute in benachbarter Kachel wird in Zustand Angriff gewechselt.
					if (abs((c.getX() + c.getY()) - (c.pray->getX() + c.pray->getY())) <= 1){
						c.setZustand(BIOSIM_ANGRIFF);
						}
				}
			 break;
			}
			case BIOSIM_ANGRIFF:{
				//Falls Beute tot oder Lebenspunkte voll wird in Zustand Ausruhen gewechselt.
				if (c.pray->getActualLifetime() <= 0 || c.getActualLifetime() >= c.getLifetime()){
					c.setZustand(BIOSIM_AUSRUHEN);
					c.setSleep (((c.getActualLifetime() / c.getLifetime())) * 5);
					break;
				}
				//Falls Beute noch in Reichweite, wird eigene Lebenskraft erhöht, die der Beute vermindert.
				if ((abs(c.getX() - c.pray->getX()) <= 1) && (abs(c.getY() - c.pray->getY()) <= 1)){
					c.setActualLifetime(-c.getStrength() / 10);
					c.pray->setActualLifetime(c.getStrength());
					break;
				}
				//Sonst wird wieder gejagt. 
				else{
					c.setZustand(BIOSIM_JAGEN);
				}
				break;
			}
			case BIOSIM_AUSRUHEN:{
			//Es werden die berechnete ANzahl Runden gewartet. Anschließend wird in den 
			//Zustand Wandern gewechselt.
			c.setActualLifetime(5);// 5 Lebenspunkte werden abgezogen.
			c.setSleep(c.getSleep()-1);//Anzahl der Schlafrunden um 1 reduziert.
			if (c.getSleep() <= 0){
				c.setZustand(BIOSIM_WANDERN);
				c.setWandering(false);
			 }
			break;
			}
			case BIOSIM_PAARUNG:{
				//Versucht 5 Umgebungskacheln aus um Kreatur zu platzieren.
				//Falls eine Runde vergangen erfolgt Wechsel in Zustand Wandern.
				bool noPlacefound = false;
				bool creature = false;
				int j = 0; //Counter
				Vector2D placement;
				while(!creature && !noPlacefound){
					int random = getRandomNumber(5);
					int random2 = getRandomNumber(5 - abs(random));
					if (c.getX() + random < 0)random = 0;
					if (c.getY() + random2 < 0)random2 = 0;
					if (c.getX() + random > map->getMapWidth() - 1)random = 0;
					if (c.getY() + random2 > map->getMapHeight() - 1)random2 = 0;
					int field = map->getField(c.getX() + random, c.getY() + random2);
					if (c.getMovement(field) > 0){
						creature = true;
						placement.x = c.getX() + random;
						placement.y = c.getY() + random2;
					}
					if (j == 5)noPlacefound = true;
					j++;
				}
				//Falls Kachel zum Platzieren gefunden, wird die Kreatur gesetzt.
				if (creature){
					CreatureProps creat(map->vCreat[c.getSelectionnumber()]);
					creat.setX(placement.x);
					creat.setY(placement.y);
					creat.setId(c.getId() + 1000);
					creat.setReproduction(steps);
					creat.environment = map->getField(c.getY(), c.getX());
					creat.setZustand(BIOSIM_WANDERN);
					cVec.push_back(creat);
				}
				c.setReproduction(steps);
				c.setSleep(((c.getActualLifetime() / c.getLifetime())) * 5);
				c.setZustand(BIOSIM_AUSRUHEN);
				
			   break;
			}
			case BIOSIM_TOT:{
				//Falls Lebenspunkte < 0 wird für die Kreatur die Tot-Textur geladen.
				if (c.getTexture() != map->getDeadTexture())
					c.setTexture(map->getDeadTexture());
				break;
			}
		}
		
	}
	//Geht die Kreaturenliste durch und löscht tote Kreaturen falls 10 Runden vergangen. 
	for (std::list<CreatureProps>::iterator it = cVec.begin(), end = cVec.end(); it != end;){
		bool water = false;
		water = ((*it).getProperties().find("Wasser") != std::string::npos);
		if ((steps - (*it).getDeath()) >= 10 && ((*it).GetZustand() == BIOSIM_TOT)){
			if (water) it = map->deleteWateranimal(*it);
			if (!water)it = map->deleteLandanimal(*it);
		}
		else{
			it++;
		}
	}
}