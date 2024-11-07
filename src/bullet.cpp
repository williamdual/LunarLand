#include "bullet.h"

using namespace godot;
void Bullet::_bind_methods() {}

Bullet::Bullet(Vector3 pos, Vector3 f, Color c, float s, float r) : GameObject(EntityType::bullet, r * 100)
{
    this->set_position(pos);
    forward = f;
    speed = s;
    mesh = memnew(SphereMesh);
    mesh->set_height(r);
    mesh->set_radius(r);
    StandardMaterial3D *mat = memnew(StandardMaterial3D);
    mat->set_albedo(c);
    mesh->surface_set_material(0, mat);
    this->set_mesh(mesh);
    timeSinceBirth = 0;
}

void Bullet::_enter_tree() {}
void Bullet::_ready() {}
void Bullet::_process(double delta)
{
    timeSinceBirth += delta;
    this->set_position(this->get_position() + forward * delta * speed);
}
void Bullet::collide(EntityType) {}

bool Bullet::overdue()
{
    return timeSinceBirth >= 5.0f;
}