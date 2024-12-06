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
    // init vars
    saved_velocity = Vector3(0, 0, 0);
    moveSpeed = 1000.0f;
    paused = false;
    gravityDelta = Vector3(0, -9.8 * 50.0, 0); // I have no idea if this works how real gravity works <- No doesnt look like it
    camera = nullptr;
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
    CylinderShape3D *surface_collider_shape = memnew(CylinderShape3D);
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
    Input *_input = Input::get_singleton();

    // dev tool for placing stuff
    if (_input->is_action_just_pressed("ui_right"))
    {
        Vector3 editor_cam_pos = EditorInterface::get_singleton()->get_editor_viewport_3d()->get_camera_3d()->get_global_position();
        Vector3 editor_cam_rot = EditorInterface::get_singleton()->get_editor_viewport_3d()->get_camera_3d()->get_global_rotation_degrees();
        UtilityFunctions::print("Cam Pos - Rot");
        UtilityFunctions::print(editor_cam_pos);
        UtilityFunctions::print(editor_cam_rot);
    }

    if (_input->is_action_just_pressed("ui_left"))
    {
        UtilityFunctions::print("Trigg:");
        UtilityFunctions::print(get_position());
    }
    // END OF DEV TOOL FOR PLACING STUFF

    if (Engine::get_singleton()->is_editor_hint())
        return; // Early return if we are in editor
                // Game loop stuff HERE

    // Checking if game should pause or unpause
    if (_input->is_action_just_pressed("pause"))
    {
        paused = !paused;
    }

    // Not accepting input if game is paused
    if (paused)
    {
        return;
    }

    // Movement block
    Vector3 prev_pos = this->get_global_position(); // before moving, record the players position so that we can unditch them if they are stuck
    movementDelta = Vector3(0, 0, 0);               // reset movementDelta, so that the previous frame input doesn't bleed into this frames input.

    // forsake savedMovementDelta, if the player releases any movement key, or presses any movement key
    bool anyMovementKeyWasReleased = (_input->is_action_just_released("move_forward") || _input->is_action_just_released("move_backward") || _input->is_action_just_released("move_right") || _input->is_action_just_released("move_left"));
    bool anyMovementKeyWasJustPressed = (_input->is_action_just_pressed("move_forward") || _input->is_action_just_pressed("move_backward") || _input->is_action_just_pressed("move_right") || _input->is_action_just_pressed("move_left"));
    if (anyMovementKeyWasReleased || anyMovementKeyWasJustPressed)
    {
        // UtilityFunctions::print("savedMovementDelta has been forsaken");
        savedMovementDelta = Vector3(0, 0, 0);
    }

    if (savedMovementDelta == Vector3(0, 0, 0)) // if savedMovementDelta is zero, then we look for user input
    {
        Vector3 movmentInputVector = Vector3(0, 0, 0);
        if (_input->is_action_pressed("move_forward"))
        {
            // UtilityFunctions::print("forward");
            movmentInputVector += camera->GetMovementPlaneForward();
        }

        if (_input->is_action_pressed("move_backward"))
        {
            // UtilityFunctions::print("backward");
            movmentInputVector -= camera->GetMovementPlaneForward();
        }

        if (_input->is_action_pressed("move_right"))
        {
            // UtilityFunctions::print("right");
            movmentInputVector += camera->GetMovementPlaneSide();
        }

        if (_input->is_action_pressed("move_left"))
        {
            // UtilityFunctions::print("left");
            movmentInputVector -= camera->GetMovementPlaneSide();
        }

        movementDelta = movmentInputVector.normalized() * moveSpeed;
    }
    else if (savedMovementDelta != Vector3(0, 0, 0)) // if savedMovementDelta is not zero, then we use it as movementDelta
    {
        // UtilityFunctions::print("Using savedMovementDelta as movementDelta");
        movementDelta = savedMovementDelta;
    }

    // apply movementDelta and gravityDelta
    this->set_velocity((movementDelta + gravityDelta) * delta);
    this->move_and_slide();

    // if the player tried to move but was unable to move, give them a vertical boost
    if ((movementDelta != Vector3(0, 0, 0)) && (prev_pos == this->get_global_position()))
    {
        // UtilityFunctions::print("Unditching player");
        // this->set_global_position(this->get_global_position() + Vector3(0.0, 1.0, 0.0));//move the player up 1 unit
    }
}

PlayerCamera *Player::GetCamera()
{
    return camera;
}

void Player::SetCamera(PlayerCamera *cam)
{
    if (camera != nullptr)
    {
        savedMovementDelta = movementDelta; // save the current movementDelta
        // UtilityFunctions::print("Set savedMovementDelta as:", savedMovementDelta);
    }

    camera = cam;
}

float Player::GetMoveSpeed()
{
    return moveSpeed;
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