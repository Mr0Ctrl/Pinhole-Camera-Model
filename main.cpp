#include <iostream>

#include <memory>
#include <vector>
#include <getopt.h>

#include "CameraModel.hpp"
#include "OpenCVVisualizer.hpp"
#include "MeshLoader.hpp"
#include "OrbitController.hpp"
#include "Renderer.hpp"



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
        vis = std::make_unique<OpenCVVisualizer>(1024,720);
        std::cout << "Graphic Engine: OpenCV" << std::endl; 
    }
    else{
        vis = std::make_unique<OpenCVVisualizer>(1024,720);
        std::cout << "Graphic Engine: OpenCV" << std::endl; 
    }

    // Camera and Window setup
    double focal_length = 420.0;
    CameraModel cam(focal_length,focal_length,512,360.0);

    // 3D Model Load
    MeshLoader::MeshData mesh = MeshLoader::loadMesh("susane.obj");

    if (mesh.vertices.empty()) {
        std::cerr << "Model Load Error" << std::endl;
        return 1;
    }

    Renderer renderer;
    OrbitController controller(2, 0.0, 0.5);

    while (true) {
        auto mouse = vis->getMouseState();

        controller.update(cam, mouse);
        
        
        vis->clear();
        renderer.render(mesh, cam, *vis);
        vis->show();
        if (cv::waitKey(30) == 27) break; 
    }

    return 0;
}
