#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "texture.h"
#include "mesh.h"
#include "model.h"


struct Light {
    glm::vec3 direction { glm::vec3(-1.0f, -1.0f, -1.0f)};
    glm::vec2 cutoff { glm::vec2(120.0f, 5.0f)};

    glm::vec3 position { glm::vec3(1.0f, 4.0f, 4.0f)};
    float distance{128.0f};
    
    glm::vec3 ambient {  glm::vec3(0.1f, 0.1f, 0.1f)};
    glm::vec3 diffuse {  glm::vec3(0.5f, 0.5f, 0.5f)};
    glm::vec3 specular { glm::vec3(1.0f, 1.0f, 1.0f)};
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

    // struct Material{

    //     //glm::vec3 specular { glm::vec3(0.5f, 0.5f, 0.5f)};
    //     float shininess { 32.0f};

    //     std::shared_ptr<Texture> diffuse;
    //     std::shared_ptr<Texture> specular;
    // };
    
    std::unique_ptr<Program>  m_program;
    std::unique_ptr<Program> m_simpleProgram;

    std::unique_ptr<Mesh> m_box;
    std::unique_ptr<Model> m_model;

    std::unique_ptr<Texture> m_texture;
    std::unique_ptr<Texture> m_texture2;

    glm::vec4 m_clearColor {glm::vec4(0.1f, 0.2f, 0.3f, 0.0f)};


    // animation (for testing)
    bool m_animation { true };

    bool m_flashLightMode{false};

    // light param
    Light m_light;
    //Material m_material;
    
    std::unique_ptr<Material> m_planeMaterial;
    std::unique_ptr<Material> m_box1Material;
    std::unique_ptr<Material> m_box2Material;


    // camera param
    bool m_cameraControl {false};
    glm::vec2 m_prevMousePos {glm::vec2(0.0f)};
    float m_cameraPitch {-20.0f};
    float m_cameraYaw {0.0f};
    glm::vec3 m_cameraPos   { glm::vec3(0.0f, 2.5f, 8.0f)};
    glm::vec3 m_cameraFront { glm::vec3(0.0f, 0.0f, -1.0f)};
    glm::vec3 m_cameraUp    { glm::vec3(0.0f, 1.0f, 0.0f)};

    int m_width {WINDOW_WIDTH};
    int m_height {WINDOW_HEIGHT};
};

#endif // __CONTEXT_H__