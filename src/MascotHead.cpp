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