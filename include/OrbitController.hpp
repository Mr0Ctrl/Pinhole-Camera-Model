    #ifndef ORBIT_CONTROLLER_HPP
#define ORBIT_CONTROLLER_HPP

#include "CameraModel.hpp"
#include "Visualizer.hpp"
#include <cmath>
#include <algorithm>

class OrbitController {
public:
    OrbitController(double radius = 5.0, double yaw = 0.0, double pitch = 0.5)
        : radius_(radius), yaw_(yaw), pitch_(pitch), target_(0, 0, 0) {}

    void update(CameraModel& cam, const Visualizer::MouseState& mouse) {
        // Zoom (Tekerlek)
        if (mouse.wheel != 0) {
            radius_ -= mouse.wheel * 0.001; 
            radius_ = std::max(0.5, radius_);
        }

        // Orbit (Orta Tuş)
        if (mouse.middle_button) {
            yaw_ += mouse.dx * 0.005;
            pitch_ += mouse.dy * 0.005;

            const double limit = M_PI / 2.0 - 0.01;
            pitch_ = std::clamp(pitch_, -limit, limit);
        }

        // Küre koordinatları -> Dünya koordinatları
        double x = radius_ * std::cos(pitch_) * std::cos(yaw_);
        double y = radius_ * std::cos(pitch_) * std::sin(yaw_);
        double z = radius_ * std::sin(pitch_);

        cam.setWorldPosition(Eigen::Vector3d(x, y, z));
        cam.lookAt(target_);
    }

    void setTarget(const Eigen::Vector3d& target) { target_ = target; }

private:
    double radius_, yaw_, pitch_;
    Eigen::Vector3d target_;
};

#endif