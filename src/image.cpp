#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

std::unique_ptr<Image> Image::Load(const std::string& filepath){

    auto image = ImageUPtr(new Image());

    if(image->LoadWithStb(filepath) == false){
        return nullptr;
    }

    return std::move(image);
}

std::unique_ptr<Image> Image::CreateSingleColorImage(int width, int height,
        const glm::vec4& color){

    // 0~1사이의 color 를 0 ~ 255 정수로 변환  d
    glm::vec4 clamped = glm::clamp(color * 255.0f, 0.0f, 255.0f);

    u8 rgba[4] = {
        (u8)clamped.r,
        (u8)clamped.g,
        (u8)clamped.b,
        (u8)clamped.a,
    };

    auto image = Create(width, height, 4);

    for(int idx=0; idx < width * height; ++idx){

        memcpy(image->m_data + 4 * idx, rgba, 4);
    }

    return std::move(image);
}



Image::~Image(){

    if(m_data){
        stbi_image_free(m_data);
    }
}


/*
정의가 static std::unique_ptr<Image> Create(int width, int height, int channelCount = 4);
로  되어 있어도, 실제 구현부에서는 아래와 같이 사용
 */
std::unique_ptr<Image> Image::Create(int width, int height, int channelCount){
    // parameter로 넘어온 세팅을 이용하여 거기에 맞는 empty 메모리를 확보하고 포인터 반납 

    auto image = std::unique_ptr<Image>(new Image());

    if(!image->Allocate(width, height, channelCount)){
        return nullptr;
    }

    return std::move(image);
}


void Image::SetCheckImage(int gridX, int gridY){
    // 
    for(int jdx=0; jdx < m_height; ++jdx){

        for(int idx=0; idx < m_width; ++idx){

            int pos = (jdx*m_width + idx) * m_channelCount;

            bool even = ( (idx/gridX) + (jdx/gridY)) % 2 == 0;

            uint8_t value = even ? 255 : 0; // even ? yes => white, no => black

            for(int kdx=0; kdx < m_channelCount; ++kdx){

                m_data[pos+kdx] = value;        
            }

            if(m_channelCount > 3)
                m_data[3] = 255;

        }
    }

}



bool Image::Allocate(int width, int height, int channelCount){
    m_width = width;
    m_height = height;

    m_channelCount = channelCount;

    m_data = (uint8_t*)malloc(m_width*m_height*m_channelCount);

    return m_data != nullptr ? true : false;

}


bool Image::LoadWithStb(const std::string& filepath){

    /* 
    stbi_set_flip_vertically_on_load(true); 를 사용하지 않으면 이미지 상하반전
    보통의 이미지는 좌상단을 원점으로 하는데 OpenGL은 좌하단을 원점으로 사용
    따라서 아래 함수가 없으면 상하 반전됨
    */
    stbi_set_flip_vertically_on_load(true);

    m_data = stbi_load(filepath.c_str(), &m_width, &m_height, &m_channelCount, 0);

    if(!m_data){

        SPDLOG_ERROR("failed to load image: {}", filepath);
        return false;
    }

    return true;
}