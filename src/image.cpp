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

Image::~Image(){

    if(m_data){
        stbi_image_free(m_data);
    }
}


std::unique_ptr<Image> Image::Create(int width, int height, int channelCount = 4){
    // parameter로 넘어온 세팅을 이용하여 거기에 맞는 empty 메모리를 확보하고 포인터 반납 

    auto image = std::unique_ptr<Image>(new Image());

    if(!image->Allocate(width, height, channelCount)){
        return nullptr;
    }

    return std::move(image);
}


void Image::SetCheckImage(int gridX, int gridY){
    // 

}



bool Image::Allocate(int width, int height, int channelCount){
    m_width = width;
    m_height = height;

    m_channelCount = channelCount;

    m_data = (uint8_t*)malloc(m_width*m_height*m_channelCount);

    return m_data != nullptr ? true : false;

}


bool Image::LoadWithStb(const std::string& filepath){

    m_data = stbi_load(filepath.c_str(), &m_width, &m_height, &m_channelCount, 0);

    if(!m_data){

        SPDLOG_ERROR("failed to load image: {}", filepath);
        return false;
    }

    return true;
}