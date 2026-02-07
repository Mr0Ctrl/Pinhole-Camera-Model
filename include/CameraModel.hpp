#ifndef CAMERA_MODEL_HPP
#define CAMERA_MODEL_HPP

#include <Eigen/Dense>

class CameraModel {
public:
    CameraModel(double fx, double fy, double cx, double cy);

    // --- Intrinsics (Mercek) Güncelleyiciler ---
    void setFocalLength(double fx, double fy);
    void setPrincipalPoint(double cx, double cy);

    // --- Extrinsics (Duruş) Setters ---
    void setWorldPosition(const Eigen::Vector3d& position);
    void setWorldRotation(const Eigen::Matrix3d& rotation_matrix);

    // --- Bakış Kontrolü ---
    void lookAt(const Eigen::Vector3d& target, const Eigen::Vector3d& world_up = Eigen::Vector3d(0, 0, 1));
    void rotateRelative(double roll, double pitch, double yaw);

    // --- Çıktı Yapısı ---
    struct ProjectionResult {
        Eigen::Vector2d pixel;
        double depth;
        bool is_visible;
    };

    // Neden parametreleri azalttık? 
    // Çünkü artık R ve t sınıfın içinde 'rotation_world_to_view_' 
    // ve 'translation_world_to_view_' olarak güncel tutuluyor.
    ProjectionResult project(const Eigen::Vector3d& world_point) const;

private:
    // Mercek Parametreleri
    double fx_, fy_, cx_, cy_;
    Eigen::Matrix3d K_matrix_; // fx, fy, cx, cy değiştikçe bu da güncellenmeli

    // Kameranın Dünyadaki Konumu ve Yönü
    Eigen::Vector3d camera_position_;
    Eigen::Matrix3d camera_rotation_; 

    // Önbelleğe Alınmış (Cached) View Matrisleri
    Eigen::Matrix3d rotation_world_to_view_;
    Eigen::Vector3d translation_world_to_view_;
    
    void updateKMatrix();
    void updateExtrinsics();
};

#endif