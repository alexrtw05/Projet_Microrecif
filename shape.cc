// 375390	Alexander Aoun  50%
// 371946	Ismail Mostafa  50%

#include "shape.h"

#include <cmath>
#include <iostream>

using namespace std;

//superclass constructor initialises position and color
Shape::Shape(double x, double y, Colors c)
{
    origin.x = x;
    origin.y = y;
    color = c;
}

//circle contructor
Circle::Circle(double r, double x, double y, Colors c) : Shape(x, y, c), radius(r) {}

//segment constructor
Segment::Segment(double l, double a, double x, double y, Colors c)
    : Shape(x, y, c), length(l), angle(a)
{
}

//square constructor
Square::Square(double l, double x, double y,Colors c):Shape(x, y, c),side_length(l) {}

// angular deviation calculation
double Segment::ecartA(Segment s2)
{
    double angle_ = M_PI - s2.angle + angle;

    if (angle_ < -M_PI)
    {
        angle_ += 2 * M_PI;
    }

    if (angle_ > M_PI)
    {
        angle_ -= 2 * M_PI;
    }

    return angle_;
}

// extremity calculation
S2d Segment::extr()
{
    double extr_x = origin.x + length * cos(angle);
    double extr_y = origin.y + length * sin(angle);

    S2d extr(extr_x, extr_y);

    return extr;
}

// superposition checker
bool superpos(Segment s1, Segment s2)
{

    if (fabs(s1.ecartA(s2)) <= delta_rot) // absolut value
    {
        return true;
    }

    if (s1.ecartA(s2) == 0.)
    {
        return true;
    }

    else
    {
        return false;
    }
}

bool on_segment(S2d p, S2d q, S2d r)
{
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && q.y <= max(p.y, r.y) &&
        q.y >= min(p.y, r.y))
    {
        return true;
    }

    return false;
}

// calculates orientation   (already given)
int orientation(S2d p, S2d q, S2d r)
{
    double temp = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

    if (fabs(temp) < epsil_zero)
        return 0;

    if (temp == 0)
        return 0;

    return (temp > 0) ? 1 : 2;
}

// checks intersection  (already given)
bool do_intersect(S2d p1, S2d q1, S2d p2, S2d q2)
{
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4)
        return true;
    if (o1 == 0 && on_segment(p1, p2, q1))
        return true;
    if (o2 == 0 && on_segment(p1, q2, q1))
        return true;
    if (o3 == 0 && on_segment(p2, p1, q2))
        return true;
    if (o4 == 0 && on_segment(p2, q1, q2))
        return true;

    return false;
}

//getters
S2d Shape::get_origin() { return origin; }

S2d Segment::get_segment_ends() { return segment_ends; }

int Segment::get_length() { return length; }

double Segment::get_angle() { return angle; }

double Square::get_side_length() { return side_length; }

double Circle::get_radius() { return radius; }

//setters
void Shape::set_origin(S2d o)
{
    origin.x = o.x;
    origin.y = o.y;
}

void Segment::set_segment_ends(S2d ends)
{
    segment_ends.x = ends.x;
    segment_ends.y = ends.y;
}

void Segment::set_length(int l) { length = l; }

void Segment::set_angle(double a) { angle = a; }

void Circle::set_radius(double r) { radius = r; }

void Square::set_side_length(double l) { side_length = l; }

//functions that call the drawing in graphic
void Segment::drawSeg()
{
    graphic_draw_segment(origin.x, origin.y, this->extr().x, this->extr().y, color);
}

void Circle::drawCircle() { graphic_draw_circle(radius, origin.x, origin.y, color); }

void Square::drawSq()
{
    graphic_draw_square_segments(side_length, origin.x, origin.y, color);
}
