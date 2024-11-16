#include "defs.h"
#include "AudioInteractable.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void AudioInteractable::_bind_methods() {}

AudioInteractable::AudioInteractable() {}

AudioInteractable::AudioInteractable(Player* p, int type, int col_type, bool glow, double rad, int file)
    : Interactable(p, type, col_type, glow, rad) {

    SetAudio(file);

}

void AudioInteractable::_enter_tree ( ){
    if(DEBUG) UtilityFunctions::print("Enter Tree - AudioInteractable.");
}

void AudioInteractable::_ready ( ){
    if(DEBUG) UtilityFunctions::print("Ready - AudioInteractable."); 
}

// Member function that plays an audio clip
void AudioInteractable::Interact() {

    // Stopping the stream in case it is playing
    audio->stop();

    // Starting the stream again
    audio->play();
}

// Member function for setting the audio
void AudioInteractable::SetAudio(int file) {
    // Setting the given values
    file_num = file;

    // Instantiating stream player and listener
    this->create_and_add_as_sub_child<AudioStreamPlayer3D>(audio, "StreamPlayer", true);

    // Getting the file and setting the stream
    Ref<AudioStreamOggVorbis> stream = ResourceLoader::get_singleton()->load(vformat("%s%s.ogg", "SoundFiles/", audio_names[file_num]), "AudioStreamOggVorbis");
    audio->set_max_distance(this->GetRadius() * 3);
    audio->set_stream(stream);
}

template <class T>
// returns true if pointer is brand-new; false if retrieved from SceneTree
bool AudioInteractable::create_and_add_as_sub_child(T *&pointer, String name, bool search)
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