#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "texture.h"
#include "mesh.h"


struct Light {
    glm::vec3 direction { glm::vec3(-1.0f, -1.0f, -1.0f)};
    glm::vec2 cutoff { glm::vec2(20.0f, 5.0f)};

    glm::vec3 position { glm::vec3(2.0f, 2.0f, 2.0f)};
    float distance{32.0f};
    
    glm::vec3 ambient {  glm::vec3(0.1f, 0.1f, 0.1f)};
    glm::vec3 diffuse {  glm::vec3(0.5f, 0.5f, 0.5f)};
    glm::vec3 specular { glm::vec3(1.0f, 1.0f, 1.0f)};
};

struct Material{

    //glm::vec3 specular { glm::vec3(0.5f, 0.5f, 0.5f)};
    float shininess { 32.0f};

    std::unique_ptr<Texture> diffuse;
    std::unique_ptr<Texture> specular;
};

CLASS_PTR(Context)
class Context {
public:
    //static ContextUPtr Create();
    static std::unique_ptr<Context> Create();
    void Render();  
    void ProcessInput(GLFWwindow* window);
    void Reshape(int width, int height);
    void MouseMove(double x, double y);
    void MouseButton(int btn, int action, double x, double y);

private:
    Context() {}
    bool Init();
    
    std::unique_ptr<Program>  m_program;
    std::unique_ptr<Program> m_simpleProgram;

    // std::unique_ptr<VertexLayout> m_vertexLayout;

    // std::unique_ptr<Buffer> m_vertexBuffer;
    // std::unique_ptr<Buffer> m_indexBuffer;

    std::unique_ptr<Mesh> m_box;

    std::unique_ptr<Texture> m_texture;
    std::unique_ptr<Texture> m_texture2;

    glm::vec4 m_clearColor {glm::vec4(0.1f, 0.2f, 0.3f, 0.0f)};


    // animation (for testing)
    bool m_animation { true };


    // light param
    Light m_light;
    Material m_material;



    // camera param
    bool m_cameraControl {false};
    glm::vec2 m_prevMousePos {glm::vec2(0.0f)};
    float m_cameraPitch {0.0f};
    float m_cameraYaw {0.0f};
    glm::vec3 m_cameraPos   { glm::vec3(0.0f, 0.0f, 3.0f)};
    glm::vec3 m_cameraFront { glm::vec3(0.0f, 0.0f, -1.0f)};
    glm::vec3 m_cameraUp    { glm::vec3(0.0f, 1.0f, 0.0f)};

    int m_width {WINDOW_WIDTH};
    int m_height {WINDOW_HEIGHT};
};

#endif // __CONTEXT_H__