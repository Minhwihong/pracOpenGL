#include "polygon.h"

#define RESOLUTION_CIRCLE   20
#define BUF_CNT_CIRCLE     (int)(360 / RESOLUTION_CIRCLE)

float* Polygon::Make_Circle(float r){

    float* arr = new float[BUF_CNT_CIRCLE * 6]; 

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    int idx = 0;

    for(int deg=0; deg< 360; deg = deg + RESOLUTION_CIRCLE, idx++){

        x = glm::cos(glm::radians((float)deg));
        y = glm::sin(glm::radians((float)deg));

        arr[idx*6 + 0] = x;
        arr[idx*6 + 1] = y;
        arr[idx*6 + 2] = z;

        arr[idx*6 + 3] = 0.0f;
        arr[idx*6 + 4] = 1.0f;
        arr[idx*6 + 5] = 1.0f;
        
    }


    return arr;
}


float* Polygon::Make_3Dcylinder(float r, float h){

    
    return nullptr;
}