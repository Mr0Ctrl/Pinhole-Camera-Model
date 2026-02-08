#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include <Eigen/Dense>
#include <vector>

class Visualizer {
public:
    struct MouseState {
        int x = 0, y = 0;       
        int dx = 0, dy = 0;     
        int wheel = 0;
        bool left_button = false;
        bool middle_button = false;
        bool right_button = false;
    };

    virtual ~Visualizer() = default;

    virtual void renderPoint(const Eigen::Vector2d& pixel, int thickness) = 0;
    virtual void renderLine(const Eigen::Vector2d& p1, const Eigen::Vector2d& p2, int thickness) = 0;
    
    virtual void clear() = 0;
    virtual void show() = 0;

    virtual MouseState getMouseState() = 0;
};



#endif