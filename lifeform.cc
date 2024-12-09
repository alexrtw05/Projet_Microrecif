// 375390	Alexander Aoun  50%
// 371946	Ismail Mostafa  50%

#include <cmath>
#include <iostream>
#include <vector>

#include "lifeform.h"
#include "message.h"
#include "shape.h"

// using namespace because to hard to keep up  with std::
using namespace std;

std::vector<bool> Lifeform::fails;

// setters up next
void Coral::lifestatus_setter(bool status_cor_live)
{
    if (!status_cor_live)
    {
        lifestatus = DEAD;
    }

    else
    {
        lifestatus = ALIVE;
    }
}

void Coral::direction_setter(bool direction_rot) // direction
{
    if (!direction_rot)
    {
        direction = TRIGO;
    }

    else
    {
        direction = INVTRIGO;
    }
}

void Coral::extend_setter(bool extend)
{
    if (!extend)
    {
        extension_status = EXTEND;
    }

    else
    {
        extension_status = REPRO;
    }
}

bool Coral::segment_setter(unsigned int length_, double angle_,
                           vector<Coral> coral_storage)
{
    Segment s;
    // checking here because more convenient
    if ((length_ < l_repro - l_seg_interne) || (length_ >= l_repro))
    {
        cout << message::segment_length_outside(ID, length_);
        fails.push_back(false);
        return false;
    }

    else
    {
        s.set_length(length_);
    }

    if (segments_Coral.size() == 0)
    {
        s.set_origin(pos);
    }

    else
    {
        s.set_origin(segments_Coral[(segments_Coral.size() - 1)].get_segment_ends());
    }

    // angle check
    if ((angle_ < -M_PI) || (angle_ > M_PI))
    {
        cout << message::segment_angle_outside(ID, angle_);
        fails.push_back(false);
        return false;
    }

    else
    {
        s.set_angle(angle_);
    }

    S2d segment_ends = s.extr();

    // check for boundaries and spawn

    if (segment_ends.x <= epsil_zero || segment_ends.y <= epsil_zero ||
        segment_ends.x >= dmax - epsil_zero || segment_ends.y >= dmax - epsil_zero)
    {
        cout << message::lifeform_computed_outside(ID,segment_ends.x, segment_ends.y);
        fails.push_back(false);
        return false;
    }

    if (segment_ends.x <= 0. || segment_ends.x >= dmax || segment_ends.y <= 0. ||
        segment_ends.y >= dmax)
    {
        cout << message::lifeform_computed_outside(ID,segment_ends.x, segment_ends.y);
        fails.push_back(false);
        return false;
    }
    s.set_segment_ends(segment_ends);
    fails.push_back(check_superpos(s));
    fails.push_back(check_collision(s, coral_storage));
    fails.push_back(check_collision_all(s));
    segments_Coral.push_back(s);

    fails.push_back(true);
    return true;
}

bool Scavenger::radius_setter(double radius_)
{
    // radius check
    if ((radius_ < r_sca) || (radius_ >= r_sca_repro))
    {
        cout << message::scavenger_radius_outside(radius_);
        fails.push_back(false);
        return false;
    }

    else
    {
        radius = radius_;
    }
    fails.push_back(true);
    return true;
}

//superclass lifeform constructor
Lifeform::Lifeform(double x, double y, unsigned int age)
{
    age_check(age);
    boundary_check(x, y);
}

//getters up next
S2d Lifeform::get_pos() 
{
    return pos; 
}

unsigned int Lifeform::get_age() 
{
    return age;
}

std::vector<bool> Lifeform::get_fails()
{
    return fails;
}


bool Coral::get_rot()
{ 
    return direction;
}

bool Coral::get_extend() 
{
    return extension_status;
}

std::vector<Segment> Coral::get_seg_vector()
{
    return segments_Coral;
}

int Scavenger::get_radius()
{
    return radius;
}

Statut_sca Scavenger::get_status_scav()
{
    return status_scav_eat;
}

int Coral::get_ID() const
{
    return ID;
}

Statut_cor Coral::get_status_cor() const 
{ 
    return lifestatus;
}

void Lifeform::clear_fails()
{
    fails.clear();
}

//sub-class algue constructor
Algue::Algue(double x, double y, unsigned int age) : Lifeform(x, y, age) 
{
}

//sub-class coral constructor
Coral::Coral(double x, double y, unsigned int age, int ID, 
            bool status_cor_live, bool direction_Rot, 
            bool extend, std::vector<Coral> coral_storage,
            std::vector<Segment> coral_s): Lifeform(x, y, age)
{
    ID_check(ID, coral_storage);
    lifestatus_setter(status_cor_live);
    direction_setter(direction_Rot);
    extend_setter(extend);
}

//sub-class scavenger constructor
Scavenger::Scavenger(double x, double y, unsigned int age, bool status_eat,
              double radius, std::vector<Coral> coral_storage,
              int target_coral) : Lifeform(x, y, age)
{
    check_status_target(status_eat, coral_storage, target_coral);
    radius_setter(radius);
}

// checks for lifeforms
bool Lifeform::age_check(int age_)
{
    // age check
    if (!(age_ > 0))
    {
        cout << message::lifeform_age(age_);
        fails.push_back(false);
        return false;
    }
    else
    {
        age = age_;
    }
    fails.push_back(true);
    return true;
}

bool Lifeform::boundary_check(double x, double y)
{
    // spawn check
    if (x <= 0.|| x >= dmax || y <= 0. || y >= dmax)
    {
        cout << message::lifeform_center_outside(x, y);
        fails.push_back(false);
        return false;
    }
    // pos setter
    else
    {
        pos.x = x;
        pos.y = y;
        fails.push_back(true);
        return true;
    }
    
}

// checks for coral
bool Coral::ID_check(int ID_, vector<Coral> coral_storage)
{
    bool ID_taken(false);

    for (const auto &coral : coral_storage)
    {
        if (ID_ == coral.ID)
        {
            ID_taken = true;
            // Stop searching once an ID is found.
        }
    }
    // duplicate id check
    if (ID_taken)
    {
        cout << message::lifeform_duplicated_id(ID_);
        fails.push_back(false);
        return false;
    }

    else
    {
        ID = ID_;
    }
    fails.push_back(true);
    return true;
}
// checks superposition
bool Coral::check_superpos(Segment s)
{
    if (!segments_Coral.empty())
    {
        if (superpos(s, segments_Coral.back()))
        {
            cout << message::segment_superposition(ID, segments_Coral.size() - 1,
                                                   segments_Coral.size());
            fails.push_back(false);
            return false;
        }
    }
    fails.push_back(true);
    return true;
}
// checks collision between two consecutive segments
bool Coral::check_collision(Segment s, const vector<Coral> &coral_storage)
{
    size_t coralIndex = 0;

    for (const auto &coral : coral_storage)
    {
        size_t segmentIndex = 0;

        for (auto segment : coral.segments_Coral)
        {
            if (do_intersect(segment.get_origin(), segment.get_segment_ends(),
                             s.get_origin(), s.get_segment_ends()))
            {
                cout << message::segment_collision(coral.ID, segmentIndex, ID,
                                                   segments_Coral.size());
                fails.push_back(false);
                return false;
            }
            ++segmentIndex;
        }
        ++coralIndex;
    }
    fails.push_back(true);
    return true;
}
// checks collisions between all segments
bool Coral::check_collision_all(Segment s)
{
    size_t index = 0;

    for (auto segment : segments_Coral)
    {
        if (index >= segments_Coral.size() - 1)
        {
            break;
        }
        if (do_intersect(segment.get_origin(), segment.get_segment_ends(),
                         s.get_origin(), s.get_segment_ends()))
        {
            cout << message::segment_collision(ID, index, ID, segments_Coral.size());
            fails.push_back(false);
            return false;
        }
        ++index;
    }

    fails.push_back(true);
    return true;
}

// checks for Scavs
bool Scavenger::check_status_target(bool status_eat, vector<Coral> &coral_storage,
                                    int target_coral)
{
    if (status_eat)
    {
        for (const auto &coral : coral_storage)
        {
            bool status_cor = coral.get_status_cor();
            if (target_coral == coral.get_ID())
            {
                if (!status_cor)
                {
                    status_scav_eat = MANGE;
                    ID_target = target_coral;
                    break;
                }
            }

            else
            {
                // Coral ID does not exist or is not dead
                cout << message::lifeform_invalid_id(target_coral);
                fails.push_back(false);
                return false;
            }
        }
    }

    else
    {
        status_scav_eat = LIBRE;
    }

    fails.push_back(true);
    return true;
}

//increment algue age
void Lifeform::increment_age()
{
    age++;
}