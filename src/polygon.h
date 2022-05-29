#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "common.h"
#include "shader.h"


enum POLY_KIND{
    POLY_CIRCLE = 0,
    POLY_CYLINDER,
    POLY_SQUARE,

};

enum PLANE_2D{
    PLANE_Z = 0,
    PLANE_Y,
    PLANE_X,
};

class Polygon;

class Polygon {
public:

    Polygon(){}
    float* Make_Circle(float r, int resol, size_t* point_cnt, int plane, u_int32_t color);
    float* Make_3Dcylinder(float r, float h);   

private:
    
};


#endif