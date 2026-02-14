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


    static Eigen::Vector3d calculateNormal(const Eigen::Vector3d& v1, 
                                           const Eigen::Vector3d& v2, 
                                           const Eigen::Vector3d& v3);
    
public:
    struct Face {
        int v1, v2, v3;
        Eigen::Vector3d normal;  
    };

    struct MeshData
    {
        std::vector<Eigen::Vector3d> vertices;
        std::vector<std::pair<int,int>> edges;
        std::vector<Face> faces;
    };
    
    static MeshData loadMesh(const std::string& filepath);
};

#endif
