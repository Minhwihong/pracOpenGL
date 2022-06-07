#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "common.h"
#include "shader.h"


namespace gl_test{

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
        float* Make_Circle(float r, int resol, size_t* point_cnt, size_t* array_size, uint32_t color);
        uint32_t* GetElementArr_Circle();
        bool Make_3Dcylinder(float r, float h, int resol);
        uint32_t* GetElementArr_3Dcylinder(float* src, size_t resolution);

        float* GetPtrVertex() const {return ptrVerex;}
        float* GetPtrColor() const {return ptrColor;}
        uint32_t GetVertexCnt() const {return vertex_cnt;}
        uint32_t GetVertexSize() const {return vertex_size;}
        uint32_t GetElemArrCnt() const {return element_arr_cnt;}

        void ChangeColor(float r, float g, float b);

    private:
        float* arrCircle;
        int circleResol;

        float* ptrVerex{nullptr};
        float* ptrColor{nullptr};
        float* ptrElemArr{nullptr};
        uint32_t vertex_cnt;
        uint32_t vertex_size;
        uint32_t element_arr_cnt;
        
    };

}




#endif