#include "Mascot.h"
#include <godot_cpp/core/memory.hpp> // for memnew
#include <godot_cpp/classes/scene_tree.hpp> // for root
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void Mascot::_bind_methods() {
	
}

Mascot::Mascot() : Node3D() {}

Mascot::~Mascot() {
	// Add cleanup here, if you have any. I don't, typically. 
}

// Adds the mascot parts. 
// the add_child and set_owner should happen in _enter_tree, or we will not see them in the editor
void Mascot::_enter_tree ( ){
	if(DEBUG) UtilityFunctions::print("Entering Tree - Mascot.");

    // Creating each of the body parts
    create_and_add_as_child<MascotBody>(body, "MascotBody", true);

	// The last (highest) spring loop is the parent of the body
	//body->set_parent(spring_loops[spring_loops.size() - 1]);

    // Setting the primitive values of the body
    body->SetPrimValues(PRIM_BOX, SHAPE_BOX);
}

void Mascot::_ready(){
	if(DEBUG) UtilityFunctions::print("Ready - Mascot.");

	// Setting the body and head positions relative to their parents
	// body->set_global_transform(body->get_transformation_matrix_without_scaling());
	// head->set_global_transform(head->get_transformation_matrix_without_scaling());

	// // Setting the eye local positions and positions relative to parents
	// left_eye->set_local_position(Vector3(1.0, 0.0, 2.0));
	// right_eye->set_local_position(Vector3(-1.0, 0.0, 2.0));
	// left_eye->set_global_transform(left_eye->get_transformation_matrix_without_scaling());
	// right_eye->set_global_transform(right_eye->get_transformation_matrix_without_scaling());
}

void Mascot::_process(double delta){
    if (Engine::get_singleton()->is_editor_hint()) return; // Early return if we are in editor
}

// Member function that adds a light to the environment object
void Mascot::AddLight(Vector3 light_pos, Vector3 light_col, int spec_power) {
    body->AddLight(light_pos, light_col, spec_power);
}

// Member function that updates the view position for the environment object
void Mascot::SetCameraPosition(Vector3 camera_pos) {
    body->SetCameraPosition(camera_pos);
}

// Member function that registers camera triggers for signal purposes
void Mascot::RegisterCameraTrigs(Vector<CameraTrigger*> cam_trigs) {
    body->RegisterCameraTrigs(cam_trigs);
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

/*
*
* A container class which sets up a LunarLand Mascot. 
*
* Copyright (c) 2024 Samantha Beilman (samanthabeilman@cmail.carleton.ca)
*
*/