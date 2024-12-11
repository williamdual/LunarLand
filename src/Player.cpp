#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event.hpp>

#include "Player.h"

using namespace godot;

void Player::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("SetCameraPosition"), &Player::SetCameraPosition);
}

Player::Player() : CharacterBody3D()
{
}

Player::~Player()
{
    // Add your cleanup here.
}

void Player::_enter_tree()
{

    // Setting proper array sizes
    light_positions.resize(32);
    light_colours.resize(32);
    specular_power.resize(32);
    num_lights = 0;

    // Setting up shader
    mat = memnew(ShaderMaterial);
    Ref<Shader> shader = ResourceLoader::get_singleton()->load(vformat("%s%s.gdshader", "Shaders/", "player"), "Shader");
    mat->set_shader(shader);

    // init vars
    time_passed = 0.0;
    saved_velocity = Vector3(0, 0, 0);
    moveSpeed = 1000.0f;
    paused = false;
    gravityDelta = Vector3(0, -9.8 * 50.0, 0); // I have no idea if this works how real gravity works <- No doesnt look like it
    camera = nullptr;
    rot_speed = Math_TAU * 2.0;
    camera_position = Vector3(0.0, 0.0, 0.0);
    start_log_pos = 0.0;

    // Mesh and Mat
    create_and_add_as_child<MeshInstance3D>(mesh, "PlayerMesh", true);
    idle = ResourceLoader::get_singleton()->load(vformat("%s%s.obj", "Models/", "playerIdle"), "Mesh");
    idle->surface_set_material(0, mat);
    mesh->set_mesh(idle);
    mesh->set_material_override(mat);
    mesh->set_global_position(Vector3(0.0, -2.7, 0.0));

    // Setting up movement meshes
    first_move = ResourceLoader::get_singleton()->load(vformat("%s%s.obj", "Models/", "playerRoughWalkFrame01"), "Mesh");
    first_move->surface_set_material(0, mat);
    second_move = ResourceLoader::get_singleton()->load(vformat("%s%s.obj", "Models/", "playerRoughWalkFrame02"), "Mesh");
    second_move->surface_set_material(0, mat);

    // Setting the texture
    Ref<Texture2D> texture = ResourceLoader::get_singleton()->load(vformat("%s%s", "Textures/", "Person_Texture.jpg"), "CompressedTexture2D");
    mat->set_shader_parameter("sampler", texture);

    // Setting the darking value for the texture
    mat->set_shader_parameter("darkening_val", 0.8);

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

    // Instantiating stream player and listener
    this->create_and_add_as_child<AudioStreamPlayer3D>(start_log, "StartLog", true);

    // Getting the file and setting the stream
    Ref<AudioStreamOggVorbis> stream = ResourceLoader::get_singleton()->load(vformat("%s%s.ogg", "SoundFiles/", "susie1"), "AudioStreamOggVorbis");
    start_log->set_max_distance(3.0);
    start_log->set_stream(stream);

    // Setting the listener
    create_and_add_as_child<AudioListener3D>(listener, "Listener", true);
    listener->make_current();

    // Creating the inventory
    create_and_add_as_child<Inventory>(inventory, "Inventory", true);
    inventory->SetupInventory();

    // Seeing if the object already has a collision shape
    if (!find_child("CollisionShape") == NULL)
    {
        return;
    }

    // Creating the hit shape
    create_and_add_as_child<CollisionShape3D>(hit_shape, "CollisionShape", false);

    // Determining widest part of scale
    CylinderShape3D *cyl_shape = memnew(CylinderShape3D);
    Vector3 obj_size = mesh->get_mesh()->get_aabb().size;
    float diameter = 1.0;
    if (obj_size.x > obj_size.z)
    {
        diameter = obj_size.x;
    }
    else
    {
        diameter = obj_size.z;
    }

    // Setting the height and radius of the shape
    cyl_shape->set_height(obj_size.y);
    cyl_shape->set_radius(diameter / 2.0);

    // Setting the collision shape
    hit_shape->set_shape(cyl_shape);

    // Setting the shape for trigger collision
    cylinder_shape->set_height(cyl_shape->get_height());
    cylinder_shape->set_radius(cyl_shape->get_radius());
}

void Player::_ready()
{
}

// called every frame (as often as possible)
void Player::_process(double delta)
{
    // Changing camera position if needed
    if (camera != NULL)
    {
        camera_position = camera->get_global_position();
    }

    // Sending the necessary values to the shader
    mat->set_shader_parameter("init_light_positions", light_positions);
    mat->set_shader_parameter("light_colours", light_colours);
    mat->set_shader_parameter("view_pos", camera_position);
    mat->set_shader_parameter("spec_power", specular_power);
    mat->set_shader_parameter("num_lights", num_lights);

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

    // Playing initial log on startup
    if (time_passed == 0.0)
    {
        start_log->play();
    }

    // Adding the delta to time passed
    time_passed += delta;

    // Checking if game should pause or unpause
    if (_input->is_action_just_pressed("pause"))
    {
        paused = !paused;
        if (paused)
        {
            start_log->stop();
        }
        else
        {
            float duration = start_log->get_stream()->get_length();
            if (start_log_pos < duration)
            {
                start_log->play(start_log_pos);
            }
        }
    }

    // Not accepting input if game is paused
    if (paused)
    {
        return;
    }
    else
    {
        start_log_pos += delta;
    }

    // Movement block
    Vector3 prev_pos = this->get_global_position(); // before moving, record the players position so that we can unditch them if they are stuck
    movementDelta = Vector3(0, 0, 0);               // reset movementDelta, so that the previous frame input doesn't bleed into this frames input.

    // forsake savedMovementDelta, if the player releases any movement key, or presses any movement key
    bool anyMovementKeyWasReleased = (_input->is_action_just_released("move_forward") || _input->is_action_just_released("move_backward") || _input->is_action_just_released("move_right") || _input->is_action_just_released("move_left"));
    bool anyMovementKeyWasJustPressed = (_input->is_action_just_pressed("move_forward") || _input->is_action_just_pressed("move_backward") || _input->is_action_just_pressed("move_right") || _input->is_action_just_pressed("move_left"));
    bool anyMovementKeyWasPressed = (_input->is_action_pressed("move_forward") || _input->is_action_pressed("move_backward") || _input->is_action_pressed("move_right") || _input->is_action_pressed("move_left"));
    if (anyMovementKeyWasReleased || anyMovementKeyWasJustPressed)
    {
        // UtilityFunctions::print("savedMovementDelta has been forsaken");
        savedMovementDelta = Vector3(0, 0, 0);
    }

    if (savedMovementDelta == Vector3(0, 0, 0)) // if savedMovementDelta is zero, then we look for user input
    {
        Vector3 movmentInputVector = Vector3(0, 0, 0);
        float theta = 0.0;
        float new_rot = 0.0;
        if (_input->is_action_pressed("move_forward"))
        {
            theta += WrapDegree(atan2(camera->GetMovementPlaneForward().x, camera->GetMovementPlaneForward().z) - mesh->get_global_rotation().y);
            movmentInputVector += camera->GetMovementPlaneForward();
        }

        if (_input->is_action_pressed("move_backward"))
        {
            theta += WrapDegree(atan2(-camera->GetMovementPlaneForward().x, -camera->GetMovementPlaneForward().z) - mesh->get_global_rotation().y);
            movmentInputVector -= camera->GetMovementPlaneForward();
        }

        if (_input->is_action_pressed("move_right"))
        {
            theta += WrapDegree(atan2(camera->GetMovementPlaneSide().x, camera->GetMovementPlaneSide().z) - mesh->get_global_rotation().y);
            movmentInputVector += camera->GetMovementPlaneSide();
        }

        if (_input->is_action_pressed("move_left"))
        {
            theta += WrapDegree(atan2(-camera->GetMovementPlaneSide().x, -camera->GetMovementPlaneSide().z) - mesh->get_global_rotation().y);
            movmentInputVector -= camera->GetMovementPlaneSide();
        }

        movementDelta = movmentInputVector.normalized() * moveSpeed;
        new_rot = mesh->get_global_rotation().y + Clamp(rot_speed * delta, 0, abs(theta)) * Sign(theta);
        mesh->set_global_rotation(Vector3(0.0, new_rot, 0.0));
        listener->set_global_rotation(Vector3(0.0, -new_rot, 0.0));

        // Setting the player mesh
        if (roundf(time_passed) > time_passed && anyMovementKeyWasPressed)
        {
            mesh->set_mesh(first_move);
        }
        else if (anyMovementKeyWasPressed)
        {
            mesh->set_mesh(second_move);
        }
        else
        {
            mesh->set_mesh(idle);
        }
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
        this->set_global_position(this->get_global_position() + Vector3(0.0, 1.0, 0.0)); // move the player up 1 unit
    }
}

float Player::WrapDegree(float value)
{
    return -Math_PI + fmod((fmod(value - -Math_PI, Math_PI - -Math_PI) + Math_PI - -Math_PI), Math_PI - -Math_PI);
}

float Player::Clamp(float value, float min, float max)
{
    // Checking if the value is smaller than the lower bound
    if (value < min)
    {
        return min;

        // Checking if value is greater than the upper bound
    }
    else if (value > max)
    {
        return max;
    }
    return value;
}

float Player::Sign(float value)
{
    // Returning sign of the value
    if (value < 0.0)
    {
        return -1.0;
    }
    return 1.0;
}

// Member function that sets a camera position
void Player::SetCameraPosition(Vector3 camera_pos)
{
    camera_position = camera_pos;
}

// Member function that adds a light position and colour to the environment object
void Player::AddLight(Vector3 light_pos, Vector3 light_col, int spec_power)
{

    // Adding light position and colour to the necessary arrays
    light_positions[num_lights] = light_pos;
    light_colours[num_lights] = light_col;
    specular_power[num_lights] = spec_power;
    num_lights++;
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
    camera_position = camera->get_global_position();
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