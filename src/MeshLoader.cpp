#include "MeshLoader.hpp"
#include <iostream>
#include <set>

MeshLoader::MeshData MeshLoader::loadMesh(const std::string& filepath){
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath, aiProcess_JoinIdenticalVertices);

    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    {
        std::cerr << "Assimp Error :" << importer.GetErrorString()<< std::endl;
        return {};
    }

    MeshData data;
    aiMesh* mesh = scene->mMeshes[0];

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        data.vertices.emplace_back(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z);
    }

    std::set<std::pair<int,int>> unique_edges;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            int i1 = face.mIndices[j];
            int i2 = face.mIndices[(j+1) % face.mNumIndices];
            if (i1>i2) std::swap(i1,i2);
            
            unique_edges.insert({i1,i2});
        }
    }
    
    for (auto const& edge : unique_edges)
    {
        data.edges.push_back(edge);
    }
    
    return data;
}