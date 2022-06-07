#include "context.h"
#include "image.h"
#include <imgui.h>

static std::vector<glm::vec3> cubePositions = {
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


std::unique_ptr<Context> Context::Create() {

    auto context = std::unique_ptr<Context>(new Context());

    if (!context->Init())
        return nullptr;

    return std::move(context);
}

void Context::Render() {

    static float temp = 0.0f;

    /* ****************************************** ImGui Rendering ****************************************** */

    if(ImGui::Begin("ui window")){

        if(ImGui::ColorEdit4("clear color", glm::value_ptr(m_clearColor)) ) {
            glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
        }

        ImGui::Separator();
        ImGui::DragFloat3("camera pos", glm::value_ptr(m_cameraPos), 0.01f);
        ImGui::DragFloat("camera yaw", &m_cameraYaw, 0.5f);
        ImGui::DragFloat("camera pitch", &m_cameraPitch, 0.5f, -89.0f, 89.0f);
        ImGui::Separator();
        if(ImGui::Button("Reset camera")){
            m_cameraYaw = 0.0f;
            m_cameraPitch = 0.0f;
            m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        }

        if(ImGui::CollapsingHeader("light", ImGuiTreeNodeFlags_DefaultOpen)){
            ImGui::DragFloat3("light pos", glm::value_ptr(m_lightPos), 0.01f);
            ImGui::ColorEdit3("light color", glm::value_ptr(m_lightColor));
            ImGui::ColorEdit3("object color", glm::value_ptr(m_objectColor));
            ImGui::SliderFloat("ambient strength", &m_ambientStrength, 0.0f, 1.0f);

            ImGui::SliderFloat("specular strength", &m_specularStrength, 0.0f, 1.0f);
            ImGui::DragFloat("specular shininess", &m_specularShininess, 1.0f, 1.0f, 256.0f);
        }

        ImGui::Checkbox("animation", &m_animation);
    }
    ImGui::End();
    /* ********************************************************************************************************* */
    

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    /* **************************************** Local좌표를 카메라 좌표로 변환 *************************************** */
    m_cameraFront = glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
                        glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f) ) *
                        glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);     // (0.0f, 0.0f, -1.0f, 0.0f)에서 마지막에 0.0을 넣은 것은 평행이동이 안되게 하기 위함

    // 종횡비 4:3, 세로화각 45도의 원근투영
    auto projection = glm::perspective(glm::radians(45.0f),
        (float)m_width/(float)m_height, 0.01f, 20.0f);


    // 카메라의 3축의 단위벡터로부터 카메라 뷰 행렬을 계산하는 glm 함수
    auto view = glm::lookAt(m_cameraPos, m_cameraFront+m_cameraPos, m_cameraUp);

    /* ********************************************************************************************************* */



    /* ****************************************** 광원에 작은 큐브 그리기 ****************************************** */
    auto lightModelTransform = glm::translate(glm::mat4(1.0), m_lightPos) * glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
    m_program->Use();
    m_program->SetUniform("lightPos", m_lightPos);
    m_program->SetUniform("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    m_program->SetUniform("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
    m_program->SetUniform("ambientStrength", 1.0f);
    m_program->SetUniform("transform", projection*view*lightModelTransform);
    m_program->SetUniform("modelTransform", lightModelTransform);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    /* ********************************************************************************************************* */


    /* ***************************************** 회전하는 큐브 여러개 그리기 ****************************************** */
    m_program->Use();
    m_program->SetUniform("lightPos", m_lightPos);
    m_program->SetUniform("lightColor", m_lightColor);
    m_program->SetUniform("objectColor", m_objectColor);
    m_program->SetUniform("ambientStrength", m_ambientStrength);

    m_program->SetUniform("viewPos", m_cameraPos);
    m_program->SetUniform("specularStrength", m_specularStrength);
    m_program->SetUniform("specularShininess", m_specularShininess);

    for(int idx=0; idx<cubePositions.size(); ++idx){
        auto& pos = cubePositions[idx];

        auto model = glm::translate(glm::mat4(1.0f), pos);
        //model =glm::rotate(model, glm::radians((float)glfwGetTime()*120.0f  + 20*(float)idx), glm::vec3(1.0f, 0.2f, 0.0f));
        model =glm::rotate(model, 
            glm::radians((m_animation ? (float)glfwGetTime() : 0.0f) *120.0f + 20*(float)idx), 
            glm::vec3(1.0f, 0.2f, 0.0f));

        auto transform = projection * view * model;
        m_program->SetUniform("transform", transform);
        m_program->SetUniform("modelTransform", model);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    /* ********************************************************************************************************* */
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
    //  posX    posY   posZ   normX  normY  normZ  texS  texT    
         -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
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
    m_vertexBuffer = Buffer::CreateWithData( GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 8 * 4 * 6);


    // XYZ coordinates
    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);

    // Normal
    m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, sizeof(float)*3);

    // Texture ST coordinates
    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, sizeof(float)*6);



    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 36);

    std::shared_ptr<Shader> vertShader = Shader::CreateFromFile("../shader/lighting.vs", GL_VERTEX_SHADER);
    std::shared_ptr<Shader> fragShader = Shader::CreateFromFile("../shader/lighting.fs", GL_FRAGMENT_SHADER);

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

    if(!m_cameraControl)
        return;

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

void Context::MouseMove(double x, double y){

     const float cameraRotSpeed = 0.8f;

    if(!m_cameraControl)
        return;

    auto pos = glm::vec2((float)x, (float)y);
    auto deltaPos = pos - m_prevMousePos;

    // static glm::vec2 prevPos = glm::vec2((float)x, (float)y);
    // auto pos = glm::vec2((float)x, (float)y);
    // auto deltaPos = pos - prevPos;

    m_cameraYaw -= deltaPos.x * cameraRotSpeed;
    m_cameraPitch -= deltaPos.y * cameraRotSpeed;

    if(m_cameraYaw < 0.0f)  
        m_cameraYaw += 360.0f;

    if(m_cameraYaw > 360.0f)  
        m_cameraYaw -= 360.0f;

    if(m_cameraPitch > 89.0f)  
        m_cameraPitch = 89.0f;

    if(m_cameraPitch < -89.0f)  
        m_cameraPitch = -89.0f;

    //prevPos = pos;
    m_prevMousePos = pos;
}


void Context::MouseButton(int btn, int action, double x, double y){

    if(btn == GLFW_MOUSE_BUTTON_RIGHT){

        if(action == GLFW_PRESS){
            m_prevMousePos = glm::vec2((float)x, (float)y);
            m_cameraControl = true;
        }
        else if(action == GLFW_RELEASE){
            m_cameraControl = false;
        }
    }
}


void Context::Reshape(int width, int height){
    m_width = width;
    m_height = height;

    glViewport(0, 0, m_width, m_height);

}
