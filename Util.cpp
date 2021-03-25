#include "stdafx.h"
#include "Util.h"
#include "LoadImage.h"
#include "Vector2D.h"
#include <math.h>


void drawQuad(GLfloat xpos, GLfloat ypos);

int DrawGLScene(Map &map, int width, int height, int creature);



bool checkNumberInVector(std::vector <int> &vec, int number){

	for (unsigned int i = 0; i < vec.size(); i++){
		if (number == vec[i])return true;
	}
	return false;
}
//Läd die Bilddateien der Landschaft
bool LoadTerrainTGAinVector(std::vector<TGA> *vecTGA){
	if (!vecTGA){
		return false;
	}
	
	TGA tga_dp("terrain\\deep_sea.tga");
	vecTGA->push_back(tga_dp);
	TGA tga_sw("terrain\\shallow_water.tga");
	vecTGA->push_back(tga_sw);
	TGA tga_s("terrain\\sand.tga");
	vecTGA->push_back(tga_s);
	TGA tga_e("terrain\\earth.tga");
	vecTGA->push_back(tga_e);
	TGA tga_r("terrain\\rocks.tga");
	vecTGA->push_back(tga_r);
	TGA tga_sn("terrain\\snow.tga");
	vecTGA->push_back(tga_sn);
	TGA tga_cursor("cursor\\cursor.tga");
	vecTGA->push_back(tga_cursor);
	TGA tga_path("cursor\\path.tga");
	vecTGA->push_back(tga_path);
	return true;

}
//Lädt die Terrainkacheln
bool LoadCreatureTGAInVector(std::vector <TGA> *vecTGA){
	if (!vecTGA){
		return false;
	}
	
	TGA tga_alge("wasser\\algen.tga");
	vecTGA->push_back(tga_alge);
	TGA tga_seetang("wasser\\seetang.tga");
	vecTGA->push_back(tga_seetang);
	TGA tga_plankton("wasser\\plankton.tga");
	vecTGA->push_back(tga_plankton);
	TGA tga_wels("wasser\\wels.tga");
	vecTGA->push_back(tga_wels);
	TGA tga_forelle("wasser\\forelle.tga");
	vecTGA->push_back(tga_forelle);
	TGA tga_krabbe("wasser\\krabbe.tga");
	vecTGA->push_back(tga_krabbe);
	TGA tga_hai("wasser\\hai.tga");
	vecTGA->push_back(tga_hai);
	TGA tga_delphin("wasser\\delphin.tga");
	vecTGA->push_back(tga_delphin);
	TGA tga_gras("land\\gras.tga");
	vecTGA->push_back(tga_gras);
	TGA tga_kaktus("land\\kaktus.tga");
	vecTGA->push_back(tga_kaktus);
	TGA tga_gebuesch("land\\busch.tga");
	vecTGA->push_back(tga_gebuesch);
	TGA tga_sonnenblume("land\\sonnenblume.tga");
	vecTGA->push_back(tga_sonnenblume);
	TGA tga_eiche("land\\eiche.tga");
	vecTGA->push_back(tga_eiche);
	TGA tga_obstbaum("land\\birne.tga");
	vecTGA->push_back(tga_obstbaum);
	TGA tga_tannenbaum("land\\tanne.tga");
	vecTGA->push_back(tga_tannenbaum);
	TGA tga_kuh("land\\kuh.tga");
	vecTGA->push_back(tga_kuh);
	TGA tga_pferd("land\\pferd.tga");
	vecTGA->push_back(tga_pferd);
	TGA tga_emu("land\\emu.tga");
	vecTGA->push_back(tga_emu);
	TGA tga_schaf("land\\schaf.tga");
	vecTGA->push_back(tga_schaf);
	TGA tga_hund("land\\hund.tga");
	vecTGA->push_back(tga_hund);
	TGA tga_baer("land\\ursus.tga");
	vecTGA->push_back(tga_baer);
	

	TGA tga_tiger("land\\tiger.tga");
	vecTGA->push_back(tga_tiger);
	TGA dead("dead\\dead.tga");
	vecTGA->push_back(dead);

	return true;
}
//Läd die TGA-Terraindateien, erstellt daraus Texturen
//und speichert sie im übergebenen Vektor.
BOOL generateTerrainTextures(std::vector <GLuint> &textures, std::vector<TGA> &vTGA){
	int size = vTGA.size();
	std::vector <GLuint> temp;
	temp.resize(size);

	for ( int i = 0; i < size; i++){
		std::vector <unsigned char> v = vTGA[0].getImageData();
		glGenTextures(1, &temp[i]);
		GLenum error = glGetError();
		glBindTexture(GL_TEXTURE_2D, temp[i]);
		error = glGetError();
		if (vTGA[0].getType() == 0){
			glTexImage2D(GL_TEXTURE_2D, 0, 3, vTGA[0].getWidth(), vTGA[0].getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, &v[0]);
			error = glGetError();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		if (vTGA[0].getType() == 1){
			
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, vTGA[0].getWidth(), vTGA[0].getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, &v[0]);
			error = glGetError();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		}
		if (error != 0){
			return FALSE;
		}
		vTGA.erase(vTGA.cbegin());
		textures.push_back(temp[i]);
	}

	return TRUE;
}

BOOL generateTexture(GLuint &texture, std::vector<TGA> &vTGA){
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	GLenum error = glGetError();
	std::vector <unsigned char> v = vTGA[0].getImageData();
	if (vTGA[0].getType() == 0){
		glTexImage2D(GL_TEXTURE_2D, 0, 3, vTGA[0].getWidth(), vTGA[0].getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, &v[0]);
		error = glGetError();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (vTGA[0].getType() == 1){

		glTexImage2D(GL_TEXTURE_2D, 0, 3, vTGA[0].getWidth(), vTGA[0].getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, &v[0]);
		error = glGetError();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (error != 0){
		return FALSE;
	}
	vTGA.erase(vTGA.cbegin());
	return TRUE;
}


//Initialisiert OpenGL und generiert die Texturen,
//da dafür ein OpenGL-Kontext notwendig ist.
BOOL initGL(std::vector <GLuint> &textures){
	std::vector <TGA> vTGA;
	if (!LoadTerrainTGAinVector(&vTGA)){
		return FALSE;
	}
	if (!generateTerrainTextures(textures, vTGA))
		return FALSE;
	if (!LoadCreatureTGAInVector(&vTGA)){
		return FALSE;
	}
	if (!generateTerrainTextures(textures, vTGA))
		return FALSE;
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f); 

	glEnable(GL_DEPTH_TEST);		
	glDepthFunc(GL_LEQUAL);
	// Enables Depth Testing                                          
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	//vielleicht ausschalten
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	return TRUE;
}
//Erstellt den Betrachtungsbereich des OpenGL-Fensters.
GLvoid ResizeGLScene(GLsizei width, GLsizei height){
	if (!height ){
		height = 1;
	}
	if (!width){
		width = 1;
	}
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLenum error;
	
	glOrtho(0,width , 0,height,-0.0f,128.0f);
	error= glGetError();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
//Zeichnet die Karte im OpenGL-Fenster. Erhält die
//zu zeichnende Karte und die Größe des Fensters.
int DrawGLScene( Map  &map, int width, int height, int creature ){
	int newX = map.getXpos();//X-Position der Karte
	int newY = map.getYpos();//Y-Position der Karte
	int maxRight = static_cast<int>(round((width) / 32));//Anzahl der Kachel in der Breite
	int maxBottom = static_cast<int>(round((height) / 32));//Anzahl der Kacheln in der Höhe
	float cursorx = static_cast<float>((map.getCursor().getXpos())); //X-Position des Cursors
	float cursory = static_cast<float>((map.getCursor().getYpos() ));//Y-Position des Cursors
	std::list <CreatureProps> cTemp;//Temporäre Liste zum überprüfen der platzierten Kreaturen
	std::vector<GLuint> textures = map.getTextures();//Terraintexturen
	float xscroll = (float)(map.getMapWidth() - maxRight) / map.getMapWidth();//Vertikaler Scroll
	float yscroll = (float)(map.getMapHeight() - maxBottom) / map.getMapHeight();//horizontaler Scroll
	int sety = static_cast<int>(cursorx / 32 + newY * 1 / 32 * yscroll);//X-Koordinate der Kachel
	int setx = static_cast<int>(cursory / 32 + newX * 1 / 32 * xscroll);//Y-Koordinate der Kachel
	static std::vector<std::vector<int>> Map2d  (map.getMap());//Integer-Terrainkarte
	int leftside = static_cast<int>( yscroll*newY / 32);//linker Rand des Fensters
	int rightside = static_cast<int>((maxBottom + 1) * 32 + newY);//rechter Rand des Fensters
	int upside = static_cast<int>(xscroll*newX / 32);//oberer Rand des Fensters
	int downside = static_cast<int>(maxRight * 32 + ((newX / 32) * 32));//unterer Rand des Fensters
	if (map.getSelection() != -1)checkForCreature(map, Map2d, creature, setx, sety);
	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 0, 0.f);
	glTranslatef(-xscroll*newX, yscroll*newY, 0.0f);//Bewegt den Rahmen dem Scroll entsprechend.
	bool activ = map.getCursor().checkActiv();
	//Zeichnet die Texturen, welche sich im Sichtbereich befinden.
	for (int t = 0; t < map.countTextures(); t++){
		glBindTexture(GL_TEXTURE_2D, textures[t]);
		for (int i = map.getMapWidth()-1; i > -1; i--){
			for (int j = map.getMapWidth()-1; j > -1; j--){
				if (((leftside <= i +1 ) && ((i) * 32 < rightside + 1) &&
					(( upside <= j +1 ) && ((j) * 32 < downside + 1)))){
					//Zeichnet die Umgebungskachel
					if (Map2d[i][j] == t){
						drawQuad(GLfloat(height-((i)*32)), GLfloat((j)*32));
					}
				}
			}
		}
	}
	//Überprüft für jeden Kreaturentyp, ob der dazugehörige Vektor nicht leer ist.
	//Falls nicht, werden die Kreaturen, welche sich im Sichfeld befinden gezeichnet. 
	if (!map.landplants.empty())renderCreature(map.landplants, leftside, rightside, upside, downside, height);
	if (!map.landanimals.empty())renderCreature(map.landanimals, leftside, rightside, upside, downside, height);
	if (!map.wateranimals.empty())renderCreature(map.wateranimals, leftside, rightside, upside, downside, height);
	if (!map.waterplants.empty())renderCreature(map.waterplants, leftside, rightside, upside, downside, height);
	//Prüft, ob auf das Fenster mit dem Cursor gekliickt wurde. Falls ja wird er gezeichnet. 
	if (activ){
		int k = static_cast<int>(sety);
		int j = static_cast<int>(setx);
		glBindTexture(GL_TEXTURE_2D, map.getCursor().getTexture());
		if (j >= map.getMapHeight())//Falls Cursor Postion > Größe der Karte
			j = map.getMapHeight() -1;
		if (k >= map.getMapWidth())
			k = map.getMapWidth() - 1;
		drawQuad(static_cast<GLfloat>(height - k * 32), static_cast<GLfloat> (j * 32));
		activ = false;
	}
	//Prüft, ob AStern aktiviert wurde. Falls ja, wird er berechnet und falls möglich gezeichnet.
	if (map.getCursor().getAStar() ){
		float oldcursory = static_cast<float>((map.getCursor().getOldXpos()));
		float oldcursorx = static_cast<float>((map.getCursor().getOldYpos()));
		Vector2D v3(static_cast<int>(oldcursorx / 32 + newX * 1 / 32 * xscroll), static_cast<int>(oldcursory / 32 + newY * 1 / 32 * yscroll));
		drawAStar(map, Map2d, v3, setx, sety);
		glBindTexture(GL_TEXTURE_2D, map.getPathTexture());
		for (unsigned int i = 0; i < map.tempPos.size(); i++){
			drawQuad(GLfloat(height - (map.tempPos[i].y * 32)), GLfloat(map.tempPos[i].x * 32));
		}
	}
	return TRUE;
}
// Zeichnet eine Kachel an die momentan ausgewählte Position.
void drawQuad(GLfloat ypos, GLfloat xpos){
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xpos, ypos, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(32+xpos, 0.0f+ypos, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(32+xpos, 32+ypos, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f+xpos, ypos+32, 0.0f);
	glEnd();
}
//Lässt AStern berechnen und zeichnet ihn gegebenenfalls.
void drawAStar(Map &map,  std::vector<std::vector<int>>& Map2d, Vector2D v1, int x, int y){
	static std::list <Vector2D> aStarPath;
	bool check = false;
	
	if (!aStarPath.empty()){
		while (aStarPath.size() != NULL){
			aStarPath.pop_front();
		}
	}
	if (!map.tempPos.empty()){
		while (map.tempPos.size() != NULL)map.tempPos.pop_back();
	}
	
	CreatureProps	c = map.vCreat[map.getPreselection()];
	c.setX(x);
	c.setY(y);
	PathFinder path(map);

	if (v1.x >= 0 && v1.y >= 0)
		 check = path.findPath(c, v1);

	if (path.pathToGoal.size() != NULL && check){
		while (path.pathToGoal.size() != 0 && map.getCursor().getAStarActiv()){
			Vector2D vTemp = *path.pathToGoal.back();
			path.pathToGoal.pop_back();
			int xx = vTemp.x;
			int yy = vTemp.y;
			aStarPath.push_back(Vector2D(xx, yy));
			map.tempPos.push_back(vTemp);
		}

	}
}
/*Überprüft, ob die im übergebenen Vektor cTemp  enthaltene Kreatur auf den übergebenen Koordinaten
gesetzt werden darf. Falls nein, so wird eine Fehlermeldung ausgegeben.
*/
bool drawCreature(std::vector<std::vector<int>>& Map2d, std::list <CreatureProps> cTemp, int k, int j){
	int size = cTemp.size();
	std::string properties = cTemp.front().getProperties();
	bool check = false;
	if (properties.find("Wasser") != std::string::npos){
		check = true;
	}
	if (check && (Map2d[cTemp.front().getY()][cTemp.front().getX()] == BIOSIM_DEEP_SEA || Map2d[cTemp.front().getY()][cTemp.front().getX()] == BIOSIM_SHALLOW_WATER)){
		cTemp.pop_front();
		return true;
	}
	if (!check && (Map2d[cTemp.front().getY()][cTemp.front().getX()] == BIOSIM_ROCKS || Map2d[cTemp.front().getY()][cTemp.front().getX()] == BIOSIM_SAND || Map2d[cTemp.front().getY()][cTemp.front().getX()] == BIOSIM_EARTH || Map2d[cTemp.front().getY()][cTemp.front().getX()] == BIOSIM_SNOW)){
		cTemp.pop_front();
		return true;
	}
	if (size == cTemp.size()){
			MessageBox(NULL, L"Kreaturen bitte nur in entsprechenden Habitaten erzeugen.", L"Hinweis", MB_OK);
			cTemp.pop_front();
			return false;
		}
	return false;
}
/*Überprüft mittels den übergebenen Integerrwerten, ob die Kreaturen im Vektor im sichtbaren Bereich liegen.
Falls ja, werden sie gezeichnet.
*/
void renderCreature(std::list<CreatureProps> &cVec, int leftside, int rightside, int upside, int downside, int height){
	if (cVec.size()> 0){
		std::vector <int> renderedTextures;
		for (std::list<CreatureProps>::iterator it = cVec.begin(), end = cVec.end(); it != end; ++it){

			bool rendered = true;
			int creatTexture = (*it).getTexture();
			//überprüft, ob Textur schon mal gebunden wurde.
			if (!checkNumberInVector(renderedTextures, creatTexture)){

				rendered = false;
			}
			if (!rendered) {
				glBindTexture(GL_TEXTURE_2D, (*it).getTexture());
				for (std::list<CreatureProps>::iterator it1 = cVec.begin(), end1 = cVec.end(); it1 != end1; ++it1){
					bool check = true;
					if (checkNumberInVector(renderedTextures, (*it1).getTexture()))check = true;
					if ((*it1).getTexture() == creatTexture)check = false;
					if (!check && ((leftside <= (*it1).getY() +1) && (((*it1).getY()) * 32 < rightside+1) &&
						((upside <= (*it1).getX()+1) && (((*it1).getX()) * 32 < downside + 1))))
						drawQuad(GLfloat(height - ((*it1).getY() * 32)), GLfloat((*it1).getX() * 32));
				}
			}
			renderedTextures.push_back(creatTexture);
		}
	}
}
/*Überprüft, ob Kreaturen mittels "Platzieren"-Knopf gesetzt wurden. Falls ja, so wird
überprüft, ob sie auf dem richtigen Terrain gesetzt wurden. Falls nicht werden sie entfernt.
*/
void checkForCreature(Map &map, std::vector<std::vector<int>> &Map2d,int creature, int setx, int sety){
	static int id = 0;
	std::list <CreatureProps> cTemp;
	if (map.getSelection() != -1){
		bool check = false;
		CreatureProps	c = map.vCreat[creature];
		id++;
		c.setX(setx);
		c.setY(sety);
		c.setReproduction(map.getSteps());
		if (setx >= map.getMapHeight()) c.setX(map.getMapHeight() - 1);
		if (sety >= map.getMapWidth())c.setY(map.getMapWidth() - 1);
		c.setId(id);
		c.environment = map.getField(c.getX(), c.getY());
		std::string properties = c.getProperties();
		bool water = false;
		if (properties.find("Wasser") != std::string::npos){
			water = true;
		}
		cTemp.push_front(c);
		check = drawCreature(Map2d, cTemp, c.getY(), c.getX());
		if (c.getSpeed() > 0 && check){
			c.setZustand(BIOSIM_WANDERN);
			if (water)map.wateranimals.push_back(c);
			if (!water)map.landanimals.push_back(c);
		}
		if (c.getSpeed() <= 0 && check){
			c.setZustand(BIOSIM_WARTEN);
			if (water)map.waterplants.push_back(c);
			if (!water)map.landplants.push_back(c);
		}
	}
}