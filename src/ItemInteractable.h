#ifndef ITEM_INTERACTABLE_H
#define ITEM_INTERACTABLE_H

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

#include "Interactable.h"
#include "Inventory.h"

// everything in gdextension is defined in this namespace
namespace godot {

class ItemInteractable : public Interactable {
    GDCLASS(ItemInteractable, Interactable);

private:

    // Item the interactable pertains to
    int item;
    
protected:
    static void _bind_methods();

public:
    ItemInteractable();
    ItemInteractable(Player* p, int type, bool glow, double rad, int lost_item);
    
    void _enter_tree ( ) override;
    void _ready ( ) override;

    // The function that triggers when an interactable is interacted with
    void Interact() override;

    // Member function that sets the item
    void SetItem(int lost_item);

};

}

#endif