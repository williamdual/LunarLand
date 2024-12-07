#include "defs.h"
#include "CounterInteractable.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void CounterInteractable::_bind_methods() {

    // Binding lockout signal
    ADD_SIGNAL(MethodInfo("InterUnlock", PropertyInfo(Variant::BOOL, "lock")));

}

CounterInteractable::CounterInteractable() {}

CounterInteractable::CounterInteractable(Player* p, int type, int col_type, bool glow, double rad, int c, int t)
    : Interactable(p, type, col_type, glow, rad) {

    counter = c;
    trigger = t;

	// Instantiating the start playing effect
	this->create_and_add_as_sub_child<AudioStreamPlayer3D>(crank, "StartEffectPlayer", true);

	// Getting the file and setting the stream
    Ref<AudioStreamWAV> effect_stream = ResourceLoader::get_singleton()->load(vformat("%s%s.wav", "SoundFiles/", "Crank"), "AudioStreamWAV");
    crank->set_max_distance(this->GetRadius() * 3);
    crank->set_stream(effect_stream);

}

void CounterInteractable::_enter_tree ( ){
    if(DEBUG) UtilityFunctions::print("Enter Tree - CounterInteractable.");
}

void CounterInteractable::_ready ( ){
    if(DEBUG) UtilityFunctions::print("Ready - CounterInteractable."); 
}

// This member function increments the counter when it is interacted with
void CounterInteractable::Interact() {

	// Rotating the crank mesh
	this->set_global_rotation(this->get_global_rotation() + Vector3(0.0, Math_PI / 2.0, 0.0));

    // Playing the crank noise
	crank_pos = 0.0;
    crank->stop();
    crank->play();

    // Incrementing the counter
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
    emit_signal("InterUnlock", false);
}

// Setter for both initial values
void CounterInteractable::SetInit(int c, int t) {
	// Setting the counter and trigger values
	counter = c;
	trigger = t;
	crank_pos = 0.0;

	// Instantiating the start playing effect
	this->create_and_add_as_sub_child<AudioStreamPlayer3D>(crank, "CrankEffectPlayer", true);

	// Getting the file and setting the stream
    Ref<AudioStreamWAV> effect_stream = ResourceLoader::get_singleton()->load(vformat("%s%s.wav", "SoundFiles/", "Crank"), "AudioStreamWAV");
    crank->set_max_distance(this->GetRadius() * 3);
    crank->set_stream(effect_stream);

	// Connecting audio stream finish functions to reset functions
	crank->connect("finished", Callable(this, "ResetCrankEffectPos"));
}

// Member function to pause audio
void CounterInteractable::PauseAudio() {
	// Getting audio positions
	crank_pos = crank->get_playback_position();

	// Pausing audio
	crank->stop();
}

// Member function to resume audio
void CounterInteractable::ResumeAudio() {
	// Playing audio from current position
	if (crank_pos > 0.0) {
		crank->play(crank_pos);
	}
}

// Member function to reset audio position
void CounterInteractable::ResetCrankEffectPos() {
	crank_pos = 0.0;
}

// Setter for the counter value
void CounterInteractable::SetCounter(int c) {
    counter = c;
}

// Setter for the trigger value
void CounterInteractable::SetTrigger(int t) {
    trigger = t;
}

template <class T>
// returns true if pointer is brand-new; false if retrieved from SceneTree
bool CounterInteractable::create_and_add_as_sub_child(T *&pointer, String name, bool search)
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