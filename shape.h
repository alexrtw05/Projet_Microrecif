// 375390	Alexander Aoun  50%
// 371946	Ismail Mostafa  50%

#ifndef SHAPE_H
#define SHAPE_H

#include "graphic.h"

#include <vector>

constexpr double epsil_zero(0.5);
constexpr double delta_rot(0.0625);

constexpr Colors red{1.0, 0.0, 0.0};
constexpr Colors green{0.0, 0.8, 0.0};
constexpr Colors blue{0.0, 0.2, 0.8};
constexpr Colors grey{0.8, 0.8, 0.8};

struct S2d
{
    double x = 0;
    double y = 0;

    S2d(double x_val = 0., double y_val = 0.) : x(x_val), y(y_val) {}
};

//superclass shape
class Shape
{
protected:
    S2d origin;
    Colors color;

public:
    Shape() = default;
    Shape(double x, double y, Colors c);
    void set_origin(S2d o);
    S2d get_origin();
};

//sub-class segment
class Segment : public Shape
{
private:
    int length = 0;
    double angle = 0.;
    S2d segment_ends;

public:
    Segment() = default;
    Segment(double l, double a, double x, double y, Colors c);
    S2d extr();
    double ecartA(Segment s2);
    S2d get_segment_ends();
    int get_length();
    double get_angle();
    void set_segment_ends(S2d ends);
    void set_length(int l);
    void set_angle(double a);

    void drawSeg();
};

//sub-class square
class Square : public Shape
{
private:
    double side_length;

public:
    Square(double l, double x, double y, Colors c);
    double get_side_length();
    void set_side_length(double l);
    void drawSq();
};

//sub-class circle
class Circle : public Shape
{
private:
    double radius;

public:
    Circle(double r, double x, double y, Colors c);
    double get_radius();
    void set_radius(double r);
    void drawCircle();
};

bool do_intersect(S2d p1, S2d q1, S2d p2, S2d q2);
bool superpos(Segment s1, Segment s2);

#endif