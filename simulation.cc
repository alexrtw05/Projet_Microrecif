// 375390	Alexander Aoun  60%
// 371946	Ismail Mostafa  40%

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "constantes.h"
#include "lifeform.h"
#include "message.h"
#include "shape.h"
#include "simulation.h"

using namespace std;

//shape containers
std::vector<Circle> Simulation::circles_container_algues;
std::vector<Circle> Simulation::circles_container_scavengers;
std::vector<Square> Simulation::squares_container;
std::vector<Segment> Simulation::segments_container;

// function reads file
void Simulation::File_read_for_sim(string file_name)
{
    string line;
    ifstream file(file_name);
    this->resetReadFails();
    this->reset_sim();
    if (file.fail())
    {
        cout << "File reading failed / File doesn't exist" << endl;
        exit(EXIT_FAILURE); // If can't read file
    }
    else
    {
        Algues_storage = parse_algues(file);
        Corals_storage = parse_corals(file);
        Scavs_storage = parse_scavs(file, Corals_storage);

        if (this->no_failure())
        {
            cout << message::success(); // Success if no errors detected
            file.clear();
            file.seekg(0, ios::beg); // Move the file pointer to the beginning
            file.close();
        }
        else
        {
            file.clear();
            file.seekg(0, ios::beg);
            file.close();
        }
    }
}

// Algue read
vector<Algue> Simulation::parse_algues(ifstream &file_to_read)
{
    vector<Algue> algues_vect; // Vector to store algues
    string line;
    int algues_to_read = 0; // Number of algues to read

    while (getline(file_to_read >> ws, line))
    {
        if (line[0] != '#')
        {
            istringstream(line) >> algues_to_read; // Extract number of algues
            break;
        }
    }

    // Read and parse each algue
    for (int i = 0; i < algues_to_read; ++i)
    {
        while (getline(file_to_read >> ws, line))
        {
            if (line[0] != '#')
            {
                double x = 0.0, y = 0.0;
                int age = 0;

                istringstream ss(line);
                ss >> x >> y >> age; // Parse the algue's details

                Algue algue(x, y, age);       // Create a random algue
                algues_vect.push_back(algue); // Add to the collection
                break;
            }
        }
    }

    return algues_vect;
}

// Coral read (Sorry for putting the "{" right next to the function didnt have space)
vector<Coral> Simulation::parse_corals(ifstream &file_to_read)
{
    vector<Coral> coral_storage; // Vector to store corals
    string line;
    int coral_to_read = 0; // Number of corals to read

    while (getline(file_to_read >> ws, line))
    {
        if (line[0] != '#') {
            istringstream(line) >> coral_to_read; // Extract the count
            break;
        }
    }
    for (int i = 0; i < coral_to_read; ++i)
    { // Read and parse each coral
        while (getline(file_to_read >> ws, line)){
            if (line[0] != '#'){
                double x, y;
                int age, ID, segment_count;
                bool statut_live, direction_rot, extend;
                istringstream ss(line);
                ss >> x >> y >> age >> ID >> statut_live >> direction_rot >> extend >>
                    segment_count;

                Coral coral(x, y, age, ID, statut_live, direction_rot, extend,
                            coral_storage);

                for (int j = 0; j < segment_count; ++j) { // Parse segments
                    getline(file_to_read >> ws, line);
                    double angle;
                    int length;
                    istringstream segment_parse(line);
                    segment_parse >> angle >> length;
                    coral.segment_setter(length, angle, coral_storage);
                }
                coral_storage.push_back(coral); // Add to the collection
                break;
            }
        }
    }
    return coral_storage;
}

// Scavenger read
vector<Scavenger> Simulation::parse_scavs(ifstream &file_to_read,
                                          vector<Coral> &coral_storage)
{
    vector<Scavenger> scavs_vect; // Vector to store scavengers
    string line;
    int scav_to_read = 0; // Number of scavengers to read

    while (getline(file_to_read >> ws, line))
    {
        if (line[0] != '#')
        {
            istringstream(line) >> scav_to_read;
            break;
        }
    }

    for (int i = 0; i < scav_to_read; ++i) // Read and parse each scavenger
    {
        while (getline(file_to_read >> ws, line))
        {
            if (line[0] != '#')
            {
                int age, radius, target_coral = 0;
                double x, y;
                bool status_scav;

                istringstream ss(line);
                ss >> x >> y >> age >> radius >> status_scav;
                // If scavenger has a target coral
                if (status_scav)
                {
                    ss >> target_coral;
                }
                Scavenger scavenger(x, y, age, status_scav, radius, coral_storage,
                                    target_coral);
                scavs_vect.push_back(scavenger); // Add to the collection

                break;
            }
        }
    }
    return scavs_vect;
}

//checks if any errors in file via lifeform
bool Simulation::no_failure()
{
    for (auto check : Lifeform::get_fails())
    {
        if (!check)
        {
            return false;
        }
    }
    return true;
}

//empties all entity storages
void Simulation::empty_storage()
{
    Algues_storage.clear();
    Corals_storage.clear();
    Scavs_storage.clear();
}

//save file function
void Simulation::save(string filename)
{
    ofstream outfile(filename);
    if (Algues_storage.size() == 0 && Corals_storage.size() == 0 &&
        Scavs_storage.size() == 0)
    {
        outfile << "0 \n0 \n0";
    }
    else
    {
        outfile << "# Algues: " << endl;
        outfile << Algues_storage.size() << endl;
        for (auto item : Algues_storage)
        {
            outfile << "    " << item.get_pos().x << " " << item.get_pos().y << " "
                    << item.get_age() << "\n"
                    << endl;
        }
        outfile << "# Corals: " << endl;
        outfile << Corals_storage.size() << endl;
        for (auto item : Corals_storage) {
            outfile << "    " << item.get_pos().x << " " << item.get_pos().y << " "
                    << item.get_age() << " " << item.get_ID() << " ";
            outfile << item.get_status_cor() << " " << item.get_rot() << " "
                    << item.get_extend() << " " << item.get_seg_vector().size()
                    << endl;
            for (auto seg : item.get_seg_vector())
            {
                outfile << "        " << seg.get_angle() << " " << seg.get_length()
                        << endl;
            }
        }
        outfile << "# Scavengers: " << endl;
        outfile << Scavs_storage.size() << endl;
        for (auto item : Scavs_storage) {
            outfile << "    " << item.get_pos().x << " " << item.get_pos().y << " "
                    << item.get_age() << " " << item.get_radius() << " "
                    << item.get_status_scav() << endl;
        }
    }
    outfile.close();
}

//getter for circles container for algues
std::vector<Circle> Simulation::get_circlesContainer_algues()
{
    return circles_container_algues;
}

//getter for circles container for scavengers
std::vector<Circle> Simulation::get_circlesContainer_scavengers()
{
    return circles_container_scavengers;
}

//getter for squares container
std::vector<Square> Simulation::get_squaresContainer() { return squares_container; }

//getter for segments container
std::vector<Segment> Simulation::get_segmentsContainer() { return segments_container;}

//function that adds new algue into vectors
void Simulation::addAlgue(double x, double y, unsigned int age)
{
    Algue newAlgue(x, y, age);
    Algues_storage.push_back(newAlgue);
    addCircle_Algue(x, y, green);
}

//function that adds the algue shape into the designed vector
void Simulation::addCircle_Algue(double x, double y, Colors color)
{
    Circle newCircle(r_alg, x, y, color);
    circles_container_algues.push_back(newCircle);
}

//function that adds the scavenger shape into the designed vector
void Simulation::addCircle_Scavenger(double x, double y, double radius, Colors color)
{
    Circle newCircle(radius, x, y, color);
    circles_container_scavengers.push_back(newCircle);
}

////function that adds the squares into the designed vector
void Simulation::addSquare(double x, double y, Colors color)
{
    Square newSquare(r_sca, x, y, color);
    squares_container.push_back(newSquare);
}

//function that adds the segments into the designed vector
void Simulation::addSegment(int l, double a, double x, double y, Colors color)
{
    Segment newSegment(l, a, x, y, color);
    segments_container.push_back(newSegment);
}

//function that empties out everything to reset simulation
void Simulation::reset_sim()
{
    this->empty_storage();
    random_algues.clear();
    circles_container_algues.clear();
    circles_container_scavengers.clear();
    segments_container.clear();
    squares_container.clear();
}

//getters
std::vector<Algue> Simulation::get_Algues_storage() { return Algues_storage; }

std::vector<Coral> Simulation::get_Corals_storage() { return Corals_storage; }

std::vector<Scavenger> Simulation::get_Scavs_storage() { return Scavs_storage; }

//function to increment algue age and remove algue from vector if too old
void Simulation::update_algues()
{
    for (size_t i = 0; i < Algues_storage.size(); ++i)
    {
        Algues_storage[i].increment_age();
        if (Algues_storage[i].get_age() >= max_life_alg)
        {
            swap(Algues_storage[i], Algues_storage[Algues_storage.size() - 1]);
            Algues_storage.pop_back();
            swap(circles_container_algues[i],
                 circles_container_algues[circles_container_algues.size() - 1]);
            circles_container_algues.pop_back();
        }
    }
}

void Simulation::resetReadFails() { Lifeform::clear_fails(); }