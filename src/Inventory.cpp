#include "defs.h"
#include "Inventory.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void Inventory::_bind_methods() {}

Inventory::Inventory() : Node3D() {}

void Inventory::_enter_tree ( ){
    if(DEBUG) UtilityFunctions::print("Enter Tree - Inventory.");
}

void Inventory::_ready ( ){
    if(DEBUG) UtilityFunctions::print("Ready - Inventory."); 
}

void Inventory::_process(double delta) {

    // Situating the inventory slots if the viewport exists
    if (!Engine::get_singleton()->is_editor_hint()) {
        // Only run in the editor, if needed
        Viewport* viewport = get_viewport();
        if (viewport == NULL) {
            UtilityFunctions::print("Error Situating Inventory");
        } else {
            // Setting the position of the inventory if it is not already set
            if (!inventory_set) {
                Size2 viewport_size = viewport->get_visible_rect().size;
                inventory_space->set_global_position(Vector2(viewport_size.x / 2, viewport_size.y - 70));
            }
        }
    }
}

// Member function that sets up inventory
void Inventory::SetupInventory() {

    // Setting up the inventory bar
    Ref<CompressedTexture2D> bar_texture = ResourceLoader::get_singleton()->load("InventorySprites/inventorybar.png", "CompressedTexture2D");
    create_and_add_as_child<Sprite2D>(inventory_space, "Inventory Space", true);
    inventory_space->set_texture(bar_texture);
    //inventory_space->set_scale(Vector2(0.5, 0.5));

    // Setting the chip sprite off the screen
    Ref<CompressedTexture2D> chip_texture = ResourceLoader::get_singleton()->load("InventorySprites/sodachip.png", "CompressedTexture2D");
    create_and_add_as_child<Sprite2D>(chip, "Computer Chip", true);
    chip->set_texture(chip_texture);
    //chip->set_scale(Vector2(0.5, 0.5));
    chip->set_global_position(Vector2(-100, -100));

    // Setting the paper sprites off the screen
    Ref<CompressedTexture2D> paper_texture = ResourceLoader::get_singleton()->load("InventorySprites/healthpapers.png", "CompressedTexture2D");
    create_and_add_as_child<Sprite2D>(papers, "Health Papers", true);
    papers->set_texture(paper_texture);
    //papers->set_scale(Vector2(0.5, 0.5));
    papers->set_global_position(Vector2(-100, -100));

    // Setting the lunar lint sprites off the screen
    Ref<CompressedTexture2D> lint_texture = ResourceLoader::get_singleton()->load("InventorySprites/lunarlint.png", "CompressedTexture2D");
    create_and_add_as_child<Sprite2D>(lint, "Lunar Lint", true);
    lint->set_texture(lint_texture);
    //lint->set_scale(Vector2(0.5, 0.5));
    lint->set_global_position(Vector2(-100, -100));

    // Setting the initial booleans
    has_chip = false;
    has_papers = false;
    has_lint = false;
    inventory_set = false;
}

// This member function picks up an item
void Inventory::PickUpItem(int found) {

    // Setting that the computer chip has been picked up
    if (found == ITEM_CHIP && !has_chip) {
        chip->set_global_position(inventory_space->get_global_position() - Vector2(52.5, 0));
        has_chip = true;

    // Setting that the health papers have been found
    } else if (found == ITEM_PAPERS && !has_papers) {
        papers->set_global_position(inventory_space->get_global_position());
        has_papers = true;

    // Setting that the lunar lint has been found
    } else if (found == ITEM_LINT && !has_lint) {
        lint->set_global_position(inventory_space->get_global_position() + Vector2(52.5, 0));
        has_lint = true;
    }
}

int Inventory::GetCapacity() {

    // Counter variable
    int counter = 0;

    // Increasing the counter for each item found
    if (has_chip) {
        counter++;
    }
    if (has_papers) {
        counter++;
    }
    if (has_lint) {
        counter++;
    }
    return counter;
}

template <class T>
// returns true if pointer is brand-new; false if retrieved from SceneTree
bool Inventory::create_and_add_as_child(T *&pointer, String name, bool search)
{
	// this is the default behaviour
	// added the search parameter so that we can skip the slow "find_child" call during runtime (not applicable to this demo, you should always use search = true until next assignment)
	if (search == false)
	{
		pointer = memnew(T);
		pointer->set_name(name);
		add_child(pointer);
		pointer->set_owner(get_tree()->get_edited_scene_root());
		return true;
	}

	// always only have to search once if we save it here
	Node *child = find_child(name);

	if (child == nullptr)
	{
		pointer = memnew(T);
		pointer->set_name(name);
		add_child(pointer);
		pointer->set_owner(get_tree()->get_edited_scene_root());
		return true;
	}
	else
	{
		pointer = dynamic_cast<T *>(child);
		return false;
	}
}