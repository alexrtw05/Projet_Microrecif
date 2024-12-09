// 375390	Alexander Aoun  40%
// 371946	Ismail Mostafa  60%

#ifndef GTKMM_EXAMPLE_MYAREA_H
#define GTKMM_EXAMPLE_MYAREA_H

#include "graphic_gui.h"
#include "simulation.h"

#include <gtkmm/application.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/eventcontrollerkey.h>
#include <glibmm/main.h>
#include <gtkmm/frame.h>
#include <gtkmm/label.h>
#include <gtkmm/separator.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/widget.h>
#include <gtkmm/window.h>

//epsil_random helps algues spawn just before (256,256)
constexpr double epsil_random(0.001);

//structure to define frame
struct Frame
{
    double xMin;
    double xMax;
    double yMin;
    double yMax;
    double asp;
    int width;
    int height;
};

//Drawing area class
class DrawingArea : public Gtk::DrawingArea
{
public:
    DrawingArea();
    virtual ~DrawingArea();
    void setFrame(Frame x);
    void adjustFrame(int width, int height);

private:
    Frame frame;

protected:
    void on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height);
};

//myArea class
class MyArea : public Gtk::Window
{
public:
    MyArea(Simulation *sim);
    virtual ~MyArea();
    bool on_timeout();
    void reset_counters();
    void init_sim();
    static Simulation access_sim();

private:
    Simulation *sim_;
    static std::default_random_engine e;

protected:
    void on_button_clicked_Step();
    void on_button_clicked_Start();
    void on_button_clicked_Save();
    void on_button_clicked_Open();
    void on_button_clicked_Exit();

    bool on_window_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);
    void on_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog);
    void on_save_dialog_response(int response_id, Gtk::FileChooserDialog *dialog);
    void on_window_resized(Gtk::Window &allocation);

    Gtk::Box m_Main_Box;
    Gtk::Box m_Side_Box;
    Gtk::Box m_Buttons_Box;
    Gtk::Box m_Drawing_Box;
    Gtk::Box m_Info_Box;
    Gtk::Box m_maj;
    Gtk::Box m_Alg;
    Gtk::Box m_Cor;
    Gtk::Box m_Scav;

    Gtk::Button m_Button_Step;
    Gtk::ToggleButton m_Button_Start;
    Gtk::Button m_Button_Save;
    Gtk::Button m_Button_Open;
    Gtk::Button m_Button_Exit;
    Gtk::CheckButton m_Check_Button;

    Gtk::Label m_General_Label;
    Gtk::Label m_Info_Label;
    Gtk::Label m_maj_Label;
    Gtk::Label m_Alg_Label;
    Gtk::Label m_Cor_Label;
    Gtk::Label m_Scav_Label;

    Gtk::Label m_maj_Count;
    Gtk::Label m_Alg_Count;
    Gtk::Label m_Cor_Count;
    Gtk::Label m_Scav_Count;

    Gtk::Frame m_Button_Frame;
    Gtk::Frame m_Info_Frame;

    DrawingArea m_Draw;

    int maj_count;
    int algue_count;
    int coral_count;
    int scav_count;

    sigc::connection m_Connection;

    void update_gui_algue();
};

#endif 
