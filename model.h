//
//  model.h
//  
//
//  Created by Bachir on 19/02/15.
//
//

#ifndef ____model__
#define ____model__

#include <vector>
#include "geometry.h"

class Model {
    private:
        std::vector<Vec3f> verts_;
        std::vector<std::vector<int> > faces_;
    public:
        Model(const char *filename);
        ~Model();
        int nverts();
        int nfaces();
        Vec3f vert(int i);
        std::vector<int> face(int i);
};

#endif /* defined(____model__) */
