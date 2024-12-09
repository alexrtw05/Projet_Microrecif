// 375390	Alexander Aoun  40%
// 371946	Ismail Mostafa  60%

#include "gui.h"
#include "graphic_gui.h"

#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

//used for random algue generation
std::default_random_engine MyArea::e(1);

static void orthographic_projection(const Cairo::RefPtr<Cairo::Context> &cr,
                                    const Frame &frame);

//initialising frame to these default values
static Frame default_frame = {0., 256., 0., 256., 1, 500, 500};

//constructor for drawing area
DrawingArea::DrawingArea()
{
    setFrame(default_frame);
    set_content_width(default_frame.width);
    set_content_height(default_frame.height);
    set_draw_func(sigc::mem_fun(*this, &DrawingArea::on_draw));
}

//destructor for drawing area
DrawingArea::~DrawingArea() {}

void DrawingArea::setFrame(Frame f)
{
    if ((f.xMin <= f.xMax) and (f.yMin <= f.yMax) and (f.height > 0))
    {
        f.asp = f.width / f.height;
        frame = f;
    }
    else
    {
        std::cout << "Incorrect Model framing or window parameters" << std::endl;
    }
}

void DrawingArea::adjustFrame(int width, int height)
{
    frame.width = width;
    frame.height = height;

    double new_aspect_ratio((double)width / height);
    if (new_aspect_ratio > default_frame.asp)
    {
        frame.yMax = default_frame.yMax;
        frame.yMin = default_frame.yMin;

        double delta(default_frame.xMax - default_frame.xMin);
        double mid((default_frame.xMax + default_frame.xMin) / 2);
        frame.xMax = mid + 0.5 * (new_aspect_ratio / default_frame.asp) * delta;
        frame.xMin = mid - 0.5 * (new_aspect_ratio / default_frame.asp) * delta;
    }
    else
    {
        frame.xMax = default_frame.xMax;
        frame.xMin = default_frame.xMin;

        double delta(default_frame.yMax - default_frame.yMin);
        double mid((default_frame.yMax + default_frame.yMin) / 2);
        frame.yMax = mid + 0.5 * (default_frame.asp / new_aspect_ratio) * delta;
        frame.yMin = mid - 0.5 * (default_frame.asp / new_aspect_ratio) * delta;
    }
}

MyArea::~MyArea() {}

//myArea constructor
MyArea::MyArea(Simulation *sim)
    : sim_(sim), m_Main_Box(Gtk::Orientation::HORIZONTAL, 10),
      m_Side_Box(Gtk::Orientation::VERTICAL, 5),
      m_Buttons_Box(Gtk::Orientation::VERTICAL, 2),
      m_Drawing_Box(Gtk::Orientation::HORIZONTAL, 5),
      m_Info_Box(Gtk::Orientation::VERTICAL, 5),
      m_maj(Gtk::Orientation::HORIZONTAL, 15), m_Alg(Gtk::Orientation::HORIZONTAL,15),
      m_Cor(Gtk::Orientation::HORIZONTAL, 15),
      m_Scav(Gtk::Orientation::HORIZONTAL, 15), m_Button_Step("step"),
      m_Button_Start("start"), m_Button_Save("save"), m_Button_Open("open"),
      m_Button_Exit("exit"), m_Check_Button("Naissance d'algue", false),
      m_General_Label("General"), m_Info_Label("Info: nombre de..."),
      m_maj_Label("mise à jour:"), m_Alg_Label("algues:"), m_Cor_Label("corails:"),
      m_Scav_Label("charognards:"), m_maj_Count(std::to_string(maj_count)),
      m_Alg_Count(std::to_string(sim_->get_Algues_storage().size())),
      m_Cor_Count(std::to_string(coral_count)),
      m_Scav_Count(std::to_string(scav_count)), m_Draw()
{
    if (sim_->no_failure())
    {
        init_sim();
    }
    else
    {
        sim_->reset_sim();
        reset_counters();
    }

    set_title("Microrécif");
    set_child(m_Main_Box);

    m_Main_Box.append(m_Side_Box);
    m_Main_Box.append(m_Drawing_Box);

    m_Drawing_Box.set_size_request(500, 500);
    m_Draw.set_expand(true);

    m_Drawing_Box.append(m_Draw);

    m_Button_Frame.set_child(m_Buttons_Box);
    m_Side_Box.append(m_Button_Frame);

    m_Buttons_Box.append(m_General_Label);
    m_Buttons_Box.append(m_Button_Exit);
    m_Buttons_Box.append(m_Button_Open);
    m_Buttons_Box.append(m_Button_Save);
    m_Buttons_Box.append(m_Button_Start);
    m_Buttons_Box.append(m_Button_Step);
    m_Buttons_Box.append(m_Check_Button);

    m_Side_Box.append(m_Info_Frame);
    m_Info_Frame.set_child(m_Info_Box);
    m_Info_Box.append(m_Info_Label);

    m_Info_Box.append(m_maj);
    m_Info_Box.append(m_Alg);
    m_Info_Box.append(m_Cor);
    m_Info_Box.append(m_Scav);

    m_maj.append(m_maj_Label);
    m_maj.append(m_maj_Count);

    m_Alg.append(m_Alg_Label);
    m_Alg.append(m_Alg_Count);

    m_Cor.append(m_Cor_Label);
    m_Cor.append(m_Cor_Count);

    m_Scav.append(m_Scav_Label);
    m_Scav.append(m_Scav_Count);

    m_Button_Start.signal_toggled().connect(
        sigc::mem_fun(*this, &MyArea::on_button_clicked_Start));

    m_Button_Step.signal_clicked().connect(
        sigc::mem_fun(*this, &MyArea::on_button_clicked_Step));

    m_Button_Open.signal_clicked().connect(
        sigc::mem_fun(*this, &MyArea::on_button_clicked_Open));

    m_Button_Save.signal_clicked().connect(
        sigc::mem_fun(*this, &MyArea::on_button_clicked_Save));

    m_Button_Exit.signal_clicked().connect(
        sigc::mem_fun(*this, &MyArea::on_button_clicked_Exit));

    auto keystroke = Gtk::EventControllerKey::create();
    keystroke->signal_key_pressed().connect(
        sigc::mem_fun(*this, &MyArea::on_window_key_pressed), false);
    add_controller(keystroke);
}

//start button define
void MyArea::on_button_clicked_Start()
{

    if (m_Button_Start.get_active())
    {
        m_Connection = Glib::signal_timeout().connect(
            sigc::mem_fun(*this, &MyArea::on_timeout), 50); //time set here
        m_Button_Start.set_label("stop");
    }

    else
    {
        m_Connection.disconnect();
        m_Button_Start.set_label("start");
    }
}

//step button define
void MyArea::on_button_clicked_Step()
{
    if (!m_Button_Start.get_active())
    {
        // bool set_naissance_algue = false;
        maj_count++;
        m_maj_Count.set_text(std::to_string(maj_count));
        update_gui_algue();
        m_Draw.queue_draw();
        if (m_Check_Button.get_active())
        {
            //random algue birth
            std::bernoulli_distribution b(alg_birth_rate);
            if (b(e))
            {
                std::uniform_real_distribution<double> u(0 + epsil_random,
                                                         256 - epsil_random);
                double x = u(e);
                double y = u(e);
                sim_->addAlgue(x, y, 1);
                m_Alg_Count.set_text(
                    std::to_string(sim_->get_Algues_storage().size()));
                m_Draw.queue_draw();
            }
        }
    }
    else
    {
        std::cout << "Simuation is active" << std::endl;
    }
}

//open button define
void MyArea::on_button_clicked_Open()
{

    auto dialog = new Gtk::FileChooserDialog("Please choose a file",
                                             Gtk::FileChooser::Action::OPEN);
    dialog->set_transient_for(*this);
    dialog->set_modal(true);
    dialog->signal_response().connect(
        sigc::bind(sigc::mem_fun(*this, &MyArea::on_file_dialog_response), dialog));

    dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
    dialog->add_button("_Open", Gtk::ResponseType::OK);

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Text files");
    filter_text->add_mime_type("text/plain");
    dialog->add_filter(filter_text);

    auto filter_cpp = Gtk::FileFilter::create();
    filter_cpp->set_name("C/C++ files");
    filter_cpp->add_mime_type("text/x-c");
    filter_cpp->add_mime_type("text/x-c++");
    filter_cpp->add_mime_type("text/x-c-header");
    dialog->add_filter(filter_cpp);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog->add_filter(filter_any);

    dialog->show();
}

//save button define
void MyArea::on_button_clicked_Save()
{

    auto dialog = new Gtk::FileChooserDialog("Enter a file name",
                                             Gtk::FileChooser::Action::SAVE);
    dialog->set_transient_for(*this);
    dialog->set_modal(true);
    dialog->signal_response().connect(
        sigc::bind(sigc::mem_fun(*this, &MyArea::on_save_dialog_response), dialog));

    dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
    dialog->add_button("_Save", Gtk::ResponseType::OK);

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Text files");
    filter_text->add_mime_type("text/plain");
    dialog->add_filter(filter_text);

    auto filter_cpp = Gtk::FileFilter::create();
    filter_cpp->set_name("C/C++ files");
    filter_cpp->add_mime_type("text/x-c");
    filter_cpp->add_mime_type("text/x-c++");
    filter_cpp->add_mime_type("text/x-c-header");
    dialog->add_filter(filter_cpp);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog->add_filter(filter_any);

    dialog->show();
}

//exit button define
void MyArea::on_button_clicked_Exit() { exit(0); }

//timer on and sim start define
bool MyArea::on_timeout()
{
    if (m_Button_Start.get_active())
    {
        update_gui_algue();
        maj_count++;
        m_maj_Count.set_text(std::to_string(maj_count));
        m_Draw.queue_draw();

        if (m_Check_Button.get_active())
        {
            //random algue birth
            std::bernoulli_distribution b(alg_birth_rate);
            if (b(e))
            {
                std::uniform_real_distribution<double> u(
                    0 + epsil_random, 256 - epsil_random); // ask abou this
                double x = u(e);
                double y = u(e);
                sim_->addAlgue(x, y, 1);
                m_Alg_Count.set_text(
                    std::to_string(sim_->get_Algues_storage().size()));
                m_Draw.queue_draw();
            }
        }
        return true;
    }
    return false;
}

//keyboard input for start and step define
bool MyArea::on_window_key_pressed(guint keyval, guint, Gdk::ModifierType state)
{
    switch (gdk_keyval_to_unicode(keyval))
    {
    case 's':
        if (m_Button_Start.get_active())
        {
            m_Button_Start.set_active(false);
        }
        else
        {
            m_Button_Start.set_active(true);
        }
        return true;
    case '1':
        on_button_clicked_Step();
        return true;
    }
    return false;
}

//error check and etc for when new file is opened
void MyArea::on_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog)
{
    switch (response_id)
    {
    case Gtk::ResponseType::OK:
    {
        reset_counters();
        sim_->reset_sim();
        m_Draw.queue_draw();
        auto filename = dialog->get_file()->get_path();
        std::cout << "File selected: " << filename << std::endl;
        sim_->File_read_for_sim(filename);
        if (sim_->no_failure())
        {
            init_sim();
        }
        break;
    }
    case Gtk::ResponseType::CANCEL:
    {
        break;
    }
    default:
    {
        break;
    }
    }
    delete dialog;
}

//file save dialog
void MyArea::on_save_dialog_response(int response_id, Gtk::FileChooserDialog *dialog)
{
    switch (response_id)
    {
    case Gtk::ResponseType::OK:
    {

        auto filename = dialog->get_file()->get_path();
        sim_->save(filename + ".txt");

        break;
    }
    case Gtk::ResponseType::CANCEL:
    {
        break;
    }
    default:
    {
        break;
    }
    }
    delete dialog;
}

//counters reset to 0
void MyArea::reset_counters()
{
    maj_count = 0;
    m_maj_Count.set_text(std::to_string(maj_count));
    m_Alg_Count.set_text(std::to_string(0));
    coral_count = 0;
    m_Cor_Count.set_text(std::to_string(coral_count));
    scav_count = 0;
    m_Scav_Count.set_text(std::to_string(scav_count));
}

static void orthographic_projection(const Cairo::RefPtr<Cairo::Context> &cr,
                                    const Frame &frame)
{
    cr->translate(frame.width / 2, frame.height / 2);

    cr->scale(frame.width / (frame.xMax - frame.xMin),
              -frame.height / (frame.yMax - frame.yMin));

    cr->translate(-(frame.xMin + frame.xMax) / 2, -(frame.yMin + frame.yMax) / 2);
}

//on_draw function calls the drawing
void DrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width,
                          int height)
{
    graphic_set_context(cr);
    adjustFrame(width, height);
    orthographic_projection(cr, frame);
    graphic_draw_square(256, 0, 0, grey);
    for (auto circle : Simulation::get_circlesContainer_algues())
    {
        circle.drawCircle();
    }

    for (auto circle : Simulation::get_circlesContainer_scavengers())
    {
        circle.drawCircle();
    }

    for (auto square : Simulation::get_squaresContainer())
    {
        square.drawSq();
    }

    for (auto seg : Simulation::get_segmentsContainer())
    {
        seg.drawSeg();
    }
}

//initialises the simulation and etc
void MyArea::init_sim()
{
    for (auto alg : sim_->get_Algues_storage())
    {
        m_Alg_Count.set_text(std::to_string(sim_->get_Algues_storage().size()));
        sim_->addCircle_Algue(alg.get_pos().x, alg.get_pos().y, green);
    }

    for (auto cor : sim_->get_Corals_storage())
    {
        coral_count++;
        m_Cor_Count.set_text(std::to_string(coral_count));
        sim_->addSquare(cor.get_pos().x, cor.get_pos().y, blue);
        for (auto seg : cor.get_seg_vector())
        {
            sim_->addSegment(seg.get_length(), seg.get_angle(), seg.get_origin().x,
                             seg.get_origin().y, blue);
        }
    }

    for (auto scav : sim_->get_Scavs_storage())
    {
        scav_count++;
        m_Scav_Count.set_text(std::to_string(scav_count));
        sim_->addCircle_Scavenger(scav.get_pos().x, scav.get_pos().y,
                                  scav.get_radius(), red);
    }

    m_Draw.queue_draw();
}

//erases algue / adds algues to the drawing
void MyArea ::update_gui_algue()
{
    sim_->update_algues();
    m_Alg_Count.set_text(std::to_string(sim_->get_Algues_storage().size()));
}