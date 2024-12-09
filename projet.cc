// 375390	Alexander Aoun  50%
// 371946	Ismail Mostafa  50%
#include "gui.h"
#include "simulation.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    Simulation sim;
    if (argc == 2)
    {
        auto app = Gtk::Application::create();  //calls gtkmm

        std::string path(argv[1]);
        sim.File_read_for_sim(path);            //calls file reading

        return app->make_window_and_run<MyArea>(1, argv, &sim); //start of gui and sim
    }

    else
    {
        auto app = Gtk::Application::create();
        return app->make_window_and_run<MyArea>(1, argv, &sim);
    }
}