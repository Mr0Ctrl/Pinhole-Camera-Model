#include "Visualizer.hpp"
#include <opencv2/opencv.hpp>

class OpenCVVisualizer : public Visualizer
{
private:
    cv::Mat canvas;
    std::string window_name;
public:
    OpenCVVisualizer(int with,int height){
        canvas = cv::Mat::zeros(height,with,CV_8UC3);
        window_name = "Pinhole Camera Similation";
    }

    void renderPoint(const Eigen::Vector2d& pixel) override {
        cv::circle(canvas,cv::Point(pixel.x(),pixel.y()),3,cv::Scalar(0,255,0),-1);
    }
    
    
    void renderLine( const Eigen::Vector2d& p1, const Eigen::Vector2d& p2) override {
        cv::line(canvas, cv::Point(p1.x(),p1.y()),cv::Point(p2.x(),p2.y()),cv::Scalar(0,255,0),1);

    }

    void show() override {
        cv::imshow(window_name, canvas);
        cv::waitKey(1); // 1ms bekle (animasyon için kritik)
        canvas = cv::Scalar(0, 0, 0); // Ekranı bir sonraki kare için temizle
    }
};