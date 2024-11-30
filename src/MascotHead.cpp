#include "MascotHead.h"

using namespace godot;

void MascotHead::_bind_methods() {
}

// You may not unset this class as top level (from parent)
MascotHead::MascotHead() : MascotPart() { 
    // This is just to line them up for the starting scene -- change this for sure. 
    local_position = Vector3(0.0f, 4.0f, 0.0f);
}

MascotHead::~MascotHead(){
    // Add cleanup here, if you have any. I don't, typically. 
}

void MascotHead::_enter_tree ( ) {
    if(DEBUG) UtilityFunctions::print("Enter Tree - MascotHead");
    // Setup Base Geometry using SphereMesh
    // SphereMesh* sphere = memnew(SphereMesh);
    // sphere->set_radius(HEAD_RAD);
    // sphere->set_height(HEAD_HEIGHT);
    // StandardMaterial3D* sphere_material = memnew(StandardMaterial3D);
    // sphere_material->set_albedo(Color(0.9961, 0.4412, 0.0942, 1.0));
    // sphere->surface_set_material(0, sphere_material);

    // set_mesh(sphere);
}

void MascotHead::_process(double delta){
    if (Engine::get_singleton()->is_editor_hint()) return; // Early return if we are in editor

    set_global_transform(get_transformation_matrix());  
}

/*
*
* A class derived from JackPart.
*
* Copyright (c) 2024 Samantha Beilman (samanthabeilman@cmail.carleton.ca)
*
*/