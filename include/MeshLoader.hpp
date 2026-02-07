#ifndef MESH_LOADER_HPP
#define MESH_LOADER_HPP

#include <Eigen/Dense>
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class MeshLoader
{
private:
    /* data */
public:
    struct MeshData
    {
        std::vector<Eigen::Vector3d> vertices;
        std::vector<std::pair<int,int>> edges;
    };
    
    static MeshData loadMesh(const std::string& filepath);
};

#endif
