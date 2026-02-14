#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "MeshLoader.hpp"
#include "CameraModel.hpp"
#include "Visualizer.hpp"
#include <Eigen/Dense>
#include <vector>
#include <algorithm>

class Renderer {
public:
    Renderer() : light_dir_(Eigen::Vector3d(0.5, 0.5, -1.0).normalized()) {}

    // Ana render fonksiyonu
    void render(const MeshLoader::MeshData& mesh, 
                CameraModel& cam, 
                Visualizer& vis);

    // Işık yönünü güncellemek için
    void setLightDir(const Eigen::Vector3d& dir) { light_dir_ = dir.normalized(); }

private:
    Eigen::Vector3d light_dir_;
    
    // Basit bir Ambient + Diffuse (Lambertian) shading hesabı
    cv::Scalar calculateShading(const Eigen::Vector3d& normal, const cv::Scalar& base_color);
};

#endif