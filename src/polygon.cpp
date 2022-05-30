#include "polygon.h"


#define RESOLUTION_CIRCLE   20
#define BUF_CNT_CIRCLE     (int)(360 / RESOLUTION_CIRCLE)

float* Polygon::Make_Circle(float r, int resol, size_t* point_cnt, size_t* array_size, int plane, uint32_t color){

    float* arr = new float[(int)(360 * (float)resol / 360) * 6 + 6]; 

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    int idx = 0;
    int inc = 360 / resol;
    int offset = 0;

    arr[offset++] = 0;
    arr[offset++] = 0;
    arr[offset++] = 0;

    arr[offset++] = 0.0f;
    arr[offset++] = 1.0f;
    arr[offset++] = 1.0f;

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
        

        arr[offset + idx*6 + 0] = x;
        arr[offset + idx*6 + 1] = y;
        arr[offset + idx*6 + 2] = z;

        arr[offset + idx*6 + 3] = 0.0f + ((float)idx/resol);
        arr[offset + idx*6 + 4] = 0.0f;
        arr[offset + idx*6 + 5] = 1.0f;
        
    }

    SPDLOG_INFO("2D circle resolution {}-deg/360", resol);
    SPDLOG_INFO("2D circle Vertex number : {}", idx+1);
    SPDLOG_INFO("2D circle Array Size: {}", (int)(360 * resol / 360) * 6); 
    

    *point_cnt = idx+1;

    arrCircle = arr;
    circleResol = resol;

    return arr;
}

uint32_t* Polygon::GetElementArr_Circle(){

    if(arrCircle == nullptr){
        return nullptr;
    }

    uint32_t* arr = new uint32_t[circleResol*3];

    for(int idx=0; idx<circleResol; ++idx){

        arr[idx*3 + 0] = 0;
        arr[idx*3 + 1] = idx+1;


        if(idx+1 < circleResol){
            arr[idx*3 + 2]  = idx+2;

            //SPDLOG_INFO("circle element arr v1: {}, v2: {}, v3: {}", 0,idx+1,idx+2);
        }
        else{
            arr[idx*3 + 2]  = 1;

            //SPDLOG_INFO("circle element arr v1: {}, v2: {}, v3: {}", 0,idx+1,1);
        }

        
    }

    return arr;

}


float* Polygon::Make_3Dcylinder(float r, float h, int resol, size_t* point_cnt, size_t* array_size, int plane){

    size_t circle_ver_cnt = (size_t)(360 * (float)resol / 360 * 6);
    
    
    float* arr = new float[circle_ver_cnt*2 + 12]; 

    float x1 = 0.0f, x2 = 0.0f;
    float y1 = 0.0f, y2 = 0.0f;
    float z1 = 0.0f, z2 = 0.0f;
    h = 0.5;

    int idx = 0;
    int inc = 360 / resol;
    int offset = 0;

    *array_size = 0;
    *point_cnt = 0;

    //원통의 윗부분
    arr[offset++] = 0;
    arr[offset++] = 0;
    arr[offset++] = 0;

    arr[offset++] = 0.0f;
    arr[offset++] = 1.0f;
    arr[offset++] = 1.0f;

    //원통의 아랫부분
    arr[offset++] = 0;
    arr[offset++] = 0;
    arr[offset++] = h;

    arr[offset++] = 0.0f;
    arr[offset++] = 1.0f;
    arr[offset++] = 1.0f;

    *array_size = offset;
    *point_cnt = 2;

    for(int deg=0; deg< 360; deg = deg + inc, idx++){

        switch(plane){
            case PLANE_Z:     // z-plane
            x1 = glm::cos(glm::radians((float)deg));
            y1 = glm::sin(glm::radians((float)deg));
            //SPDLOG_INFO("pos x: {}, y: {}, z: {}", x,y,z);
            break;

            case PLANE_Y:     // y-plane
            x1 = glm::cos(glm::radians((float)deg));
            z1 = glm::sin(glm::radians((float)deg));
            break;

            case PLANE_X:     // x-plane
            y1 = glm::cos(glm::radians((float)deg));
            z1 = glm::sin(glm::radians((float)deg));
            break;
        }
        
        //원통의 윗부분
        arr[offset + idx*12 + 0] = x1;
        arr[offset + idx*12 + 1] = y1;
        arr[offset + idx*12 + 2] = z1;

        arr[offset + idx*12 + 3] = 0.0f + ((float)idx/resol);
        arr[offset + idx*12 + 4] = 0.0f;
        arr[offset + idx*12 + 5] = 1.0f;

        //원통의 아랫부분
        arr[offset + idx*12 + 6] = x1;
        arr[offset + idx*12 + 7] = y1;
        arr[offset + idx*12 + 8] = h;

        arr[offset + idx*12 + 9] = 0.0f + ((float)idx/resol);
        arr[offset + idx*12 + 10] = 0.0f;
        arr[offset + idx*12 + 11] = 1.0f;
        
        (*array_size) += 12;
        (*point_cnt) += 2;
    }


    SPDLOG_INFO("2D circle resolution {}-deg/360", resol);
    SPDLOG_INFO("2D circle Vertex number : {}", *point_cnt);
    SPDLOG_INFO("2D circle Array Size: {}", *array_size); 

   //arrCircle = arr;
    //circleResol = resol;

    return arr;
}