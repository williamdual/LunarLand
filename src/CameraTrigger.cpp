#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event.hpp>

#include "CameraTrigger.h"

using namespace godot;

void CameraTrigger::_bind_methods() {}

CameraTrigger::CameraTrigger() : Area3D()
{
    target_cam = nullptr;
}

CameraTrigger::~CameraTrigger()
{
    // Add your cleanup here.
}

void CameraTrigger::_enter_tree()
{
}

void CameraTrigger::_ready()
{
    collider = memnew(CollisionShape3D);
    collider->set_name("Coll");
    this->add_child(collider);
    collider->set_owner(get_tree()->get_edited_scene_root());

    //TODO GET THIS WORKING
    box_shape = memnew(BoxShape3D);
    box_shape->set_name("box_shape");
    collider->set_shape(box_shape);
    box_shape->set_size(Vector3(1, 1, 1));
}

void CameraTrigger::_process(double delta)
{
    if (Engine::get_singleton()->is_editor_hint())
        return; // Early return if we are in editor
}

void CameraTrigger::SetCamera(PlayerCamera *cam)
{
    this->target_cam = cam;
}

void CameraTrigger::SetPlayer(Node3D *p)
{
    this->player = p;
}

void CameraTrigger::setColliderTransformation(Vector3 size)
{
    box_shape->set_size(size);
}