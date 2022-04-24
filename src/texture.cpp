#include "texture.h"


std::unique_ptr<Texture> Texture::CreateFromImage(const Image* image){

    auto texture = std::unique_ptr<Texture>(new Texture());

    texture->CreateTexture();
    texture->SetTextureFromImage(image);

    return std::move(texture);
}

Texture::~Texture(){
    if(m_texture){
        glDeleteTextures(1, &m_texture);
        
    }
}


void Texture::Bind() const {

    // openGL texture object create and bind to m_texture
    // m_texture로 바인딩이 되면 앞으로 설정할 텍스터 관련 설정함수(glTexParameteri,glTexImage2D등)은 m_texture와 연결됨
    glBindTexture(GL_TEXTURE_2D, m_texture);
}


void Texture::SetFilter(uint32_t minFilter, uint32_t magFilter) const {

    // glTexParameter(i) -> int type parameter set
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}


void Texture::SetWrap(uint32_t sWrap, uint32_t tWrap) const {

    // GL_TEXTURE_WRAP_S -> X축,  GL_TEXTURE_WRAP_T -> Y축
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);
}



void Texture::CreateTexture(){

    glGenTextures(1, &m_texture);

    Bind();
    SetFilter(GL_LINEAR, GL_LINEAR);
    SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

}



void Texture::SetTextureFromImage(const Image* image){

    GLenum format = GL_RGBA;

    switch(image->GetChannelCount()){

        case 1: format = GL_RED; break;
        case 2: format = GL_RG; break;
        case 3: format = GL_RGB; break;
        default:    break;
    }

    //(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, 
    //     GLenum format, GLenum type, const void *pixels);
    // GPU쪽의 텍스처 데이터를 기술함 : GL_TEXTURE_2D, 0, GL_RGB, image->GetWidth(), image->GetHeight(), 0
    // CPU쪽의 텍스처 데이터를 기술함 : GL_RGB, GL_UNSIGNED_BYTE, image->GetData()
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->GetWidth(), image->GetHeight(), 0, 
        format, GL_UNSIGNED_BYTE, image->GetData());

}