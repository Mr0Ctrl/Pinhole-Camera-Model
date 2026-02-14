#include "Visualizer.hpp"
#include <opencv2/opencv.hpp>

class OpenCVVisualizer : public Visualizer
{
private:
    cv::Mat canvas;
    std::string window_name;
    
    MouseState current_state_; // Visualizer::MouseState kullanıyor
    int last_x_ = 0;
    int last_y_ = 0;

    static void onMouse(int event, int x, int y, int flags, void* userdata) {
        if (!userdata) return;
        auto* self = static_cast<OpenCVVisualizer*>(userdata);
        
        self->current_state_.x = x;
        self->current_state_.y = y;

        if (event == cv::EVENT_MBUTTONDOWN) self->current_state_.middle_button = true;
        if (event == cv::EVENT_MBUTTONUP)   self->current_state_.middle_button = false;
        if (event == cv::EVENT_LBUTTONDOWN) self->current_state_.left_button = true;
        if (event == cv::EVENT_LBUTTONUP)   self->current_state_.left_button = false;
        
        if (event == cv::EVENT_MOUSEWHEEL) {
            self->current_state_.wheel += cv::getMouseWheelDelta(flags);
        }
    }

public: // <-- PUBLIC ETİKETİ onMouse DIŞINDA OLMALI
    OpenCVVisualizer(int width, int height) {
        canvas = cv::Mat::zeros(height, width, CV_8UC3);
        window_name = "Pinhole Camera Simulation";
        cv::namedWindow(window_name);
        cv::setMouseCallback(window_name, onMouse, this);
    }

    void clear() override {
        canvas = cv::Scalar(0, 0, 0);
    }

    void renderPoint(const Eigen::Vector2d& pixel, int thickness) override {
        cv::circle(canvas, cv::Point(pixel.x(), pixel.y()), thickness, cv::Scalar(0, 0, 255), -1);
    }
    
    void renderLine(const Eigen::Vector2d& p1, const Eigen::Vector2d& p2, int thickness) override {
        cv::line(canvas, cv::Point(p1.x(), p1.y()), cv::Point(p2.x(), p2.y()), cv::Scalar(0, 255, 0), thickness);
    }

    void renderTriangle (const Eigen::Vector2d& p1, const Eigen::Vector2d& p2, const Eigen::Vector2d& p3, const cv::Scalar& color) override {
        cv::Point pts[3];
        pts[0] = cv::Point(p1.x(), p1.y());
        pts[1] = cv::Point(p2.x(), p2.y());
        pts[2] = cv::Point(p3.x(), p3.y());

        cv::fillConvexPoly(canvas, pts, 3, color);
    }

    void show() override {
        cv::imshow(window_name, canvas);
        cv::waitKey(1); 
        current_state_.wheel = 0; // mouse_state_ değil, current_state_ olmalı
    }

    MouseState getMouseState() override {
        current_state_.dx = current_state_.x - last_x_;
        current_state_.dy = current_state_.y - last_y_;
        
        last_x_ = current_state_.x;
        last_y_ = current_state_.y;

        return current_state_;
    }
};