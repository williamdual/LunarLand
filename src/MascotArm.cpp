#include "MascotArm.h"

using namespace godot;

void MascotArm::_bind_methods() {
}

// You may not unset this class as top level (from parent)
MascotArm::MascotArm() : MascotPart() { 
    // This is just to line them up for the starting scene -- change this for sure. 
    local_position = Vector3(0.0f, 2.0f, 5.0f);
}

MascotArm::~MascotArm(){
    // Add cleanup here, if you have any. I don't, typically. 
}

void MascotArm::_enter_tree ( ) {
    // if(DEBUG) UtilityFunctions::print("Enter Tree - MascotArm");
    // // Setup Base Geometry using BoxMesh
    // BoxMesh* box = memnew(BoxMesh);
    // box->set_size(Vector3(1.0, 2.0, 1.0));
    // StandardMaterial3D* box_material = memnew(StandardMaterial3D);
    // box_material->set_albedo(Color(0.5294, 0.8078, 0.9216, 1));
    // box->surface_set_material(0, box_material);

    // set_mesh(box);
}

void MascotArm::_process(double delta){
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