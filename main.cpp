#include "tgaimage.h"
#include <cmath>

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
	line(image, x0, y0, x1, y1);
	line(image, x1, y1, x2, y2);
	if (x2<x0) {
  		std::swap(y0,y2);
  		std::swap(x0,x2);
  	}
  	if (x2<x1){
    		std::swap(y1,y2);
    		std::swap(x1,x2);
 	 }
/*  	for( int x=x0; x<=x1; x++) {
		float t = (x-x0)/(float)(x1-x0); 
		int y = y0*(1.-t)+y2*t;
		line(image, x, y0, x, y);
	}
	for( int x=x1; x<=x2; x++ ){
		float t = (x-x1)/(float)(x2-x1);
		int y = y1*(1.-t)+y2*t;
		line(image, x, y2, x, y);
	}
*/
}
int main() {
	TGAImage image(400, 400, 1);
//	line(image, 13, 20, 13, 60);
	triangle(image,16,20,80,55,100,20);
	image.flip_vertically();
	image.write_tga_file("dump.tga");
}
