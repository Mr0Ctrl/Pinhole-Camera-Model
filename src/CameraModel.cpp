#include "CameraModel.hpp"

CameraModel::CameraModel(
    double fx,
    double fy,
    double cx,
    double cy
){
    K.setZero();
    K(0,0) = fx;
    K(1,1) = fy;
    K(0,2) = cx;
    K(1,2) = cy;
    K(2,2) = 1.0;
}

Eigen::Vector2d CameraModel::project(
    const Eigen::Vector3d& point3D,
    const Eigen::Matrix3d& R,
    const Eigen::Vector3d& t
){
    //Global cordinate to Camera Cordinate
    Eigen::Vector3d point_cam = R*point3D+t;

    //Homogenous Cordinate Normalization
    double x_norm = point_cam.x() / point_cam.z();
    double y_norm = point_cam.y() / point_cam.z();

    Eigen::Vector3d pixel_homo = K * Eigen::Vector3d(x_norm,y_norm,1.0);

    return Eigen::Vector2d(pixel_homo.x(),pixel_homo.y());
}