#include "MascotArm.h"

using namespace godot;

void MascotArm::_bind_methods() {
}

// You may not unset this class as top level (from parent)
MascotArm::MascotArm() : MascotPart() { 
    // This is just to line them up for the starting scene -- change this for sure. 
    local_position = Vector3(0.0f, 0.5f, 0.5f);
    scale = Vector3(0.5, 0.75, 0.5);
    time_passed = 0.0;
}

MascotArm::~MascotArm(){
    // Add cleanup here, if you have any. I don't, typically. 
}

void MascotArm::_enter_tree ( ) {}

void MascotArm::_process(double delta){
    if (Engine::get_singleton()->is_editor_hint()) return; // Early return if we are in editor

    // Bobbing up and down
    time_passed += delta;

    set_global_transform(get_transformation_matrix());

    // Sending the necessary values to the shader
    mat->set_shader_parameter("init_light_positions", light_positions);
    mat->set_shader_parameter("light_colours", light_colours);
    mat->set_shader_parameter("view_pos", camera_position);
    mat->set_shader_parameter("spec_power", specular_power);
    mat->set_shader_parameter("num_lights", num_lights);  
}

/*
*
* A class derived from JackPart.
*
* Copyright (c) 2024 Samantha Beilman (samanthabeilman@cmail.carleton.ca)
*
*/