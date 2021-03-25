
#include "Map.h"
#include "AStern.h"
#include <vector>
#include <list>
BOOL initGL(std::vector <GLuint> &textures);

bool loadRenderingData();

void update();

GLvoid ResizeGLScene(GLsizei width, GLsizei height);

int DrawGLScene(Map &map, int widht, int height, int pos);

void drawAStar(Map &map,  std::vector<std::vector<int>> &Map2d, Vector2D v1, int x, int y);

bool drawCreature(std::vector<std::vector<int>>& Map2d, std::list <CreatureProps> cTemp, int k, int j);

void deleteTextures(Map &map);

void renderCreature(std::list<CreatureProps> &cVec, int leftside, int rightside, int upside, int downside, int height);

void checkForCreature(Map &map, std::vector<std::vector<int>> &Map2d, int creature, int setx, int sety);