#ifndef __HESH_H__
#define __HESH_H__

#include "common.h"
#include "buffer.h"
#include "vertex_layout.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class Mesh {

public:
    static std::unique_ptr<Mesh> Create(const std::vector<Vertex>& vertices,
        const std::vector<uint32_t>& indices, uint32_t primitiveType);

    static std::unique_ptr<Mesh> MakeBox();

    void Draw() const;

    const VertexLayout* GetVertexLayout() const {return m_vertexLayout.get();}

    std::shared_ptr<Buffer> GetVertexBuffer() const {return m_vertexBuffer;}
    std::shared_ptr<Buffer> GetIndexBuffer() const {return m_indexBuffer;}

private:
    Mesh(){}

    void Init(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
        uint32_t primitiveType);

    uint32_t m_primitiveType {GL_TRIANGLES};
    std::unique_ptr<VertexLayout> m_vertexLayout;
    std::shared_ptr<Buffer> m_vertexBuffer;
    std::shared_ptr<Buffer> m_indexBuffer;
};


#endif