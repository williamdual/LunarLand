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

    // Playing pick up audio
    pick_up->stop();
    pick_up->play();
}

// Member function that sets the item
void ItemInteractable::SetItem(int lost_item) {
    item = lost_item;

    // Instantiating the start playing effect
	this->create_and_add_as_sub_child<AudioStreamPlayer3D>(pick_up, "PickUpEffectPlayer", true);

	// Getting the file and setting the stream
    Ref<AudioStreamWAV> effect_stream = ResourceLoader::get_singleton()->load(vformat("%s%s.wav", "SoundFiles/", "ItemPickUp"), "AudioStreamWAV");
    pick_up->set_max_distance(this->GetRadius() * 3);
    pick_up->set_stream(effect_stream);
}

template <class T>
// returns true if pointer is brand-new; false if retrieved from SceneTree
bool ItemInteractable::create_and_add_as_sub_child(T *&pointer, String name, bool search)
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