#ifndef __MODEL_H__
#define __MODEL_H__

#include "common.h"
#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {

public:
    static std::unique_ptr<Model> Load(const std::string& filename);

    int GetMeshCount() const { return (int)m_meshes.size();}
    std::shared_ptr<Mesh> GetMesh(int index) const {return m_meshes[index];}
    void Draw() const;

private:
    Model() {}

    bool LoadByAssimp(const std::string& filename);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene);
    void ProcessNode(aiNode* node, const aiScene* scene);

    std::vector<std::shared_ptr<Mesh>> m_meshes;

};



#endif