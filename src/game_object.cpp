#include "game_object.h"

using namespace godot;

void GameObject::_bind_methods() {}

GameObject::GameObject(EntityType e, float r) : MeshInstance3D()
{
    gameOver = false;
    radius = r / 25;
    entity = e;
}

bool GameObject::isColliding(GameObject *other)
{
    float dist = this->get_position().distance_to(other->get_position());
    if (dist <= this->radius + other->radius)
    {
        this->collide(other->entity);
        other->collide(entity);
        return true;
    }
    return false;
}
bool GameObject::isColliding(QuatCamera *other)
{
    float dist = this->get_position().distance_to(other->get_position());
    if (dist <= this->radius + other->radius)
    {
        other->collide(entity);
        return true;
    }
    return false;
}