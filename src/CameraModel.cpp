#include "CameraModel.hpp"

CameraModel::CameraModel(double fx, double fy, double cx, double cy)
    : fx_(fx), fy_(fy), cx_(cx), cy_(cy) {
    
    camera_position_ = Eigen::Vector3d::Zero();
    camera_rotation_ = Eigen::Matrix3d::Identity();
    
    updateKMatrix();
    updateExtrinsics();
}

void CameraModel::updateKMatrix() {
    K_matrix_ << fx_, 0,   cx_,
                 0,   fy_, cy_,
                 0,   0,   1;
}

void CameraModel::setFocalLength(double fx, double fy) {
    fx_ = fx;
    fy_ = fy;
    updateKMatrix();
}

void CameraModel::setPrincipalPoint(double cx, double cy) {
    cx_ = cx;
    cy_ = cy;
    updateKMatrix();
}

// --- Extrinsics ---

void CameraModel::setWorldPosition(const Eigen::Vector3d& position) {
    camera_position_ = position;
    updateExtrinsics();
}

const Eigen::Vector3d& CameraModel::getWorldPosition() {
    return camera_position_;
}

void CameraModel::setWorldRotation(const Eigen::Matrix3d& rotation_matrix) {
    camera_rotation_ = rotation_matrix;
    updateExtrinsics();
}

void CameraModel::updateExtrinsics() {
    // 1. Dünyadan Kameraya Dönüş: Kameranın dünyadaki rotasyonunun evriğidir (transpose)
    rotation_world_to_view_ = camera_rotation_.transpose();
    
    // 2. Dünyadan Kameraya Öteleme: -R * C
    translation_world_to_view_ = -rotation_world_to_view_ * camera_position_;
}

// --- Viev Controls ---

void CameraModel::lookAt(const Eigen::Vector3d& target, const Eigen::Vector3d& world_up) {
    Eigen::Vector3d f = (target - camera_position_).normalized(); // Forward
    Eigen::Vector3d r = world_up.cross(f).normalized();           // Right
    Eigen::Vector3d u_cv = r.cross(f);                            // Down (OpenCV Up)

    // Kameranın dünyadaki yeni eksenlerini matrise sütun olarak diziyoruz
    camera_rotation_.col(0) = r;
    camera_rotation_.col(1) = u_cv;
    camera_rotation_.col(2) = f;

    updateExtrinsics();
}

void CameraModel::rotateRelative(double roll, double pitch, double yaw) {
    // Kameranın KENDİ eksenleri etrafında dönüşü (Local Rotation)
    Eigen::Matrix3d local_rotation;
    local_rotation = Eigen::AngleAxisd(yaw,   Eigen::Vector3d::UnitY()) *
                     Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitX()) *
                     Eigen::AngleAxisd(roll,  Eigen::Vector3d::UnitZ());

    // Mevcut dünya rotasyonunu lokal dönüşle güncelle
    camera_rotation_ = camera_rotation_ * local_rotation;

    updateExtrinsics();
}

// --- Projeksiyon ---

CameraModel::ProjectionResult CameraModel::project(const Eigen::Vector3d& world_point) const {
    // 1. Noktayı Kamera Koordinat Sistemine Al: P_c = R*P_w + t
    Eigen::Vector3d p_camera = rotation_world_to_view_ * world_point + translation_world_to_view_;

    ProjectionResult res;
    res.depth = p_camera.z(); // Derinlik bilgisi (Kamera Z ekseni)

    // 2. Near-Clipping
    if (res.depth < 0.1) {
        res.is_visible = false;
        return res;
    }

    // 3. İzdüşüm: u = fx*(X/Z) + cx ...
    // K_matrix_ kullanarak yapmak istersen: 
    // Eigen::Vector3d p_pixel = K_matrix_ * (p_camera / p_camera.z());
    res.pixel.x() = fx_ * (p_camera.x() / p_camera.z()) + cx_;
    res.pixel.y() = fy_ * (p_camera.y() / p_camera.z()) + cy_;
    res.is_visible = true;

    return res;
}