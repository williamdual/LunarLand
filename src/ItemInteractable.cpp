#include "defs.h"
#include "ItemInteractable.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void ItemInteractable::_bind_methods() {}

ItemInteractable::ItemInteractable() {}

ItemInteractable::ItemInteractable(Player* p, int type, int col_type, bool glow, double rad, int found_item)
    : Interactable(p, type, col_type, glow, rad) {

    item = found_item;

}

void ItemInteractable::_enter_tree ( ){
    if(DEBUG) UtilityFunctions::print("Enter Tree - ItemInteractable.");
}

void ItemInteractable::_ready ( ){
    if(DEBUG) UtilityFunctions::print("Ready - ItemInteractable."); 
}

// Nothing in this function it is meant to be overriden
void ItemInteractable::Interact() {
    
    // Giving the player the item they found
    this->GetPlayer()->GetInventory()->PickUpItem(item);
}

// Member function that sets the item
void ItemInteractable::SetItem(int lost_item) {
    item = lost_item;
}