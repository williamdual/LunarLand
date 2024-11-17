#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event.hpp>

#include "CameraTrigger.h"

using namespace godot;

void CameraTrigger::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("Collision"), &CameraTrigger::Collision);
    ADD_SIGNAL(MethodInfo("NewCamPos", PropertyInfo(Variant::VECTOR3, "camera_pos")));
}

CameraTrigger::CameraTrigger() : Area3D()
{
    target_cam = nullptr;
    player = nullptr;
}

CameraTrigger::~CameraTrigger()
{
    // Add your cleanup here.
}

void CameraTrigger::_enter_tree()
{
    collider = memnew(CollisionShape3D);
    collider->set_name("Coll");
    this->add_child(collider);
    collider->set_owner(get_tree()->get_edited_scene_root());

    // TODO GET THIS WORKING
    box_shape = memnew(BoxShape3D);
    box_shape->set_name("box_shape");
    collider->set_shape(box_shape);
    box_shape->set_size(Vector3(1, 5, 3));

    connect("area_entered", Callable(this, "Collision"));
}

void CameraTrigger::Collision(Area3D *area_that_entered)
{
    UtilityFunctions::print("HELLOOOO");
    UtilityFunctions::print(this->get_name());
    UtilityFunctions::print(area_that_entered->get_parent()->get_name());
    UtilityFunctions::print(player == nullptr);
    UtilityFunctions::print(target_cam == nullptr);
    if (area_that_entered->get_parent()->get_name() == player->get_name() && target_cam->is_current() == false)
    {
        UtilityFunctions::print(target_cam->get_name());
        target_cam->make_current();
        player->SetCamera(target_cam);
        emit_signal("NewCamPos", target_cam->get_global_position());
    }
}

void CameraTrigger::_ready()
{
    // UtilityFunctions::print("ready");
}

void CameraTrigger::_process(double delta)
{
    if (Engine::get_singleton()->is_editor_hint())
        return; // Early return if we are in editor
}

void CameraTrigger::SetTrigger()
{
    // get_overlapping_bodies() //<- gets the list of things overlapping with it, can iterate to find player and check name
    // this->area
}

void CameraTrigger::SetCamera(PlayerCamera *cam)
{
    this->target_cam = cam;
}

void CameraTrigger::SetPlayer(Player *p)
{
    this->player = p;
}

void CameraTrigger::setColliderTransformation(Vector3 size)
{
    box_shape->set_size(size);
}