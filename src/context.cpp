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
    auto image2 = Image::Load("../image/awesomeface.png");

    if(image == nullptr || image2 == nullptr){
        return false;
    } 

    SPDLOG_INFO("Image 1: {}x{}, {} channels", image->GetWidth(), image->GetHeight(), image->GetChannelCount());
    SPDLOG_INFO("Image 2: {}x{}, {} channels", image2->GetWidth(), image2->GetHeight(), image2->GetChannelCount());


    // auto image = Image::Create(512, 512);
    // image->SetCheckImage(16,16);
    m_texture = Texture::CreateFromImage(image.get());

    m_texture2 = Texture::CreateFromImage(image2.get());

    if(!m_texture){
        SPDLOG_ERROR("Texture loading failed..");
    }

    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->Get());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture2->Get());

    m_program->Use();

    // use texture slot no.0
    glUniform1i(glGetUniformLocation(m_program->Get(), "tex"), 0);

    // use texture slot no.1
    glUniform1i(glGetUniformLocation(m_program->Get(), "tex2"), 1);

    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    // 단위행렬 기준 (1,1,0)만큼 평행이동하는 행렬
    auto trans = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
    // 단위행렬 기준 z축으로 90도 회전행렬
    auto rot = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // 3배 확대 행렬
    auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f));
    // 확대 -> 회전 -> 평행이동 순으로 진행됨(행렬곱셈)
    //vec = trans * rot * scale * vec;
    vec = trans * vec;
    SPDLOG_INFO("translate vec: [{}, {}, {}]", vec.x, vec.y, vec.z);




    return true;
}
