#ifndef COUNTER_INTERACTABLE_H
#define COUNTER_INTERACTABLE_H

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/texture2d.hpp>

#include <godot_cpp/variant/vector3.hpp>

#include "interactable.h"

// everything in gdextension is defined in this namespace
namespace godot {

class CounterInteractable : public Interactable {
    GDCLASS(CounterInteractable, Interactable);

private:

    // Counter member variables
    int counter;
    int trigger;
    
protected:
    static void _bind_methods();

public:
    CounterInteractable();
    CounterInteractable(Player* p, int type, bool glow, double rad, int c, int t);
    
    void _enter_tree ( ) override;
    void _ready ( ) override;

    // This member function sets values for the counter and trigger
    void SetCounter(int c);
    void SetTrigger(int t);

    // The function that triggers when an interactable is interacted with
    void Interact() override;

    // This member function is triggered when
    void Trigger();
};

}

#endif