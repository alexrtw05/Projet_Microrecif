// 375390	Alexander Aoun  50%
// 371946	Ismail Mostafa  50%

#include "graphic.h"
#include "graphic_gui.h"

#include <iostream>

static const Cairo::RefPtr<Cairo::Context> *ptcr(nullptr);

void graphic_set_context(const Cairo::RefPtr<Cairo::Context> &cr)
{
    ptcr = &cr;
}

//draw circle function
void graphic_draw_circle(double radius, int xc, int yc, Colors color)
{
    (*ptcr)->arc(xc, yc, radius, 0.0, 2.0 * M_PI); // full circle
    (*ptcr)->set_source_rgb(color.R, color.G, color.B);
    (*ptcr)->stroke();
}

//draw segment function
void graphic_draw_segment(double startX, double startY, double endX, double endY, Colors color)
{
    (*ptcr)->set_line_width(1);
    (*ptcr)->set_source_rgb(color.R, color.G, color.B);

    (*ptcr)->move_to(startX, startY);
    (*ptcr)->line_to(endX, endY);
    (*ptcr)->stroke();
}

//draw square for segment base function
void graphic_draw_square_segments(double side, int xc, int yc, Colors color)
{
    (*ptcr)->set_line_width(1.0);
    (*ptcr)->set_source_rgb(color.R, color.G, color.B);
    (*ptcr)->rectangle(xc - (side / 2), yc - (side / 2), side, side);
    (*ptcr)->stroke();

}

//draw square for gui function
void graphic_draw_square(double side, int xc, int yc, Colors color)
{
    (*ptcr)->set_line_width(1.0);
    (*ptcr)->set_source_rgb(color.R, color.G, color.B);
    (*ptcr)->rectangle(xc , yc , side, side);
    (*ptcr)->stroke();
}

//draw border for gui function
void graphic_draw_border()
{
    (*ptcr)->set_line_width(1.0);
    (*ptcr)->set_source_rgb(0.5, 0.5, 0.5);

    (*ptcr)->move_to(0, 500);
    (*ptcr)->line_to(500, 500);
    (*ptcr)->move_to(500, 500);
    (*ptcr)->line_to(500, 0);
    (*ptcr)->move_to(500, 0);
    (*ptcr)->line_to(0, 0);
    (*ptcr)->move_to(0, 0);
    (*ptcr)->line_to(0, 500);
    (*ptcr)->stroke();
}

//erase all
void graphic_erase()
{
    (*ptcr)->set_source_rgb(1, 1, 1);
    (*ptcr)->paint();
    graphic_draw_border();
}


