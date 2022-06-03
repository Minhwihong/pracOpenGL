#include "context.h"
#include "polygon.h"
#include "image.h"
#include <imgui.h>


float* circle2D_z;
float* circle2D_y;
float* cylinder;
uint32_t* elem_cylinder;
uint32_t elem_cylinder_len = 0;

uint32_t* element_circle2Dz;
uint32_t* element_circle2Dy;

size_t verCnt_2dCircle_z = 0;
size_t size_2dCircle_y = 0;
size_t size_cylinder;
size_t size_arr_cylinder;

int circle_resol = 0;


gl_test::Polygon poly = gl_test::Polygon();

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




GLfloat triangle_vertices[] = {
    -3.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     3.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.0f, -3.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.0f,  3.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.0f,  0.0f,-3.0f, 0.0f, 0.0f, 1.0f,
     0.0f,  0.0f, 3.0f, 0.0f, 0.0f, 1.0f,
};





static unsigned int line_buf;
static unsigned int vao_line;

std::unique_ptr<Context> Context::Create() {

    auto context = std::unique_ptr<Context>(new Context());

    if (!context->Init())
        return nullptr;

    return std::move(context);
}

void Context::Render() {

    static float temp = 0.0f;
    static int iTemp = 0;
    static int iTemp2 = 0;

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
    }
    ImGui::End();

    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

   
    m_cameraFront = glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
                        glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f) ) *
                        glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);     // (0.0f, 0.0f, -1.0f, 0.0f)에서 마지막에 0.0을 넣은 것은 평행이동이 안되게 하기 위함

    // 종횡비 4:3, 세로화각 45도의 원근투영
    auto projection = glm::perspective(glm::radians(45.0f),
        (float)m_width/(float)m_height, 0.01f, 20.0f);


    // 카메라의 3축의 단위벡터로부터 카메라 뷰 행렬을 계산하는 glm 함수
    auto view = glm::lookAt(m_cameraPos, m_cameraFront+m_cameraPos, m_cameraUp);

    auto pos = glm::vec3( glm::sin(temp), glm::cos(temp), 0.0f);
    temp = temp + 0.001f;

    auto model = glm::translate(glm::mat4(1.0f), pos);
    model =glm::rotate(model, glm::radians((float)glfwGetTime()*120.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    auto transform = projection * view * model;


    auto org = glm::vec3( 0.0f, 0.0f, 0.0f);
    auto org_model = glm::translate(glm::mat4(1.0f), org);  // (0,0,0)의 벡터를 (0,0,0,1)의 동차좌표계로 변환(affine 공간)

    m_tex_program->Use();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    m_objVertexLayout->Bind();
    m_tex_program->SetUniform("transform", transform);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    m_poly_program->Use();

    m_lineLayout->Bind();
    m_poly_program->SetUniform("transform", projection * view * org_model);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_LINES, 0, 6);


    m_polyLayout->Bind();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glDrawArrays(GL_TRIANGLES, 0, size_cylinder);

    poly.ChangeColor(0.3f, 0.3f, 0.9f);
    m_polyclrBuf->DataModify(0, poly.GetVertexSize()*sizeof(float), poly.GetPtrColor());
    //glDrawElements(GL_TRIANGLES, poly.GetElemArrCnt(), GL_UNSIGNED_INT, 0);
    glDrawElements(GL_TRIANGLES, circle_resol*3, GL_UNSIGNED_INT, 0);

    poly.ChangeColor(0.9f, 0.3f, 0.1f);
    m_polyclrBuf->DataModify(0, poly.GetVertexSize()*sizeof(float), poly.GetPtrColor());
    glDrawElements(GL_TRIANGLES, circle_resol*8, GL_UNSIGNED_INT, (void*)(circle_resol*4));

    //poly.ChangeColor(0.2f, 0.9f, 0.1f);
    //m_polyclrBuf->DataModify(0, poly.GetVertexSize()*sizeof(float), poly.GetPtrColor());
    //glDrawElements(GL_TRIANGLES, circle_resol, GL_UNSIGNED_INT, &elem_cylinder[circle_resol*3]);

}

    

bool Context::Init() {


    for(int idx=0; idx<120; ++idx){
        
        int x = idx%5;
        
        if(x == 3 || x == 4){
        }
        else {
            vertices[idx] /= 2.0f;
        }
    }

    
    /* **************************** Coordinates Line VertexLayout&Buffer ******************************** */
    m_lineLayout = VertexLayout::Create();
    m_lineBuf = Buffer::CreateWithData( GL_ARRAY_BUFFER, GL_STATIC_DRAW, triangle_vertices, sizeof(float)*18 * 6);
    m_lineLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
    m_lineLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, sizeof(float)*3);
    /* ************************************************************************************************** */


    /* ********************************* Polygon VertexLayout&Buffer ************************************ */
    
    circle_resol = 45;


    poly.Make_3Dcylinder(0.5f, 1.5f, circle_resol);
    elem_cylinder = poly.GetElementArr_3Dcylinder(poly.GetPtrVertex(), circle_resol);

    m_polyLayout= VertexLayout::Create();

    m_polyPosBuf = Buffer::CreateWithData( GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW, poly.GetPtrVertex(), sizeof(float)*poly.GetVertexSize());
    //m_polyLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
    m_polyLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    m_polyclrBuf = Buffer::CreateWithData( GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW, poly.GetPtrColor(), sizeof(float)*poly.GetVertexSize());
    m_polyLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);
    //m_polyLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, sizeof(float)*3);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    elem_cylinder_len = poly.GetElemArrCnt();
    //size_cylinder = 3* poly.GetVertexCnt();
    
    m_idxPolyBuf = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW, elem_cylinder, sizeof(uint32_t)*poly.GetElemArrCnt());
    /* ************************************************************************************************** */

    

    /* ************************************** Cube & Texture **************************************** */
    m_objVertexLayout = VertexLayout::Create();
    m_objVertexBuf = Buffer::CreateWithData( GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 120);

    // XYZ coordinates
    m_objVertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);

    // RGB
    //m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, sizeof(float)*3);

    // Texture ST coordinates
    m_objVertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*5, sizeof(float)*3);
    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 36);
    /* ************************************************************************************************** */



    /* ***************************** Shader for Texture Program Create ********************************** */
    std::shared_ptr<Shader> vertShader = Shader::CreateFromFile("../../shader/simple.vs", GL_VERTEX_SHADER);
    std::shared_ptr<Shader> fragShader = Shader::CreateFromFile("../../shader/simple.fs", GL_FRAGMENT_SHADER);

    if (!vertShader || !fragShader)
        return false;

    SPDLOG_INFO("texture vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("texture fragment shader id: {}", fragShader->Get());

    m_tex_program = Program::Create({fragShader, vertShader});

    if (!m_tex_program)
        return false;

    SPDLOG_INFO("texture program id: {}", m_tex_program->Get());   
    /* ************************************************************************************************** */



     /* **************************** Shader for Line&Polygon Program Create ***************************** */
    std::shared_ptr<Shader> vertShader_poly = Shader::CreateFromFile("../../shader/polygon.vs", GL_VERTEX_SHADER);
    std::shared_ptr<Shader> fragShader_poly = Shader::CreateFromFile("../../shader/polygon.fs", GL_FRAGMENT_SHADER);

    if (!vertShader_poly || !fragShader_poly)
        return false;

    SPDLOG_INFO("polygon vertex shader id: {}", vertShader_poly->Get());
    SPDLOG_INFO("polygon fragment shader id: {}", fragShader_poly->Get());

    m_poly_program = Program::Create({fragShader_poly, vertShader_poly});

    if (!m_poly_program)
        return false;

    SPDLOG_INFO("polygon program id: {}", m_poly_program->Get());   
    /* ***************************************************************************************************** */
 


    auto image = Image::Load("../../image/container.jpg");
    auto image2 = Image::Load("../../image/awesomeface.png");

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

    m_tex_program->Use();
    // use texture slot no.0
    m_tex_program->SetUniform("tex", 0);
    // use texture slot no.1
    m_tex_program->SetUniform("tex2", 1);
 
    return true;
}



void Context::ProcessInput(GLFWwindow* window){

    const float cameraSpeed = 0.01f;

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

     const float cameraRotSpeed = 0.1f;

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
