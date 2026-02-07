#include <iostream>

#include <memory>
#include <vector>
#include <getopt.h>

#include "CameraModel.hpp"
#include "OpenCVVisualizer.hpp"
#include "MeshLoader.hpp"



int main(int argc, char** argv)
{
    std::string engine = "opencv";

    #pragma region Parameter Management
    
    
    static struct option long_options[] = {
        {"graphic-engine", required_argument, 0, 'g'},
        {"help",           no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    int c;

    while ((c = getopt_long(argc, argv, "g:h", long_options, &option_index)) != -1) {
        switch (c) {
            case 'g':
                engine = optarg;
                break;
            case 'h':
                std::cout << "Kullanim: ./camera_sim --graphic-engine [opencv/vulkan]\n";
                return 0;
            default:
                break;
        }
    }

    #pragma endregion

    std::unique_ptr<Visualizer> vis;

    if (engine == "opencv"){
        vis = std::make_unique<OpenCVVisualizer>(800,450);
        std::cout << "Graphic Engine: OpenCV" << std::endl; 
    }
    else{
        vis = std::make_unique<OpenCVVisualizer>(800,450);
        std::cout << "Graphic Engine: OpenCV" << std::endl; 
    }

    // Camera and Window setup
    double focal_length = 320.0;
    CameraModel cam(focal_length,focal_length,400.0,225.0);

    // 3D Model Load
    MeshLoader::MeshData mesh = MeshLoader::loadMesh("susane.obj");

    if (mesh.vertices.empty()) {
        std::cerr << "Model Load Error" << std::endl;
        return 1;
    }

    double angle = 0.0;
    double radius = 2.5;

    while (true) {
        
        //Camera Circular Rotation
        Eigen::Vector3d camera_pos_world(
            radius * std::cos(angle), 
            radius * std::sin(angle), 
            0.0 
        );

        //Camera Look At
        Eigen::Vector3d target(0, 0, 0);
        
        Eigen::Vector3d world_up(0, 0, 1);

        // Camera Look Vector
        Eigen::Vector3d cForward = (target - camera_pos_world).normalized();
        
        Eigen::Vector3d cRight = world_up.cross(cForward).normalized();
        Eigen::Vector3d cUpward = cRight.cross(cForward).normalized();

        // World Camera Rotation Matrix
        Eigen::Matrix3d R_world_to_cam;
        R_world_to_cam.row(0) = cRight; 
        R_world_to_cam.row(1) = cUpward; 
        R_world_to_cam.row(2) = cForward;

        Eigen::Vector3d view_translation = -R_world_to_cam * camera_pos_world;

        // Drawing Loop
        for (const auto& edge : mesh.edges) {
            Eigen::Vector2d p1 = cam.project(mesh.vertices[edge.first], R_world_to_cam, view_translation);
            Eigen::Vector2d p2 = cam.project(mesh.vertices[edge.second], R_world_to_cam, view_translation);
            
            vis->renderLine(p1, p2);
        }

        //Rendering
        vis->show();

        //Animation
        angle += 0.1; // Animasyon hızı
        if (cv::waitKey(30) == 27) break; // ESC ile çıkış
    }

    return 0;
}
