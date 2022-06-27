#ifndef __MESH_H__
#define __MESH_H__

#include "common.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "texture.h"
#include "program.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class Material;

class Mesh {

public:
    static std::unique_ptr<Mesh> Create(const std::vector<Vertex>& vertices,
        const std::vector<uint32_t>& indices, uint32_t primitiveType);

    static std::unique_ptr<Mesh> MakeBox();

    void Draw(const Program* program) const;

    const VertexLayout* GetVertexLayout() const {return m_vertexLayout.get();}

    std::shared_ptr<Buffer> GetVertexBuffer() const {return m_vertexBuffer;}
    std::shared_ptr<Buffer> GetIndexBuffer() const {return m_indexBuffer;}

    void SetMaterial(std::shared_ptr<Material> material){m_material = material;}
    std::shared_ptr<Material> GetMaterial() const { return m_material;}

private:
    Mesh(){}

    void Init(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
        uint32_t primitiveType);

    uint32_t m_primitiveType {GL_TRIANGLES};
    std::unique_ptr<VertexLayout> m_vertexLayout;
    std::shared_ptr<Buffer> m_vertexBuffer;
    std::shared_ptr<Buffer> m_indexBuffer;

    //여러개의 매쉬에서 하나의 재질을 사용할 수 있음
    std::shared_ptr<Material> m_material;
};


class Material{

public:
    static std::unique_ptr<Material> Create() {
        return std::unique_ptr<Material>(new Material());
    }

    std::shared_ptr<Texture> diffuse;
    std::shared_ptr<Texture> specular;
    float shininess{32.0f};

    void SetToProgram(const Program* program) const;

private:
    Material(){}

};



#endif