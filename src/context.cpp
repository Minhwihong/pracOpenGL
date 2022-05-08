#include "context.h"
#include "image.h"


std::unique_ptr<Context> Context::Create() {

    auto context = std::unique_ptr<Context>(new Context());

    if (!context->Init())
        return nullptr;

    return std::move(context);
}

void Context::Render() {

    std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f),
    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // 종횡비 4:3, 세로화각 45도의 원근투영
    auto projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.01f, 20.0f);


    // -1 from 1 sin value
    float xx = sinf((float)glfwGetTime() * glm::pi<float>() * 2.0f) * 3.0f;

    // 주변을 회전하는 카메라 (y축을 중심으로 회전)
    // float angle = glfwGetTime() * glm::pi<float>() * 0.5f;
    // auto x = sinf(angle) * 10.0f;
    // auto z = cosf(angle) * 10.0f;

    // auto cameraPos      = glm::vec3(x, 3.0f, z);
    // auto cameraTarget   = glm::vec3(0.0f, 0.0f, 0.0f);
    // auto cameraUp       = glm::vec3(0.0f, 1.0f, 0.0f);


    // 카메라의 3축의 단위벡터로부터 카메라 뷰 행렬을 계산하는 glm 함수
    auto view = glm::lookAt(m_cameraPos, m_cameraFront+m_cameraPos, m_cameraUp);

    for(int idx=0; idx<cubePositions.size(); ++idx){
        auto& pos = cubePositions[idx];

        auto model = glm::translate(glm::mat4(1.0f), pos);
        model =glm::rotate(model, glm::radians((float)glfwGetTime()*120.0f  + 20*(float)idx), glm::vec3(1.0f, 0.2f, 0.0f));

        auto transform = projection * view * model;
        m_program->SetUniform("transform", transform);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

}

bool Context::Init() {
    // float vertices[] = {
    //     // Format [X, Y, Z, R, G, B, S, T]
    //     //        (3D 좌표) (RGB색상 ) (텍스처좌표)

    //     //X    Y      Z     R     G     B     S     T
    //     0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right, red
    //     0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right, green
    //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left, blue
    //     -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left, yellow
    // };


    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
    };

    uint32_t indices[] = {
         0,  2,  1,  2,  0,  3,
         4,  5,  6,  6,  7,  4,
         8,  9, 10, 10, 11,  8,
        12, 14, 13, 14, 12, 15,
        16, 17, 18, 18, 19, 16,
        20, 22, 21, 22, 20, 23,
    };

    m_vertexLayout = VertexLayout::Create();
    m_vertexBuffer = Buffer::CreateWithData( GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 120);


    // XYZ coordinates
    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
    // RGB
    //m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, sizeof(float)*3);
    // Texture ST coordinates
    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*5, sizeof(float)*3);



    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 36);

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
    m_program->SetUniform("tex", 0);
    // use texture slot no.1
    m_program->SetUniform("tex2", 1);
 
    return true;
}



void Context::ProcessInput(GLFWwindow* window){

    const float cameraSpeed = 0.05f;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        m_cameraPos += cameraSpeed * m_cameraFront;
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        m_cameraPos -= cameraSpeed * m_cameraFront;
    }

    auto cameraRight = glm::normalize(glm::cross(m_cameraUp, -m_cameraFront));

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        m_cameraPos -= cameraSpeed * cameraRight;
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        m_cameraPos += cameraSpeed * cameraRight;
    }

    auto cameraUp = glm::normalize(glm::cross(-m_cameraFront, cameraRight));

    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        m_cameraPos -= cameraSpeed * cameraUp;
    }

    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        m_cameraPos += cameraSpeed * cameraUp;
    }

}



