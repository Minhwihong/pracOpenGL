#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "common.h"
#include "shader.h"


enum POLY_KIND{
    POLY_CIRCLE = 0,
    POLY_CYLINDER,
    POLY_SQUARE,

};

class Polygon;

class Polygon {
public:

    Polygon(){}
    float* Make_Circle(float r);
    float* Make_3Dcylinder(float r, float h);   

private:
    
};


#endif