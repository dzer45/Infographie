#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_() {
	std::ifstream in;
	in.open (filename, std::ifstream::in);
	std::string line;
	while ( !in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		//if(!line.compare(0, 2, "v")) {
		//	Vec3f v;
	}
}

Model::~Model(){
}

int Model::nverts() {
	return (int)verts_.size();
}

int Model::nfaces() {
	return (int)faces_.size();
}

std::vector<int> Model::face(int idx){
	return faces_[idx];
}

Vect3f Model::vert(int i) {
	return verts_[i];
}			
