#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event.hpp>

#include "Player.h"

using namespace godot;

void Player::_bind_methods() {}

Player::Player() : MeshInstance3D()
{
}

Player::~Player()
{
    // Add your cleanup here.
}

void Player::_enter_tree()
{
    // Mesh and Mat
    mesh = memnew(CylinderMesh);
    mesh->set_height(2);
    mesh->set_top_radius(1);
    mesh->set_bottom_radius(1);
    set_mesh(mesh);

    mat = memnew(StandardMaterial3D);
    mat->set_albedo(Color(1.0f, 1.0f, 0.0f, 1.0f));
    mesh->surface_set_material(0, mat);

    set_mesh(mesh);

    // Colision
    area = memnew(Area3D);
    this->add_child(area);
    collider = memnew(CollisionShape3D);
    collider->set_name("Player_coll");
    area->add_child(collider);
    collider->set_owner(get_tree()->get_edited_scene_root());

    cylinder_shape = memnew(CylinderShape3D);
    cylinder_shape->set_name("cylinder_shape");
    collider->set_shape(cylinder_shape);
    cylinder_shape->set_height(2);
    cylinder_shape->set_radius(1);
}

void Player::_ready()
{
}

// called every frame (as often as possible)
void Player::_process(double delta)
{
    if (Engine::get_singleton()->is_editor_hint())
        return; // Early return if we are in editor
                // Game loop stuff HERE
}