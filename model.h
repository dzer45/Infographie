#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "vect3f.h"


class Model
{
private :
	std::vector<Vect3f> verts_;
	std::vector<std::vector<int> > faces_;

public :
	//Constructeurs
	Model(const char *filename);
	~Model();
	int nverts();
	int nfaces();
	Vect3f vert(int i);
	std::vector<int> face(int idx);
};

#endif
