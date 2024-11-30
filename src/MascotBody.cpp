#include "MascotBody.h"

using namespace godot;

void MascotBody::_bind_methods() {
}

// You may not unset this class as top level (from parent)
MascotBody::MascotBody() : MascotPart() { 
    // This is just to line them up for the starting scene -- change this for sure. 
    local_position = Vector3(0.0f, 2.25f, 0.0f);
}

MascotBody::~MascotBody(){
    // Add cleanup here, if you have any. I don't, typically. 
}

void MascotBody::_enter_tree ( ) {
    if(DEBUG) UtilityFunctions::print("Enter Tree - MascotBody");
    // Setup Base Geometry using CylinderMesh
    // CylinderMesh* cylinder = memnew(CylinderMesh);
    // cylinder->set_bottom_radius(BODY_RAD);
    // cylinder->set_top_radius(BODY_RAD);
    // cylinder->set_height(BODY_HEIGHT);

    // StandardMaterial3D* material = memnew(StandardMaterial3D);
    // material->set_albedo(Color(0.5294, 0.8078, 0.9216, 1));
    // cylinder->surface_set_material(0, material);

    // set_mesh(cylinder);
}

void MascotBody::_process(double delta){
    if (Engine::get_singleton()->is_editor_hint()) return; // Early return if we are in editor

    set_global_transform(get_transformation_matrix());  
}

/*
*
* A class derived from MascotPart.
*
* Copyright (c) 2024 Samantha Beilman (samanthabeilman@cmail.carleton.ca)
*
*/