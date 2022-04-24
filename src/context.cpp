#include "context.h"
#include "image.h"


std::unique_ptr<Context> Context::Create() {

    auto context = std::unique_ptr<Context>(new Context());

    if (!context->Init())
        return nullptr;

    return std::move(context);
}

void Context::Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->Use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_LINE_STRIP, 0, 7);
}

bool Context::Init() {
    float vertices[] = {
        // Format [X, Y, Z, R, G, B, S, T]
        //        (3D 좌표) (RGB색상 ) (텍스처좌표)

        //X    Y      Z     R     G     B     S     T
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right, red
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right, green
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left, blue
        -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left, yellow
    };

    uint32_t indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    m_vertexLayout = VertexLayout::Create();
    m_vertexBuffer = Buffer::CreateWithData( GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 32);


    // XYZ coordinates
    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
    // RGB
    m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, sizeof(float)*3);
    // Texture ST coordinates
    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, sizeof(float)*6);



    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

    std::shared_ptr<Shader> vertShader = Shader::CreateFromFile("../shader/simple.vs", GL_VERTEX_SHADER);
    std::shared_ptr<Shader> fragShader = Shader::CreateFromFile("../shader/simple.fs", GL_FRAGMENT_SHADER);

    if (!vertShader || !fragShader)
        return false;

    SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

    m_program = Program::Create({fragShader, vertShader});

    if (!m_program)
        return false;

    SPDLOG_INFO("program id: {}", m_program->Get());   
 

 
    auto image = Image::Load("../image/container.jpg");

    if(image == nullptr){
        return false;
    } 

    SPDLOG_INFO("Image: {}x{}, {} channels", image->GetWidth(), image->GetHeight(), image->GetChannelCount());

    m_texture = Texture::CreateFromImage(image.get());

    if(!m_texture){
        SPDLOG_ERROR("Texture loading failed..");
    }

    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

    return true;
}
