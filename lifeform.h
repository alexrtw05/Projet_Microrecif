// 375390	Alexander Aoun  50%
// 371946	Ismail Mostafa  50%

#ifndef LIFEFORM_H
#define LIFEFORM_H

#include <vector>

#include "constantes.h"
#include "shape.h"

//superclass lifeform
class Lifeform
{
protected:
    unsigned int age;
    static std::vector<bool> fails;
    S2d pos;

public:
    Lifeform(double x = 0., double y = 0., unsigned int age = 0);
    ~Lifeform() = default;
    bool age_check(int age_); //tests that all lifeforms have
    bool boundary_check(double x, double y);

    static std::vector<bool> get_fails();
    static void clear_fails();
    S2d get_pos();
    unsigned int get_age();
    void increment_age();
};

//sub-class algue
class Algue : public Lifeform
{
public:
    //constructor for algue
    Algue(double x, double y, unsigned int age);
};

// sub-class coral
class Coral : public Lifeform
{
private:
    std::vector<Segment> segments_Coral;

    int ID;

    Statut_cor lifestatus; // bool values
    Dir_rot_cor direction;
    Statut_dev extension_status;

public:
    // constructor for coral
    Coral(double x, double y, unsigned int age, int ID = 0, bool status_cor_live = 0,
          bool direction_Rot = 0, bool extend = 0,
          std::vector<Coral> coral_storage = {}, std::vector<Segment> coral_s = {});

    // getters for some values
    Statut_cor get_status_cor() const;

    int get_ID() const;
    bool get_rot();
    bool get_extend();
    std::vector<Segment> get_seg_vector();

    // setters for some values
    bool segment_setter(unsigned int length_, double angle_,
                        std::vector<Coral> coral_storage);

    void direction_setter(bool direction_rot);

    void extend_setter(bool repro);

    void lifestatus_setter(bool lifestatus);

    // Coral tests including ID check and checks that involve segments
    bool ID_check(int ID_, std::vector<Coral> coral_storage);

    bool check_superpos(Segment s);

    bool check_collision(Segment s, const std::vector<Coral> &coral_storage);

    bool check_collision_all(Segment s);
};

// sub-class scavenger
class Scavenger : public Lifeform
{
private:
    int ID_target;
    int radius;

    Statut_sca status_scav_eat;

public:
    // constructor for Scavenger
    Scavenger(double x, double y, unsigned int age, bool status_eat = 0,
              double radius = 0., std::vector<Coral> coral_storage = {},
              int target_coral = 0);
    int get_radius();
    Statut_sca get_status_scav();

    // setter for radius
    bool radius_setter(double radius_);

    // check for scav
    bool check_status_target(bool status_eat, std::vector<Coral> &coral_storage,
                             int target_coral);
};

#endif