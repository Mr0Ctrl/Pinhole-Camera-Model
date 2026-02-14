#include "Renderer.hpp"

cv::Scalar Renderer::calculateShading(const Eigen::Vector3d& normal, const cv::Scalar& base_color) {
    // Lambertian Shading: Dot product (N . L)
    // Işık yönü kameradan objeye doğru olduğu için tersini alıyoruz (-light_dir_)
    double diffuse = normal.dot(-light_dir_);
    
    // Işığı 0 ile 1 arasına kısıtla ve biraz Ambient (0.1) ekle ki gölgeler zifiri olmasın
    double intensity = std::max(0.1, diffuse);
    
    return cv::Scalar(
        base_color[0] * intensity,
        base_color[1] * intensity,
        base_color[2] * intensity
    );
}
//performance hell
void Renderer::render(const MeshLoader::MeshData& mesh, CameraModel& cam, Visualizer& vis) {
    // A. Tüm noktaları tek seferde projekte et (Döngü dışında bir kez)
    struct ProjectedVertex {
        Eigen::Vector2d pixel;
        double depth;
        bool is_visible;
    };
    std::vector<ProjectedVertex> projected_vertices(mesh.vertices.size());
    for(size_t i = 0; i < mesh.vertices.size(); ++i) {
        auto res = cam.project(mesh.vertices[i]);
        projected_vertices[i] = {res.pixel, res.depth, res.is_visible};
    }

    // B. Sadece Görünür Yüzeyleri Seç ve Sırala
    // (Back-face culling'i sıralamadan ÖNCE yaparsak yük azalır)
    struct FaceToRender {
        const MeshLoader::Face* face;
        double depth;
    };
    std::vector<FaceToRender> render_queue;
    Eigen::Vector3d cam_pos = cam.getWorldPosition();

    for (const auto& face : mesh.faces) {
        // 1. Back-face Culling (Görünmeyeni baştan ele)
        Eigen::Vector3d v_pos = mesh.vertices[face.v1];
        if (face.normal.dot((v_pos - cam_pos).normalized()) >= 0) continue;

        // 2. Görünürlük Kontrolü
        if (!projected_vertices[face.v1].is_visible || 
            !projected_vertices[face.v2].is_visible || 
            !projected_vertices[face.v3].is_visible) continue;

        double d = (projected_vertices[face.v1].depth + 
                    projected_vertices[face.v2].depth + 
                    projected_vertices[face.v3].depth) / 3.0;
        render_queue.push_back({&face, d});
    }

    // C. Sıralama (Uzak olan arkada kalır)
    std::sort(render_queue.begin(), render_queue.end(), [](const FaceToRender& a, const FaceToRender& b) {
        return a.depth > b.depth;
    });

    // D. Çizim
    for (const auto& item : render_queue) {
        cv::Scalar color = calculateShading(item.face->normal, cv::Scalar(200, 150, 100));
        vis.renderTriangle(
            projected_vertices[item.face->v1].pixel,
            projected_vertices[item.face->v2].pixel,
            projected_vertices[item.face->v3].pixel,
            color
        );
    }
}