#include "enemy.h"

using namespace godot;

void Enemy::_bind_methods() {}

Enemy::Enemy(Vector3 pos, QuatCamera *t, float r) : GameObject(EntityType::enemy, r)
{
    this->set_position(pos);
    target = t;

    mesh = memnew(CapsuleMesh);
    mesh->set_height(r);
    StandardMaterial3D *mat = memnew(StandardMaterial3D);
    mat->set_albedo(Color(1.0f, 0.0f, 0.0f, 1.0f));
    mesh->surface_set_material(0, mat);
    this->set_mesh(mesh);
}

void Enemy::_process(double delta)
{
    if (target != nullptr)
    {
        this->set_position(this->get_position() + (target->get_position() - this->get_position()).normalized() * delta * speed);
    }
}

void Enemy::_enter_tree() {}
void Enemy::_ready() {}
void Enemy::collide(EntityType) {}