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

        if(ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen)){
            ImGui::DragFloat("l.distance", &m_light.distance, 0.5f, 0.0f, 3000.0f);
            ImGui::DragFloat2("l.cutoff", glm::value_ptr(m_light.cutoff), 0.5f, 0.0f, 90.0f);
            ImGui::DragFloat3("l.position", glm::value_ptr(m_light.position), 0.01f);
            ImGui::DragFloat3("l.direction", glm::value_ptr(m_light.direction), 0.01f);
            ImGui::ColorEdit3("l.ambient", glm::value_ptr(m_light.ambient));
            ImGui::ColorEdit3("l.diffuse", glm::value_ptr(m_light.diffuse));
            ImGui::ColorEdit3("l.specular", glm::value_ptr(m_light.specular));

            ImGui::Checkbox("flash light", &m_flashLightMode);
        }

        if(ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)){
            //ImGui::ColorEdit3("m.specular", glm::value_ptr(m_material.specular));
            //ImGui::DragFloat("m.shininess", &m_material.shininess, 1.0f, 1.0f, 256.0f);

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
    glm::vec3 lightPos = m_light.position;
    glm::vec3 lightDir = m_light.direction;

    if(m_flashLightMode){

        lightPos = m_cameraPos;
        lightDir = m_cameraFront;
    }
    else {

        auto lightModelTransform = glm::translate(glm::mat4(1.0), m_light.position) * 
            glm::scale(glm::mat4(1.0), glm::vec3(0.1f));

        m_simpleProgram->Use();
        m_simpleProgram->SetUniform("color", glm::vec4(m_light.ambient + m_light.diffuse, 1.0f));
        m_simpleProgram->SetUniform("transform", projection * view * lightModelTransform);
        m_box->Draw(m_simpleProgram.get());
    }
    /* ********************************************************************************************************* */



    /* ***************************************** Render Scene ****************************************** */
    m_program->Use();
    m_program->SetUniform("viewPos", m_cameraPos);

    m_program->SetUniform("light.position", lightPos);
    m_program->SetUniform("light.direction", lightDir);
    m_program->SetUniform("light.cutoff", glm::vec2(
        cosf(glm::radians(m_light.cutoff[0])),
        cosf(glm::radians(m_light.cutoff[0] + m_light.cutoff[1])) ));
    m_program->SetUniform("light.attenuation", GetAttenuationCoeff(m_light.distance));
    m_program->SetUniform("light.ambient", m_light.ambient);
    m_program->SetUniform("light.diffuse", m_light.diffuse);
    m_program->SetUniform("light.specular", m_light.specular);

    // m_program->SetUniform("material.diffuse", 0);               // texture slot number
    // m_program->SetUniform("material.specular", 1);
    // m_program->SetUniform("material.shininess", m_box1Material->shininess);

    auto modelTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 1.0f, 10.0f));

    auto tranform = projection * view * modelTransform;

    m_program->SetUniform("transform", tranform);
    m_program->SetUniform("modelTransform", modelTransform);
    m_planeMaterial->SetToProgram(m_program.get());
    m_box->Draw(m_program.get());



    modelTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.75f, -4.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.5f));

    tranform = projection * view * modelTransform;

    m_program->SetUniform("transform", tranform);
    m_program->SetUniform("modelTransform", modelTransform);
    m_box1Material->SetToProgram(m_program.get());
    m_box->Draw(m_program.get());



    modelTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.75f, 2.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.5f));

    tranform = projection * view * modelTransform;

    m_program->SetUniform("transform", tranform);
    m_program->SetUniform("modelTransform", modelTransform);
    m_box2Material->SetToProgram(m_program.get());
    m_box->Draw(m_program.get());
}

bool Context::Init() {

    m_box = Mesh::MakeBox();
    // m_model = Model::Load("../models/backpack/backpack.obj");

    // if(!m_model){
    //     return false;
    // }


    /* ******************************* Shader Program Load ******************************************** */
    
    m_program = Program::Create("../shader/lighting.vs", "../shader/lighting.fs");

    if(!m_program){
        return false;
    }

    m_simpleProgram = Program::Create("../shader/simple.vs", "../shader/simple.fs");

    if(!m_simpleProgram){
        return false;
    }

    /* *************************************************************************************************** */

    // m_material.diffuse = Texture::CreateFromImage(
    //     Image::CreateSingleColorImage(4, 4, glm::vec4(1.0f,1.0f,1.0f,1.0f)).get()
    // );

    // m_material.specular = Texture::CreateFromImage(
    //     Image::CreateSingleColorImage(4, 4, glm::vec4(0.5f,0.5f,0.5f,0.5f)).get()
    // );

    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, m_texture->Get());
    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, m_texture2->Get());

    // m_program->Use();
    // m_program->SetUniform("tex", 0);
    // m_program->SetUniform("tex2", 1);

    std::shared_ptr<Texture> darkGrayTexture = Texture::CreateFromImage(
        Image::CreateSingleColorImage(4, 4, glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)).get()
    );

    std::shared_ptr<Texture> GrayTexture = Texture::CreateFromImage(
        Image::CreateSingleColorImage(4, 4, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)).get()
    );


    m_planeMaterial = Material::Create();
    m_planeMaterial->diffuse = Texture::CreateFromImage(
        Image::Load("../image/marble.jpg").get()
    );

    m_planeMaterial->specular = GrayTexture;
    m_planeMaterial->shininess = 128.0f;



    m_box1Material = Material::Create();
    m_box1Material->diffuse = Texture::CreateFromImage(
        Image::Load("../image/container.jpg").get()
    );

    m_box1Material->specular = darkGrayTexture;
    m_box1Material->shininess = 16.0f;



    m_box2Material = Material::Create();
    m_box2Material->diffuse = Texture::CreateFromImage(
        Image::Load("../image/container2.png").get()
    );

    m_box2Material->specular = Texture::CreateFromImage(
        Image::Load("../image/container2_specular.png").get()
    );
    m_box2Material->shininess = 64.0f;



    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
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
