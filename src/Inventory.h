#ifndef INVENTORY_H
#define INVENTORY_H

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

#include <godot_cpp/variant/rect2.hpp>		 // for viewport size
#include <godot_cpp/classes/canvas_item.hpp> // for viewport size
#include <godot_cpp/classes/viewport.hpp> // for viewport size

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/compressed_texture2d.hpp>

#include <godot_cpp/variant/vector3.hpp>

// everything in gdextension is defined in this namespace
namespace godot {

// Enum for the items to be found
enum MissingItems {
    ITEM_NONE,
    ITEM_CHIP,
    ITEM_PAPERS,
    ITEM_LINT,
    NUM_MISSING_ITEMS
};

class Inventory : public Node3D {
    GDCLASS(Inventory, Node3D);

private:
    // Booleans for items contained in the inventory
    bool has_chip;
    bool has_papers;
    bool has_lint;

    // Condition to set the coordinates of the inventory
    bool inventory_set;

    // Sprites that make up the inventory
    Sprite2D* inventory_space;
    Sprite2D* chip;
    Sprite2D* papers;
    Sprite2D* lint;
    
protected:
    static void _bind_methods();

public:
    Inventory();
    
    void _enter_tree ( ) override;
    void _ready ( ) override;
    void _process(double delta);

    // Member function that sets up the inventory
    void SetupInventory(void);

    // Member function that places an item in the inventory
    void PickUpItem(int found);

    // Member function that returns how many items in the inventory
    int GetCapacity(void);

    // the return type represents whether it existed already; true if it is brand-new; false if it was retrieved from the SceneTree
	// search defines whether the scenetree should be checked for an instance
	template <class T>
	bool create_and_add_as_child(T *&pointer, String name, bool search = false);
};

}

#endif