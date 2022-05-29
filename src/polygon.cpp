#include "polygon.h"


#define RESOLUTION_CIRCLE   20
#define BUF_CNT_CIRCLE     (int)(360 / RESOLUTION_CIRCLE)

float* Polygon::Make_Circle(float r, int resol, size_t* point_cnt, int plane, uint32_t color){

    float* arr = new float[(int)(360 * (float)resol / 360) * 6]; 

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    int idx = 0;
    int inc = 360 / resol;

     

    for(int deg=0; deg< 360; deg = deg + inc, idx++){

        switch(plane){
            case PLANE_Z:     // z-plane
            x = glm::cos(glm::radians((float)deg));
            y = glm::sin(glm::radians((float)deg));
            //SPDLOG_INFO("pos x: {}, y: {}, z: {}", x,y,z);
            break;

            case PLANE_Y:     // y-plane
            x = glm::cos(glm::radians((float)deg));
            z = glm::sin(glm::radians((float)deg));
            break;

            case PLANE_X:     // x-plane
            y = glm::cos(glm::radians((float)deg));
            z = glm::sin(glm::radians((float)deg));
            break;
        }
        

        arr[idx*6 + 0] = x;
        arr[idx*6 + 1] = y;
        arr[idx*6 + 2] = z;

        arr[idx*6 + 3] = 0.0f;
        arr[idx*6 + 4] = 1.0f;
        arr[idx*6 + 5] = 1.0f;
        
    }

    SPDLOG_INFO("2D circle resolution {}-deg/360", resol);
    SPDLOG_INFO("2D circle Vertex number : {}", idx);
    SPDLOG_INFO("2D circle Array Size: {}", (int)(360 * resol / 360) * 6); 
    

    *point_cnt = idx;
    //*point_cnt = (*point_cnt)*6;

    return arr;
}


float* Polygon::Make_3Dcylinder(float r, float h){

    
    return nullptr;
}