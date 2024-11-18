#include "defs.h"
#include "LockoutInteractable.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void LockoutInteractable::_bind_methods() {

    ClassDB::bind_method(D_METHOD("Lockout"), &LockoutInteractable::Lockout);
    ADD_SIGNAL(MethodInfo("InterLockout", PropertyInfo(Variant::BOOL, "lock")));

}

LockoutInteractable::LockoutInteractable() {}

LockoutInteractable::LockoutInteractable(Player* p, int type, int col_type, bool glow, double rad, int found_item, CounterInteractable* gen)
    : Interactable(p, type, col_type, glow, rad) {

    item = found_item;
    is_locked_out = false;
    stored_radius = rad;
    generator = gen;

}

void LockoutInteractable::_enter_tree ( ){
    if(DEBUG) UtilityFunctions::print("Enter Tree - LockoutInteractable.");
}

void LockoutInteractable::_ready ( ){
    if(DEBUG) UtilityFunctions::print("Ready - LockoutInteractable."); 
}

// This member function deals with an interaction with a lockout interactable
void LockoutInteractable::Interact() {
    
    // Checking to see if interactable is locked out
    UtilityFunctions::print(is_locked_out);
    if (is_locked_out) {
        return;
    }

    // Checking to see if the interactable should lock out or not
    if (item != ITEM_NONE) {
        // Giving the player the item they found
        this->GetPlayer()->GetInventory()->PickUpItem(item);
    }

    // Locking out the interactable
    emit_signal("InterLockout", true);
    Lockout(true);
}

// This member function locks or unlocks the interactable
void LockoutInteractable::Lockout(bool lock) {

    // Checking if the interactable should be locked out
    if (lock) {

        // Locking out the interactable
        is_locked_out = true;
        this->SetRadius(0.0);

    // Unlocking the interactable
    } else {
        is_locked_out = false;
        this->SetRadius(stored_radius);
    }
}

// Member function that sets the lockout state
void LockoutInteractable::SetLockout(int lost_item, CounterInteractable* gen, Vector<LockoutInteractable*> dependents) {

    // Setting initial values
    item = lost_item;
    is_locked_out = false;
    stored_radius = this->GetRadius();
    generator = gen;
    generator->connect("InterUnlock", Callable(this, "Lockout"));
    for (int i = 0; i < dependents.size(); i++) {
        LockoutInteractable* item = dependents[i];
        item->connect("InterLockout", Callable(this, "Lockout"));
    }

}