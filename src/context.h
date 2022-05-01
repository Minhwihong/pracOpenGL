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

    //texture id 저장을 위한 변수 
    //uint32_t m_texture;
};

#endif // __CONTEXT_H__