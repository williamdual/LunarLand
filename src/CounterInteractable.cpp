#include "defs.h"
#include "CounterInteractable.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void CounterInteractable::_bind_methods() {}

CounterInteractable::CounterInteractable() {}

CounterInteractable::CounterInteractable(Player* p, int type, bool glow, double rad, int c, int t)
    : Interactable(p, type, glow, rad) {

    counter = c;
    trigger = t;

}

void CounterInteractable::_enter_tree ( ){
    if(DEBUG) UtilityFunctions::print("Enter Tree - CounterInteractable.");
}

void CounterInteractable::_ready ( ){
    if(DEBUG) UtilityFunctions::print("Ready - CounterInteractable."); 
}

// This member function increments the counter when it is interacted with
void CounterInteractable::Interact() {
    // Incrementing the counter
    UtilityFunctions::print("Incremented");
    counter++;

    // Checking if the right value has been reached
    if (counter == trigger) {
        Trigger();
    }
}

// This member function does something when the counter reaches a certain value
void CounterInteractable::Trigger() {
    UtilityFunctions::print("Made It");
    trigger *= 10;
}

// Setter for the counter value
void CounterInteractable::SetCounter(int c) {
    counter = c;
}

// Setter for the trigger value
void CounterInteractable::SetTrigger(int t) {
    trigger = t;
}