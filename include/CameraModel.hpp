#ifndef CAMERA_MODEL_HPP
#define CAMERA_MODEL_HPP

#include <Eigen/Dense>

class CameraModel
{
public:
    CameraModel(double fx, double fy, double cx, double cy);
    Eigen::Vector2d project(
        const Eigen::Vector3d& point3D,
        const Eigen::Matrix3d& R,
        const Eigen::Vector3d& t
    );
private:
    Eigen::Matrix3d K;
};

#endif
