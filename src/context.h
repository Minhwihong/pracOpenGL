#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "texture.h"

CLASS_PTR(Context)
class Context {
public:
    //static ContextUPtr Create();
    static std::unique_ptr<Context> Create();
    void Render();  
    void ProcessInput(GLFWwindow* window);
    //std::unique_ptr<int> x;  
private:
    Context() {}
    bool Init();
    
    //ProgramUPtr m_program;
    //VertexLayoutUPtr m_vertexLayout;
    
    std::unique_ptr<Program>  m_program;
    std::unique_ptr<VertexLayout> m_vertexLayout;

    std::unique_ptr<Buffer> m_vertexBuffer;
    std::unique_ptr<Buffer> m_indexBuffer;

    std::unique_ptr<Texture> m_texture;
    std::unique_ptr<Texture> m_texture2;

    // camera param
    glm::vec3 m_cameraPos   { glm::vec3(0.0f, 0.0f, 3.0f)};
    glm::vec3 m_cameraFront { glm::vec3(0.0f, 0.0f, -1.0f)};
    glm::vec3 m_cameraUp    { glm::vec3(0.0f, 1.0f, 0.0f)};
};

#endif // __CONTEXT_H__