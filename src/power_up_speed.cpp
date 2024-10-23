#include "power_up_speed.h"

using namespace godot;
void PowerUpSpeed::_bind_methods() {}

PowerUpSpeed::PowerUpSpeed(Vector3 pos, Color c, float r) : GameObject(EntityType::powerup_s, r)
{
    this->set_position(pos);
    mesh = memnew(SphereMesh);
    mesh->set_height(r);
    StandardMaterial3D *mat = memnew(StandardMaterial3D);
    mat->set_albedo(c);
    mesh->surface_set_material(0, mat);
    this->set_mesh(mesh);
}

void PowerUpSpeed::_enter_tree() {}
void PowerUpSpeed::_ready() {}
void PowerUpSpeed::_process(double delta) {}
void PowerUpSpeed::collide(EntityType) {}
