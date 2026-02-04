#include <iostream>

#include <vector>
#include <cmath>
#include <Eigen/Dense>

#include "CameraModel.hpp"
#include "OpenCVVisualizer.hpp"



int main(int argc, char const *argv[])
{
    // Camera and Window setup
    double focal_length = 360.0;
    CameraModel cam(focal_length,focal_length,320.0,240.0);
    OpenCVVisualizer vis(640, 480);

    // Cube Definition
    std::vector<Eigen::Vector3d> cubeVertices = {
        {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},
        {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}
    };

    std::vector<std::pair<int, int>> edges = {
        {0,1}, {1,2}, {2,3}, {3,0},
        {4,5}, {5,6}, {6,7}, {7,4},  
        {0,4}, {1,5}, {2,6}, {3,7}   
    };
    
    double angle = 0.0;
    double radius = 2.0;

    while (true) {
        // 3. Kamerayı Küp Etrafında Döndür (Orbit)
        // Kameranın dünyadaki konumu (X-Z düzleminde daire çiziyor)
        double camX = radius * std::sin(angle);
        double camZ = radius * std::cos(angle);
        
        Eigen::Vector3d camera_pos_world(camX, 0, camZ);

        // 4. Rotasyon Matrisi (Kamerayı merkeze bakacak şekilde döndür)
        // Basitlik için sadece Y ekseninde döndürüyoruz
        Eigen::Matrix3d R_world_to_cam;
        R_world_to_cam = Eigen::AngleAxisd(-angle, Eigen::Vector3d::UnitY());

        // Dışsal Parametreler (Extrinsics):
        // P_cam = R * (P_world - camera_pos_world)
        // Bu yüzden t = -R * camera_pos_world olur.
        Eigen::Vector3d t = -R_world_to_cam * camera_pos_world;

        // 5. Çizim
        for (const auto& edge : edges) {
            Eigen::Vector2d p1 = cam.project(cubeVertices[edge.first], R_world_to_cam, t);
            Eigen::Vector2d p2 = cam.project(cubeVertices[edge.second], R_world_to_cam, t);
            vis.renderLine(p1, p2);
        }

        vis.show();

        angle += 0.03; 
        if (cv::waitKey(30) == 27) break;
    }

    return 0;
}
