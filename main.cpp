#include <iostream>
#include <Eigen/Dense>
#include "CameraModel.hpp"
int main(int argc, char const *argv[])
{
    // Camera Setup
    double focal_length = 500.0;
    CameraModel myCam(focal_length,focal_length,320.0,240.0);

    // Camera Pose Definition
    Eigen::Matrix3d R = Eigen::Matrix3d::Identity();
    Eigen::Vector3d t = Eigen::Vector3d::Zero();

    // Point in space
    Eigen::Vector3d worldPoint(0.5,0.5,2.0);
    
    // Projection process
    Eigen::Vector2d pixel = myCam.project(worldPoint,R,t);

    std::cout << "3D point:" << worldPoint.transpose() << std::endl;
    std::cout << "Pixel position:" << pixel.transpose() << std::endl;


    return 0;
}
