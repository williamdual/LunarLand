#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event.hpp>

#include "Player.h"

using namespace godot;

void Player::_bind_methods() {}

Player::Player() : CharacterBody3D()
{
}

Player::~Player()
{
    // Add your cleanup here.
}

void Player::_enter_tree()
{
    // Mesh and Mat
    create_and_add_as_child<MeshInstance3D>(mesh_instance, "PlayerMesh", true);

    mesh = memnew(CylinderMesh);
    mesh->set_height(2);
    mesh->set_top_radius(1);
    mesh->set_bottom_radius(1);
    mesh_instance->set_mesh(mesh);

    mat = memnew(StandardMaterial3D);
    mat->set_albedo(Color(1.0f, 1.0f, 0.0f, 1.0f));
    mesh->surface_set_material(0, mat);

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

    // Setting the collision shape that will serve to detect collisions with physical objects in the environment
    surface_collider = memnew(CollisionShape3D);
    create_and_add_as_child<CollisionShape3D>(surface_collider, "SurfaceCollider", true);
    CylinderShape3D* surface_collider_shape = memnew(CylinderShape3D);
    surface_collider_shape->set_height(2);
    surface_collider_shape->set_radius(1);
    surface_collider->set_shape(surface_collider_shape);

    // Setting the listener
    create_and_add_as_child<AudioListener3D>(listener, "Listener", true);
    listener->make_current();

    // Creating the inventory
    create_and_add_as_child<Inventory>(inventory, "Inventory", true);
    inventory->SetupInventory();
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

    Input *_input = Input::get_singleton();
    float moveSpeed = 10;

    if (_input->is_action_pressed("move_forward"))
    {
        //this->set_global_position(this->get_global_position() + camera->GetMovementPlaneForward() * delta * moveSpeed);
        this->set_velocity(camera->GetMovementPlaneForward() * moveSpeed + Vector3(0.0, -1.0, 1.0));
        this->move_and_slide();
        //this->set_global_position(this->get_global_position() + camera->GetMovementPlaneForward() * delta * moveSpeed);
    }
    if (_input->is_action_pressed("move_backward"))
    {
        //this->set_global_position(this->get_global_position() - camera->GetMovementPlaneForward() * delta * moveSpeed);
        this->set_velocity(-1.0 * camera->GetMovementPlaneForward() * moveSpeed + Vector3(0.0, -1.0, 1.0));
        this->move_and_slide();
    }
    if (_input->is_action_pressed("move_right"))
    {
        //this->set_global_position(this->get_global_position() + camera->GetMovementPlaneSide() * delta * moveSpeed);
        this->set_velocity(camera->GetMovementPlaneSide() * moveSpeed + Vector3(0.0, -1.0, 1.0));
        this->move_and_slide();
    }
    if (_input->is_action_pressed("move_left"))
    {
        //this->set_global_position(this->get_global_position() - camera->GetMovementPlaneSide() * delta * moveSpeed);
        this->set_velocity(-1.0 * camera->GetMovementPlaneSide() * moveSpeed + Vector3(0.0, -1.0, 1.0));
        this->move_and_slide();
    }
}

PlayerCamera *Player::GetCamera()
{
    return camera;
}

void Player::SetCamera(PlayerCamera *cam)
{
    camera = cam;
}

template <class T>
// returns true if pointer is brand-new; false if retrieved from SceneTree
bool Player::create_and_add_as_child(T *&pointer, String name, bool search)
{
	// this is the default behaviour
	// added the search parameter so that we can skip the slow "find_child" call during runtime (not applicable to this demo, you should always use search = true until next assignment)
	if (search == false)
	{
		pointer = memnew(T);
		pointer->set_name(name);
		add_child(pointer);
		pointer->set_owner(get_tree()->get_edited_scene_root());
		return true;
	}

	// always only have to search once if we save it here
	Node *child = find_child(name);

	if (child == nullptr)
	{
		pointer = memnew(T);
		pointer->set_name(name);
		add_child(pointer);
		pointer->set_owner(get_tree()->get_edited_scene_root());
		return true;
	}
	else
	{
		pointer = dynamic_cast<T *>(child);
		return false;
	}
}