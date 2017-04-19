#ifndef __DATASTRUCTURE_H__
#define __DATASTRUCTURE_H__

#include "ObjLibrary/Vector2.h"

class Particle
{
public:
	Vector2 pos;		// position
	Vector2 vel;		// velocity
	Vector2 acc;		// acceleration

	float dens;			// density
	float pres;			// pressure

	Particle *next;		// link list
};

class Cell
{
public:
	Particle *head;		// the cell is head of the link list
};

#endif
