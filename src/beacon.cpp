#include "beacon.h"

#include <godot_cpp/core/memory.hpp> // for memnew
#include <godot_cpp/classes/random_number_generator.hpp>

using namespace godot;

void Beacon::_bind_methods() {}

Beacon::Beacon(Vector3 pos, Color c, float r, bool t) : GameObject(EntityType::beacon, r)
{
    color = c;
    this->set_global_position(pos);
    bool targeted = t;

    targetedMesh = memnew(SphereMesh);
    targetedMesh->set_height(r);
    StandardMaterial3D *mat = memnew(StandardMaterial3D);
    mat->set_albedo(Color(255.0f, 165.0f, 0.0f, 1.0f));
    targetedMesh->surface_set_material(0, mat);

    if (t)
        becomeTargeted();
    else
    {

        notTargetedMesh = memnew(BoxMesh);
        notTargetedMesh->set_size(Vector3(r, r, r));
        StandardMaterial3D *mat = memnew(StandardMaterial3D);
        mat->set_albedo(color);
        notTargetedMesh->surface_set_material(0, mat);
        this->set_mesh(notTargetedMesh);
    }
}

void Beacon::becomeTargeted()
{
    targeted = true;
    this->set_mesh(targetedMesh);
}

void Beacon::_enter_tree() {}
void Beacon::_ready() {}
void Beacon::_process(double delta) {}
void Beacon::collide(EntityType) {}