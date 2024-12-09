// 375390	Alexander Aoun  60%
// 371946	Ismail Mostafa  40%

#ifndef SIMULATION_H
#define SIMULATION_H

#include <fstream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "lifeform.h"
#include "shape.h"

// Simulation class handles all methods related to file
// reading and assigning values to lifeforms...

class Simulation
{
private:
    std::vector<Algue> Algues_storage;
    std::vector<Coral> Corals_storage;
    std::vector<Scavenger> Scavs_storage;
    static std::vector<Circle> circles_container_algues;
    static std::vector<Circle> circles_container_scavengers;
    static std::vector<Square> squares_container;
    static std::vector<Segment> segments_container;
    static std::default_random_engine e;
    std::vector<bool> random_algues;

public:
    void File_read_for_sim(std::string file_name);
    bool no_failure();
    void empty_storage();
    void reset_sim();
    void resetReadFails();
    std::vector<Algue> parse_algues(std::ifstream &file_to_read);
    std::vector<Coral> parse_corals(std::ifstream &file_to_read);
    std::vector<Scavenger> parse_scavs(std::ifstream &file_to_read,
                                       std::vector<Coral> &coral_storage);

    void save(std::string filename);
    void addAlgue(double x, double y, unsigned int age);
    void addCircle_Algue(double x, double y, Colors color);
    void addCircle_Scavenger(double x, double y, double radius, Colors color);
    void addSquare(double x, double y, Colors color);
    void addSegment(int l, double a, double x, double y, Colors color);

    std::vector<Algue> get_Algues_storage();
    std::vector<Coral> get_Corals_storage();
    std::vector<Scavenger> get_Scavs_storage();

    static std::vector<Circle> get_circlesContainer_algues();
    static std::vector<Circle> get_circlesContainer_scavengers();
    static std::vector<Square> get_squaresContainer();
    static std::vector<Segment> get_segmentsContainer();

    void update_algues();
};

#endif