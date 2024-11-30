#include "MascotPart.h"


using namespace godot;

void MascotPart::_bind_methods() {}

MascotPart::MascotPart() : EnvObject(), parent(nullptr) {
	time_passed = 0.0;

	// primary attributes
	local_position = Vector3();
	local_rotation = Quaternion();

	joint_position = Vector3(0, 0, 0);
	orbit_rotation = Quaternion();

	scale = Vector3(1, 1, 1);

	// this makes it so that any hierarchy established in the node structure does not apply to the hierarchical transformations done by Godot for us, which would interfere
	set_as_top_level(true); // DO NOT CHANGE THIS LINE
}

MascotPart::~MascotPart(){
	// Add cleanup here, if you have any. I don't, typically. 
}

void MascotPart::_enter_tree ( ){
	if(DEBUG) UtilityFunctions::print("Enter Tree - MascotPart."); 
}

// Since most of the MascotParts use the default hierarchical transformation, we can just use the parent class's implementation (this one); the hook will need to be overridden
void MascotPart::_ready ( ){
	if(DEBUG) UtilityFunctions::print("Ready - ", get_name()); // since this will be called by many of the parts

	set_global_transform(get_transformation_matrix());  
}

void MascotPart::set_parent(MascotPart* par){
	this->parent = par;
}

MascotPart* MascotPart::get_parent(){
	return parent;
}

// This function is specifically for use as a parent for hierarchical transformations. 
Transform3D MascotPart::get_transformation_matrix(){	

	Transform3D translation = Transform3D().translated(local_position);
	Transform3D orbit = Transform3D().translated(joint_position) * Transform3D(orbit_rotation) * Transform3D().translated(-1.0 * joint_position); // TODO QUESTION 1: finish implementing orbits
	Transform3D rotation = Transform3D(local_rotation);
	Transform3D scaling = Transform3D().scaled(scale);
	// UtilityFunctions::print(get_name()); 

	Transform3D local_transform = translation * orbit * rotation * scaling;

	return (get_parent() == nullptr)? local_transform : parent->get_transformation_matrix_without_scaling() * local_transform; // for hierarchical transformations, we typically leave scaling off
}

// You usually don't want to encorporate the parent's scaling.
Transform3D MascotPart::get_transformation_matrix_without_scaling(){	
	Transform3D translation = Transform3D().translated(local_position);
	Transform3D orbit = Transform3D().translated(joint_position) * Transform3D(orbit_rotation) * Transform3D().translated(-1.0 * joint_position); // TODO QUESTION 1: finish implementing orbits	
	Transform3D rotation = Transform3D(local_rotation);
	// UtilityFunctions::print(get_name()); 

	Transform3D local_transform = translation * orbit * rotation;

	return (get_parent() == nullptr)? local_transform : parent->get_transformation_matrix_without_scaling() * local_transform; // for hierarchical transformations, we typically leave scaling off
}


#pragma region SIMPLE GETTERS & SETTERS
Vector3 MascotPart::get_local_position(){
	return local_position;
}

void MascotPart::set_local_position(Vector3 input){
	local_position = input;
}

Quaternion MascotPart::get_local_rotation(){
	return local_rotation;
}

void MascotPart::set_local_rotation(Quaternion input){
	local_rotation = input;
}

Vector3 MascotPart::get_scale(){
	return scale;
}

void MascotPart::set_scale(Vector3 input){
	scale = input;
}

Quaternion MascotPart::get_orbit_rotation(){
	return orbit_rotation;
}

void MascotPart::set_orbit_rotation(Quaternion input){
	orbit_rotation = input;
}

Vector3 MascotPart::get_joint_position(){
	return joint_position;
}

void MascotPart::set_joint_position(Vector3 input){
	joint_position = input;
}
#pragma endregion SIMPLE GETTERS & SETTERS

/*
*
* A class which acts as the parent class for all of the crane parts used to build a toy crane. Specifically allows students to do the hierarchical transformations themselves. 
*
* Copyright (c) 2024 Samantha Beilman (samanthabeilman@cmail.carleton.ca)
*
*/