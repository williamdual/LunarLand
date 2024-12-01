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

        // Playing pick up sound effect
        power_down->stop();
        power_back->stop();
        pick_up->play();
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

        // Playing the power up sound
        power_back->stop();
        power_down->play();

    // Unlocking the interactable
    } else {
        is_locked_out = false;
        this->SetRadius(stored_radius);

        // Playing the power down sound
        power_down->stop();
        power_back->play();
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

    // Instantiating both sound effects
	this->create_and_add_as_sub_child<AudioStreamPlayer3D>(power_down, "PowerDownEffectPlayer", true);
    this->create_and_add_as_sub_child<AudioStreamPlayer3D>(power_back, "PowerBackEffectPlayer", true);
    this->create_and_add_as_sub_child<AudioStreamPlayer3D>(pick_up, "PickUpEffectPlayer", true);

	// Getting the file and setting the stream
    Ref<AudioStreamWAV> power_down_stream = ResourceLoader::get_singleton()->load(vformat("%s%s.wav", "SoundFiles/", "PowerDown"), "AudioStreamWAV");
    power_down->set_max_distance(this->GetRadius() * 6);
    power_down->set_stream(power_down_stream);

    Ref<AudioStreamWAV> power_back_stream = ResourceLoader::get_singleton()->load(vformat("%s%s.wav", "SoundFiles/", "PowerRestored"), "AudioStreamWAV");
    power_back->set_max_distance(this->GetRadius() * 6);
    power_back->set_stream(power_back_stream);

    Ref<AudioStreamWAV> pick_up_stream = ResourceLoader::get_singleton()->load(vformat("%s%s.wav", "SoundFiles/", "ItemPickUp"), "AudioStreamWAV");
    pick_up->set_max_distance(this->GetRadius() * 6);
    pick_up->set_stream(pick_up_stream);
}

template <class T>
// returns true if pointer is brand-new; false if retrieved from SceneTree
bool LockoutInteractable::create_and_add_as_sub_child(T *&pointer, String name, bool search)
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