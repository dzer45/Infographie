#include "tgaimage.h"
#include "geometry.h"
#include <cmath>
#include "model.h"

const int width = 800;
const int height = 800;
const int depth = 255;
 
Model *model = NULL;


void line(TGAImage &image, int x0, int y0, int x1, int y1) {
	bool steep = false;
	if (std::abs(x0-x1)<std::abs(y0-y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0>x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	for (int x=x0; x<=x1; x++) { 
		float t = 0;
		int y = 0;
		if ( y1 != y0) {
			t = (x-x0)/(float)(x1-x0); 
			y = y0*(1.-t) + y1*t;  
		} else {
			y = y0;
		}
		if (steep) {
			image.set(y, x, TGAColor(255, 1));
		} else {
			image.set(x, y, TGAColor(255, 1));
		}
	}


}

void triangle(TGAImage &image, int x0, int y0, int x1, int y1, int x2, int y2){
	if (x2<x0) {
  		std::swap(y0,y2);
  		std::swap(x0,x2);
  	}
  	if (x2<x1) {
    		std::swap(y1,y2);
    		std::swap(x1,x2);
 	}
	
  	for( int x=x0; x<=x1; x++) {
		if ( x0 != x1 ) {
			float t = (x-x0)/(float)(x1-x0); 
			float t2 = (x-x0)/(float)(x2-x0);
			int y = y0*(1.-t)+y1*t;
			int y3 = y0*(1.-t2)+y2*t2;
			line(image, x, y, x, y3);
		}else	
			line(image, x0, y0, x1, y1);
			
	}
	for( int x=x1; x<=x2; x++ ){
		if ( x1 != x2) {
			float t = (x-x1)/(float)(x2-x1);
			float t2 = (x-x1)/(float)(x2-x0);
			int y = y1*(1.-t)+y2*t;
			int y3 = y0*(1.-t2)+y2*t2;
			line(image, x, y, x, y3);
		} else
			line(image, x1, y1, x2, y2);
	}

}

int main() {
	model = new Model("obj/african_head.obj");

	TGAImage image(width,height, TGAImage::RGB );
	for( int i=0; i < model->nfaces(); i++) {
		std::vector<int> face = model -> face(i);
		Vec3i screen_coords[3];
		Vec3f world_coords[3];
		for ( int j=0; j<3; j++) {
			Vec3f v = model->vert(face[j]);
			screen_coords[j] = Vec3i((v.x+1.)*width/2., (v.y+1.)*height/2., (v.z+1.)*depth/2);
		//	world_coords[j] = v ;
		}
//		line

	}
	



	image.flip_vertically();
	image.write_tga_file("dump.tga");
}
