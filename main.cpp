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
    Eigen::Vector3d target(0, 0, 0);

    while (true) {
        
        Eigen::Vector3d camera_pos_world(
        radius * std::cos(angle), 
        radius * std::sin(angle), 
        0.5 // Biraz yukarıdan bakması derinlik algısını artırır
        );

        // 2. Kamerayı Güncelle
        // Sınıfın içine taşıdığımız mantık sayesinde sadece konum verip "şuraya bak" diyoruz.
        cam.setWorldPosition(camera_pos_world);
        cam.lookAt(target); // Varsayılan olarak world_up (0,0,1) kullanır

        // 3. Çizim Döngüsü
        for (const auto& edge : mesh.edges) {
            // Projeksiyon artık R ve t'yi içeriden otomatik kullanıyor
            auto p1 = cam.project(mesh.vertices[edge.first]);
            auto p2 = cam.project(mesh.vertices[edge.second]);

            // Sadece her iki nokta da kameranın önündeyse çiz
            if (p1.is_visible && p2.is_visible) {
                // İleride buraya p1.depth kullanarak kalınlık ekleyebiliriz
                int thickness = static_cast<int>(50 / pow((p1.depth+p2.depth),2)); 
                if (thickness < 1) thickness = 1;

                vis->renderPoint(p1.pixel,thickness);
                vis->renderPoint(p2.pixel,thickness);
                vis->renderLine(p1.pixel, p2.pixel, thickness);
            }
        }

        // 4. Rendering ve Animasyon
        vis->show();

        angle += 0.02; // Daha akıcı bir dönüş için hızı biraz düşürdük
        if (cv::waitKey(30) == 27) break; 
    }

    return 0;
}
