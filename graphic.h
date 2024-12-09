// 375390	Alexander Aoun  50%
// 371946	Ismail Mostafa  50%

#ifndef GRAPHIC_H
#define GRAPHIC_H

struct Colors
{
    double R;
    double G;
    double B;
};

void graphic_draw_circle(double radius, int xc, int yc, Colors color);
void graphic_draw_segment(double startX, double startY, double endX, double endY, Colors color);
void graphic_draw_square(double side, int xc, int yc, Colors color);
void graphic_draw_square_segments(double side, int xc, int yc, Colors color);
void graphic_draw_border();

void graphic_erase();

#endif