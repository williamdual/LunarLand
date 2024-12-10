#include "Mascot.h"
#include <godot_cpp/core/memory.hpp> // for memnew
#include <godot_cpp/classes/scene_tree.hpp> // for root
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void Mascot::_bind_methods() {
	
}

Mascot::Mascot() : Node3D() {
    time_passed = 0.0;
}

Mascot::~Mascot() {
	// Add cleanup here, if you have any. I don't, typically. 
}

// Adds the mascot parts. 
// the add_child and set_owner should happen in _enter_tree, or we will not see them in the editor
void Mascot::_enter_tree ( ){
	if(DEBUG) UtilityFunctions::print("Entering Tree - Mascot.");

    // Creating each of the body parts
    create_and_add_as_child<MascotBody>(body, "Body", true);
    create_and_add_as_child<MascotHead>(head, "Head", true);
    create_and_add_as_child<MascotArm>(upper_left, "UpperLeftArm", true);
    create_and_add_as_child<MascotArm>(upper_right, "UpperRightArm", true);
    create_and_add_as_child<MascotArm>(lower_left, "LowerLeftArm", true);
    create_and_add_as_child<MascotArm>(lower_right, "LowerRightArm", true);
    create_particle_system("Repulsors", "hover");

	// The last (highest) spring loop is the parent of the body
	//body->set_parent(spring_loops[spring_loops.size() - 1]);
    head->set_parent(body);
    upper_left->set_parent(body);
    upper_right->set_parent(body);
    lower_left->set_parent(upper_left);
    lower_right->set_parent(upper_right);

    // Setting the primitive values of the body
    body->SetPrimValues(PRIM_CAPSULE, ENV_OBJECT_ALIEN, SHAPE_CYLINDER);
    head->SetValues(ENV_OBJECT_ALIEN, SHAPE_BOX);
    upper_left->SetPrimValues(PRIM_BOX, ENV_OBJECT_ALIEN, SHAPE_BOX);
    upper_right->SetPrimValues(PRIM_BOX, ENV_OBJECT_ALIEN, SHAPE_BOX);
    lower_left->SetPrimValues(PRIM_BOX, ENV_OBJECT_ALIEN, SHAPE_BOX);
    lower_right->SetPrimValues(PRIM_BOX, ENV_OBJECT_ALIEN, SHAPE_BOX);
}

void Mascot::_ready(){
	if(DEBUG) UtilityFunctions::print("Ready - Mascot.");

	// Setting the body and head positions relative to their parents
    body->set_local_position(Vector3(159.58, 1.75, -15.260));
    body->set_global_transform(body->get_transformation_matrix_without_scaling());
    head->set_local_position(Vector3(0.0f, 1.6f, 0.0f));
	head->set_global_transform(head->get_transformation_matrix_without_scaling());

    // Setting arm positions

	// Setting the upper right arm local position and position relative to parent
	upper_right->set_local_position(Vector3(-0.25, 0.0, 0.0));
	upper_right->set_local_rotation(Vector3(0.0, 0.0, Math_PI));
	upper_right->set_joint_position(Vector3(-0.5, 1.0, 0.0));
	upper_right->set_orbit_rotation(Quaternion(0.0, 0.0, -0.5, 1.0));
	upper_right->set_global_transform(upper_right->get_transformation_matrix_without_scaling());

	// Setting the lower right arm local position and position relative to parent
	lower_right->set_local_position(Vector3(0.0, -0.75, 0.0));
	lower_right->set_joint_position(Vector3(0.0, 0.5, 0.0));
	lower_right->set_orbit_rotation(Quaternion(0.0, 0.0, 0.25, 1.0));
	lower_right->set_global_transform(lower_right->get_transformation_matrix_without_scaling());

	// Setting the upper left arm local position and position relative to parent
	upper_left->set_local_position(Vector3(0.5, 0.5, 0.0));
	upper_left->set_joint_position(Vector3(0.0, 0.5, 0.0));
	upper_left->set_orbit_rotation(Quaternion(0.0, 0.0, 0.5, 1.0));
	upper_left->set_global_transform(upper_left->get_transformation_matrix_without_scaling());

	// Setting the lower left arm local position and position relative to parent
	lower_left->set_local_position(Vector3(0.0, -0.75, 0.0));
	lower_left->set_joint_position(Vector3(0.0, 0.5, 0.0));
	lower_left->set_orbit_rotation(Quaternion(0.0, 0.0, -0.25, 1.0));
	lower_left->set_global_transform(lower_left->get_transformation_matrix_without_scaling());

    // Setting up particle system
    GPUParticles3D* particle_system = particle_systems[0];
	ShaderMaterial* shader_material = dynamic_cast<ShaderMaterial*>(*particle_system->get_draw_pass_mesh(0)->surface_get_material(0));
	particle_system->set_amount(20000);
	shader_material->set_shader_parameter("texture_image", ResourceLoader::get_singleton()->load("res://Textures/flame4x4orig.png"));
    particle_system->set_scale(Vector3(0.1, 0.1, 0.1));
	particle_system->set_global_position(body->get_local_position() - Vector3(0.0, 1.5, 0.0));
}

void Mascot::_process(double delta){
    if (Engine::get_singleton()->is_editor_hint()) return; // Early return if we are in editor

    // Adding time
    time_passed += delta;

    // Waving the arm
    upper_right->set_orbit_rotation(Quaternion(0.0, 0.0, 0.9 * sin(2.0 * time_passed) - 1.5, 1.0));

    // Adjusting particle system position
    particle_systems[0]->set_global_position(body->get_local_position() - Vector3(0.0, 1.5, 0.0));
}

// Member function that adds a light to the environment object
void Mascot::AddLight(Vector3 light_pos, Vector3 light_col, int spec_power) {
    body->AddLight(light_pos, light_col, spec_power);
    head->AddLight(light_pos, light_col, spec_power);
    upper_left->AddLight(light_pos, light_col, spec_power);
    upper_right->AddLight(light_pos, light_col, spec_power);
    lower_left->AddLight(light_pos, light_col, spec_power);
    lower_right->AddLight(light_pos, light_col, spec_power);
}

// Member function that updates the view position for the environment object
void Mascot::SetCameraPosition(Vector3 camera_pos) {
    body->SetCameraPosition(camera_pos);
    head->SetCameraPosition(camera_pos);
    upper_left->SetCameraPosition(camera_pos);
    upper_right->SetCameraPosition(camera_pos);
    lower_left->SetCameraPosition(camera_pos);
    lower_right->SetCameraPosition(camera_pos);
}

// Member function that registers camera triggers for signal purposes
void Mascot::RegisterCameraTrigs(Vector<CameraTrigger*> cam_trigs) {
    body->RegisterCameraTrigs(cam_trigs);
    head->RegisterCameraTrigs(cam_trigs);
    upper_left->RegisterCameraTrigs(cam_trigs);
    upper_right->RegisterCameraTrigs(cam_trigs);
    lower_left->RegisterCameraTrigs(cam_trigs);
    lower_right->RegisterCameraTrigs(cam_trigs);
}

// Member function that sets global position
void Mascot::SetPosition(Vector3 pos) {
    this->set_global_position(pos);
    body->set_local_position(pos);
}

// Member function that sets the global rotation
void Mascot::SetRotation(Vector3 rot) {
    this->set_global_rotation(rot);
    body->set_local_rotation(rot);
}

// it felt a bit cleaner in my eyes to bundle this together
// not full file name for the shader; see the particle system code for more detail
void Mascot::create_particle_system(String node_name, String shader_name)
{
	// if you want to use non-zero argument constructors, here is an example of how to do that
	ParticleSystem3501 *system = memnew(ParticleSystem3501(shader_name));
	add_as_child(system, node_name, true);
	particle_systems.push_back(system);
}

template <class T>
// returns true if pointer is brand-new; false if retrieved from SceneTree
bool Mascot::create_and_add_as_child(T* &pointer, String name, bool search){
	// this is the default behaviour
	// added the search parameter so that we can skip the slow "find_child" call during runtime (not applicable to this demo, you should always use search = true until next assignment)
	if(search == false){
		pointer = memnew(T);
		pointer->set_name(name);
		add_child(pointer);
		pointer->set_owner(get_tree()->get_edited_scene_root());
		return true;
	}

	// always only have to search once if we save it here
	Node* child = find_child(name);
	
	if(child == nullptr){
		pointer = memnew(T);
		pointer->set_name(name);
		add_child(pointer);
		pointer->set_owner(get_tree()->get_edited_scene_root());
		return true;
	}
	else{
		pointer = dynamic_cast<T*>(child);
		return false;
	}
}

template <class T>
// there is only one difference from the usual
bool Mascot::create_and_add_as_child_of_node(T* &pointer, String name, Node* parent, bool search){
    // this is the default behaviour
    if(search == false){
        pointer = memnew(T);
        pointer->set_name(name);
        parent->add_child(pointer);
        pointer->set_owner(get_tree()->get_edited_scene_root());
        return true;
    }

    // always only have to search once if we save it here
    Node* child = find_child(name);

    if(child == nullptr){
        pointer = memnew(T);
        pointer->set_name(name);
        parent->add_child(pointer);
        pointer->set_owner(get_tree()->get_edited_scene_root());
        return true;
    }
    else{
        pointer = dynamic_cast<T*>(child);
        return false;
    }
}

// This is a variant of the usual one. It allows you to more easily use a non-zero argument constructor, which I noticed some of you have struggled with. Hope this helps!
// returns true if pointer is brand-new; false if retrieved from SceneTree
// deletes the memory if the node exists in the scenetree and isn't null when passed in
// IMPORTANT: IF SEARCH IS FALSE, IT ASSUMES THAT THE POINTER IS TO A VALID INSTANCE ALREADY AKA MEMNEW HAS ALREADY BEEN CALLED
template <class T>
bool Mascot::add_as_child(T *&pointer, String name, bool search)
{
	// this is the default behaviour
	// added the search parameter so that we can skip the slow "find_child" call during runtime
	if (search == false)
	{
		pointer->set_name(name);
		add_child(pointer);
		pointer->set_owner(get_tree()->get_edited_scene_root());
		return true;
	}

	// always only have to search once if we save it here
	Node *child = find_child(name);

	// if the node hasn't been added to the SceneTree yet
	if (child == nullptr)
	{
		pointer->set_name(name);
		add_child(pointer);
		pointer->set_owner(get_tree()->get_edited_scene_root());
		return true;
	}
	// if we are grabbing the existent one, clean up the memory to the new one that was just made and passed as an argument
	else
	{
		if (pointer == nullptr)
		{
			UtilityFunctions::print("There is a nullptr being passed to add_as_child...");
		}
		else
		{
			memdelete(pointer);
		}
		pointer = dynamic_cast<T *>(child);
		return false;
	}
}

/*
*
* A container class which sets up a LunarLand Mascot. 
*
* Copyright (c) 2024 Samantha Beilman (samanthabeilman@cmail.carleton.ca)
*
*/