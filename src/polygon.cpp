#include "polygon.h"


#define RESOLUTION_CIRCLE   20
#define BUF_CNT_CIRCLE     (int)(360 / RESOLUTION_CIRCLE)

float* Polygon::Make_Circle(float r, int resol, size_t* point_cnt, size_t* array_size, uint32_t color){

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

        x = glm::cos(glm::radians((float)deg));
        y = glm::sin(glm::radians((float)deg));
        
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


bool Polygon::Make_3Dcylinder(float r, float h, int resol){

    size_t circle_ver_cnt = (size_t)(360 * (float)resol / 360 * 6);
    size_t point_cnt = 0;
    size_t array_size = 0;
    
    //float* arr = new float[circle_ver_cnt*2 + 12]; 

                
    ptrVerex = new float[(size_t)(360 * (float)resol / 360*6 + 3 + 3)];
    ptrColor = new float[(size_t)(360 * (float)resol / 360*6 + 3 + 3)];

    float x1 = 0.0f;
    float y1 = 0.0f;
    float z1 = 0.0f;
    //h = 0.5;

    int idx = 0;
    int inc = 360 / resol;
    int offset_vert = 0;
    int offset_clr = 0;


    point_cnt = 0;

    //원통의 윗부분
    ptrVerex[offset_vert++] = 0;
    ptrVerex[offset_vert++] = 0;
    ptrVerex[offset_vert++] = 0;

    ptrColor[offset_clr++] = 0.0f;
    ptrColor[offset_clr++] = 1.0f;
    ptrColor[offset_clr++] = 1.0f;

    //원통의 아랫부분
    ptrVerex[offset_vert++] = 0;
    ptrVerex[offset_vert++] = 0;
    ptrVerex[offset_vert++] = h;

    ptrColor[offset_clr++] = 0.0f;
    ptrColor[offset_clr++] = 1.0f;
    ptrColor[offset_clr++] = 1.0f;

    array_size = offset_vert;
    point_cnt = 2;

    for(int deg=0; deg< 360; deg = deg + inc, idx++){

        x1 = glm::cos(glm::radians((float)deg));
        y1 = glm::sin(glm::radians((float)deg));
        
        //원통의 윗부분
        ptrVerex[offset_vert + idx*6 + 0] = x1*r;
        ptrVerex[offset_vert + idx*6 + 1] = y1*r;
        ptrVerex[offset_vert + idx*6 + 2] = z1;

        ptrColor[offset_clr + idx*6 + 0] = 0.0f + ((float)idx/resol);
        ptrColor[offset_clr + idx*6 + 1] = 0.0f;
        ptrColor[offset_clr + idx*6 + 2] = 1.0f;

        //원통의 아랫부분
        ptrVerex[offset_vert + idx*6 + 3] = x1*r;
        ptrVerex[offset_vert + idx*6 + 4] = y1*r;
        ptrVerex[offset_vert + idx*6 + 5] = h;

        ptrColor[offset_clr + idx*6 + 3] = 0.0f + ((float)idx/resol);
        ptrColor[offset_clr + idx*6 + 4] = 1.0f;
        ptrColor[offset_clr + idx*6 + 5] = 0.0f;
        
        array_size += 6;
        point_cnt += 2;
    }

    vertex_cnt = point_cnt;
    vertex_size = array_size;

    SPDLOG_INFO("3D cylinder resolution {}-deg/360", resol);
    SPDLOG_INFO("3D cylinder Vertex cnt : {}", vertex_cnt);
    SPDLOG_INFO("3D cylinder Array Size: {}", array_size); 

    return true;
}


uint32_t* Polygon::GetElementArr_3Dcylinder(float* src, size_t resolution){

    int idx = 0;
    int cnt = 0;

    uint32_t* arr = new uint32_t[resolution*12];

    for(idx=0; idx<resolution; ++idx){

        arr[cnt++] = 0;
        arr[cnt++] = idx*2 + 2;
        arr[cnt++] = idx*2 + 4;
    }


    for(idx=0; idx<resolution; ++idx){

        arr[cnt++] = idx*2 + 2;
        arr[cnt++] = idx*2 + 3;
        arr[cnt++] = idx*2 + 5;
    }

    for(idx=0; idx<resolution; ++idx){

        arr[cnt++] = idx*2 + 2;
        arr[cnt++] = idx*2 + 4;
        arr[cnt++] = idx*2 + 5;
    }

    for(idx=0; idx<resolution; ++idx){

        arr[cnt++] = idx*2 + 3;
        arr[cnt++] = idx*2 + 5;
        arr[cnt++] = 1;
    }

    element_arr_cnt = cnt;
    SPDLOG_INFO("3D cylinder Elem Array Size: {}", element_arr_cnt);
    

    return arr;

}



bool Polygon::Make_3Dcylinder_depth(float r, float h, int resol, int h_step){

    uint32_t side_cnt = (uint32_t)(360 * (float)resol / 360);


    std::vector<float> v_top((side_cnt + 1) * 3);
    std::vector<float> v_bottom((side_cnt + 1) * 3);
    std::vector<float> v_wall((side_cnt) * 3 * h_step);


    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    int idx=0;
    int jdx = 0;
    int inc = 360 / resol;

    float unit_h = h  * (float)h_step / h;

    auto iter_top = v_top.begin();
    auto iter_bottom = v_bottom.begin();
    auto iter_wall = v_wall.begin();

    *iter_top = 0;
    iter_top++;
    *iter_top = 0;
    iter_top++;
    *iter_top = 0;
    iter_top++;

    *iter_bottom = 0;
    iter_bottom++;
    *iter_bottom = 0;
    iter_bottom++;
    *iter_bottom = h;
    iter_bottom++;

    // top & bottom array
    for(int deg=0; deg< 360; deg = deg + inc){

        x = glm::cos(glm::radians((float)deg));
        y = glm::sin(glm::radians((float)deg));

        *iter_top = x;
        iter_top++;
        *iter_top = y;
        iter_top++;
        *iter_top = z;
        iter_top++;


        *iter_bottom = x;
        iter_bottom++;
        *iter_bottom = y;
        iter_bottom++;
        *iter_bottom = h;
        iter_bottom++;
    }

    idx = 0;
    jdx = 0;


    for(jdx=0; jdx<=h_step; ++jdx){

        for(int deg=0; deg< 360; deg = deg + inc, idx++){

            x = glm::cos(glm::radians((float)deg));
            y = glm::sin(glm::radians((float)deg));

            *iter_wall = x;
            iter_wall++;
            *iter_wall = y;
            iter_wall++;
            *iter_wall = unit_h*(float)jdx;
            iter_wall++;
            
        }
    }


    m_cylinder.h_step = h_step;
    m_cylinder.height = h;
    m_cylinder.radius = r;
    m_cylinder.resolution = resol;
    m_cylinder.ptr_bottom = &v_bottom;
    m_cylinder.ptr_top = &v_top;
    m_cylinder.ptr_wall = &v_wall;



    return true;
}


bool Polygon::GetElementArray(){


    uint32_t element_cnt  = 0;
    
    auto c = m_cylinder;

    std::vector<uint32_t> arr(element_cnt);

    std::vector<uint32_t> v_elm_top(c.resolution  * 3);
    std::vector<uint32_t> v_elm_bottom(c.resolution  * 3);
    std::vector<uint32_t> v_elm_wall(c.resolution  * 6 * c.h_step);



    auto iter_elm_top = v_elm_top.begin();
    auto iter_elm_bottom = v_elm_bottom.begin();
    auto iter_elm_wall = v_elm_wall.begin();

    int idx=0;

    for(idx=1; idx<c.resolution; ++idx){

        *iter_elm_top = 0;
        iter_elm_top++;
        *iter_elm_top = idx;
        iter_elm_top++;
        *iter_elm_top = idx+1;

        *iter_elm_bottom = 0;
        iter_elm_bottom++;
        *iter_elm_bottom = idx;
        iter_elm_bottom++;
        *iter_elm_bottom = idx+1;

        *iter_elm_wall = idx;
        iter_elm_wall++;
        *iter_elm_wall = idx + c.resolution + 1;
        iter_elm_wall++;
        *iter_elm_wall = idx + c.resolution + 2;

        *iter_elm_wall = idx;
        iter_elm_wall++;
        *iter_elm_wall = idx + c.resolution + 1;
        iter_elm_wall++;
        *iter_elm_wall = idx + 1;
    }



    return true;
}



void Polygon::ChangeColor(float r, float g, float b){

    for(int idx=0;idx<vertex_cnt; ++idx){
        ptrColor[idx*3 + 0] = r;
        ptrColor[idx*3 + 1] = g;
        ptrColor[idx*3 + 2] = b;
    }
}


