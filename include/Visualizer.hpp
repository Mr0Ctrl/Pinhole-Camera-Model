#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include <Eigen/Dense>
#include <vector>

class Visualizer
{
public:
    virtual ~Visualizer() = default;
    
    virtual void renderPoint(const Eigen::Vector2d& pixel) = 0;
    virtual void renderLine(
        const Eigen::Vector2d& p1,
        const Eigen::Vector2d& p2
    ) = 0;
    virtual void show() = 0;
};

#endif