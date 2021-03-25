#ifndef BIOSIM_VECTOR2D_H
#define BIOSIM_VECTOR2D_H
/*Die Klasse Vector2D besitzt Member für die x-und y-
Koordinaten eines Punktes.
*/
#include "Node.h"
class Vector2D{
public:
	int x;
	int y;
	~Vector2D(){};
	Vector2D(){};
	Vector2D(int r, int s){
		x = (r);
		y = (s);
	};
};

#endif