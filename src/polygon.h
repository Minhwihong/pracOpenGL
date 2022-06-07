#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "common.h"
#include "shader.h"

#define MAX_BUF_SIZE    1024*256

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


typedef struct cylinder_s{
    std::vector<float>* ptr_top;
    std::vector<float>* ptr_bottom;
    std::vector<float>* ptr_wall;
    std::vector<uint32_t>* ptr_element;

    int resolution;
    int h_step;
    uint32_t element_cnt;
    float radius;
    float height;
}CylinderS_t;

class Polygon;

class Polygon {
public:

    Polygon(){
        memset(buffer, 0, MAX_BUF_SIZE);
    }
        float* Make_Circle(float r, int resol, size_t* point_cnt, size_t* array_size, uint32_t color);
        uint32_t* GetElementArr_Circle();
        bool Make_3Dcylinder(float r, float h, int resol);
        bool Make_3Dcylinder_depth(float r, float h, int resol, int depth);
        uint32_t* GetElementArr_3Dcylinder(float* src, size_t resolution);

        float* GetPtrVertex() const {return ptrVerex;}
        float* GetPtrColor() const {return ptrColor;}
        uint32_t GetVertexCnt() const {return vertex_cnt;}
        uint32_t GetVertexSize() const {return vertex_size;}
        uint32_t GetElemArrCnt() const {return element_arr_cnt;}

        void ChangeColor(float r, float g, float b);

        const CylinderS_t& GetCylinderInst(uint8_t no)  {return m_cylinder;}

        const float* GetCylinderBuffer(){

            CylinderS_t& c = m_cylinder;
            size_t offset = 0;

            std::vector<float>* p_top = c.ptr_top;
            std::vector<float>* p_bottom = c.ptr_bottom;
            std::vector<float>* p_wall = c.ptr_wall;

            std::copy(p_top->begin(), p_top->end(), &buffer[offset]);
            offset += p_top->size();

            std::copy(p_bottom->begin(), p_bottom->end(), &buffer[offset]);
            offset += p_bottom->size();

            std::copy(p_wall->begin(), p_wall->end(), &buffer[offset]);
            offset += p_wall->size();

            return buffer;
        }

        bool GetElementArray();



private:
    float* arrCircle;
        int circleResol;

        float buffer[MAX_BUF_SIZE];

        float* ptrVerex{nullptr};
        float* ptrColor{nullptr};
        float* ptrElemArr{nullptr};
        uint32_t vertex_cnt;
        uint32_t vertex_size;
        uint32_t element_arr_cnt;

        CylinderS_t m_cylinder;
};


#endif