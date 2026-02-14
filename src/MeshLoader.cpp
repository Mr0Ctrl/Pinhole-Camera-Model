#include "MeshLoader.hpp"
#include <iostream>
#include <set>

MeshLoader::MeshData MeshLoader::loadMesh(const std::string& filepath){
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

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

    // Read Faces and Edges
    std::set<std::pair<int,int>> unique_edges;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            
            // Sadece 3 indeksi olan (üçgen) yüzeyleri alıyoruz (Triangulate sayesinde garanti)
            if (face.mNumIndices == 3) {
                Face f;
                f.v1 = face.mIndices[0];
                f.v2 = face.mIndices[1];
                f.v3 = face.mIndices[2];

                // Normal hesapla ve ata
                f.normal = calculateNormal(data.vertices[f.v1], 
                                        data.vertices[f.v2], 
                                        data.vertices[f.v3]);
                
                data.faces.push_back(f);

                // Tel kafes (wireframe) için edge bilgilerini de doldurmaya devam et
                for (unsigned int j = 0; j < 3; j++) {
                    int i1 = face.mIndices[j];
                    int i2 = face.mIndices[(j + 1) % 3];
                    if (i1 > i2) std::swap(i1, i2);
                    unique_edges.insert({i1, i2});
                }
            }
        }
    }
    
    for (auto const& edge : unique_edges)
    {
        data.edges.push_back(edge);
    }
    
    return data;
}

Eigen::Vector3d MeshLoader::calculateNormal(const Eigen::Vector3d& v1, 
                                 const Eigen::Vector3d& v2, 
                                 const Eigen::Vector3d& v3) {
    Eigen::Vector3d edge1 = v2 - v1;
    Eigen::Vector3d edge2 = v3 - v1;
    // Dış çarpım (Cross Product) normali verir
    return edge1.cross(edge2).normalized();
}