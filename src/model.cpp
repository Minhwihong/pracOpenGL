#include "model.h"

std::unique_ptr<Model> Model::Load(const std::string& filename){

    auto model = std::unique_ptr<Model>(new Model());

    if(!model->LoadByAssimp(filename)){
        return nullptr;
    }

    return std::move(model);

}

bool Model::LoadByAssimp(const std::string& filename){

    Assimp::Importer importer;

    auto scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs );

    if(!scene){
        SPDLOG_ERROR("failed to load model: {}", filename);
        return false;
    }

    if(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){

        SPDLOG_ERROR("AI_SCENE_FLAGS_INCOMPLETE");
        return false;
    }

    if(!scene->mRootNode){

        SPDLOG_ERROR("mRootNode not exist");
        return false;
    }

    ProcessNode(scene->mRootNode, scene);
    return true;
}


void Model::ProcessNode(aiNode* node, const aiScene* scene){

    for(uint32_t idx=0; idx < node->mNumMeshes; ++idx){

        auto meshIndex = node->mMeshes[idx];
        auto mesh = scene->mMeshes[meshIndex];
        ProcessMesh(mesh, scene);
    }

    for(uint32_t idx=0; idx < node->mNumChildren; ++idx){

        ProcessNode(node->mChildren[idx], scene);
    }
}


void Model::ProcessMesh(aiMesh* mesh, const aiScene* scene){

    SPDLOG_INFO("process mesh: {}, #vert: {}, #face: {}",
        mesh->mName.C_Str(), mesh->mNumVertices, mesh->mNumFaces);

    std::vector<Vertex> vertices;

    vertices.resize(mesh->mNumVertices);

    for(u32 idx=0; idx < mesh->mNumVertices; ++idx){

        auto& v = vertices[idx];

        v.position = glm::vec3(mesh->mVertices[idx].x, 
            mesh->mVertices[idx].y, 
            mesh->mVertices[idx].z);
        
        v.normal = glm::vec3(mesh->mNormals[idx].x,
            mesh->mNormals[idx].y, 
            mesh->mNormals[idx].z);

        v.texCoord = glm::vec2(mesh->mTextureCoords[0][idx].x, mesh->mTextureCoords[0][idx].y);


    }

    std::vector<u32> indices;

    indices.resize(mesh->mNumFaces * 3);

    for(u32 idx=0; idx < mesh->mNumFaces; ++idx){

        indices[3*idx + 0] = mesh->mFaces[idx].mIndices[0];
        indices[3*idx + 1] = mesh->mFaces[idx].mIndices[1];
        indices[3*idx + 2] = mesh->mFaces[idx].mIndices[2];
    }

    auto glMesh = Mesh::Create(vertices, indices, GL_TRIANGLES);

    m_meshes.push_back(std::move(glMesh));

    
}


void Model::Draw() const {

    for(auto& mesh : m_meshes){
        mesh->Draw();
    }
}