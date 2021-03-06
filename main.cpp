#include <vector>
#include <iostream>
#include <limits>
#include <cmath>
#include <cstdlib>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

Matrix Viewport;
Matrix Projection;
Matrix ModelView;
Vec3f varying_intensity;
mat<2,3,float> Uv;
mat<4,4,float> M;

Model *model     = NULL;
const int width  = 800;
const int height = 800;

Vec3f light_dir(10,1,0);
Vec3f    cam(1,-1,4);
Vec3f    center(0,0,0);
Vec3f        up(0,1,0);

Matrix  viewport(int x, int y, int w, int h) {
    Viewport = Matrix::identity();
    Viewport[0][3] = x+w/2.f;
    Viewport[1][3] = y+h/2.f;
    Viewport[2][3] = 255.f/2.f;
    Viewport[0][0] = w/2.f;
    Viewport[1][1] = h/2.f;
    Viewport[2][2] = 255.f/2.f;
     return Viewport;
}

Matrix  projection(float coeff) {
    Projection = Matrix::identity();
    Projection[3][2] = coeff;
    return Projection;
}

Matrix  lookat(Vec3f cam , Vec3f center, Vec3f up) {
    Vec3f z = (cam -center).normalize();
    Vec3f x = cross(up,z).normalize();
    Vec3f y = cross(z,x).normalize();
    ModelView = Matrix::identity();
    for (int i=0; i<3; i++) {
        ModelView[0][i] = x[i];
        ModelView[1][i] = y[i];
        ModelView[2][i] = z[i];
        ModelView[i][3] = -center[i];
    }
    return ModelView;
}



void triangle(Vec4f p0, Vec4f p1, Vec4f p2,Vec2f A ,Vec2f B ,Vec2f C , TGAImage &image, TGAImage &zbuffer) {
    Vec2f bboxmin( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    bboxmin[0] = std::min(bboxmin[0], p0[0]/p0[3]);
    bboxmax[0] = std::max(bboxmax[0], p0[0]/p0[3]);
    bboxmin[1] = std::min(bboxmin[1], p0[1]/p0[3]);
    bboxmax[1] = std::max(bboxmax[1], p0[1]/p0[3]);

    bboxmin[0] = std::min(bboxmin[0], p1[0]/p1[3]);
    bboxmax[0] = std::max(bboxmax[0], p1[0]/p1[3]);
    bboxmin[1] = std::min(bboxmin[1], p1[1]/p1[3]);
    bboxmax[1] = std::max(bboxmax[1], p1[1]/p1[3]);
    
    bboxmin[0] = std::min(bboxmin[0], p2[0]/p2[3]);
    bboxmax[0] = std::max(bboxmax[0], p2[0]/p2[3]);
    bboxmin[1] = std::min(bboxmin[1], p2[1]/p2[3]);
    bboxmax[1] = std::max(bboxmax[1], p2[1]/p2[3]);
    
    Vec3f s[2]; 
    Vec3f c;
    Vec2i P;
    TGAColor color;
    for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) {
        for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) {
	  Vec2f P2 = proj<2>(P);
	  for (int i=2; i--; ) {
	    s[i][0] = C[i]-A[i];
	    s[i][1] = B[i]-A[i];
	    s[i][2] = A[i]-P2[i];
	  }
	  Vec3f u = cross(s[0], s[1]);
	  if (std::abs(u[2])>1e-2){ 
	    c = Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
	  }else{
	    c = Vec3f(-1,1,1); 
	  }
          float z = p0[2]*c.x + p1[2]*c.y + p2[2]*c.z;
          float w = p0[3]*c.x + p1[3]*c.y + p2[3]*c.z;
          int frag_depth = std::max(0, std::min(255, int(z/w+.5)));
          if (c.x<0 || c.y<0 || c.z<0 || zbuffer.get(P.x, P.y)[0]>frag_depth) continue;
	    	Vec2f uv = Uv*c;  
	    	Vec3f n = proj<3>(embed<4>(model->normal(uv))).normalize();
	    	Vec3f l = proj<3>(M*embed<4>(light_dir)).normalize();
		Vec3f r = (n*(n*l*2.f)-l).normalize();
		float spec = pow(std::max(r.z,0.0f),model->specular(uv));
		float diff = std::max(0.f, n*l);
	    	TGAColor color2 =model->diffuse(uv);
            	color = color2 ;
		for(int i=0;i<3;i++) {
			color[i] = std::min<float>(5 + color2[i]*(diff+ .6*spec), 255);    
		}        	
		bool discard = false;
            if (!discard) {
                zbuffer.set(P.x, P.y, TGAColor(frag_depth));
                image.set(P.x, P.y, color);
            }
        }
    }
}


int main(int argc, char** argv) {
    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("obj/african_head.obj");
    }

    lookat(cam , center , up);
    viewport(width/8, height/8, width*3/4, height*3/4);
    projection(-1.f/(cam -center).norm());
    light_dir.normalize();

    TGAImage image  (width, height, TGAImage::RGB);
    TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);
    M = Projection*ModelView;    
    Matrix transformation = Viewport*Projection*ModelView;

    for (int i=0; i<model->nfaces(); i++) {
        Vec4f coords[3];
        for (int j=0; j<3; j++) {
		Uv.set_col(j,model->uv(i,j));
		Vec4f position = embed<4>(model->vert(i, j));
        	position = transformation*position;
        	varying_intensity[j] = std::max(0.f, model->normal(i, j)*light_dir);
            	coords[j] = position;
        }
	Vec2f A = proj<2>(coords[0]/coords[0][3]);
    	Vec2f B = proj<2>(coords[1]/coords[1][3]);
    	Vec2f C = proj<2>(coords[2]/coords[2][3]);
        triangle(coords[0],coords[1],coords[2],A,B,C, image, zbuffer);

    }

    image.  flip_vertically(); // to place the origin in the bottom left corner of the image
    zbuffer.flip_vertically();
    image.  write_tga_file("output.tga");
    zbuffer.write_tga_file("zbuffer.tga");

    delete model;
    return 0;
}
