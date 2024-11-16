#ifndef AUDIO_INTERACTABLE_H
#define AUDIO_INTERACTABLE_H

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
#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/audio_listener3d.hpp>
#include <godot_cpp/classes/audio_stream_ogg_vorbis.hpp>

#include <godot_cpp/variant/vector3.hpp>

#include "interactable.h"

// everything in gdextension is defined in this namespace
namespace godot {

// Enum for easily selecting audio files
enum AudioFile {
    AUDIO_SUSIE_OPENING,
    AUDIO_SUSIE_PAPERS,
    AUDIO_SUSIE_CHIP,
    AUDIO_SUSIE_LINT,
    NUM_AUDIO_FILES
};

class AudioInteractable : public Interactable {
    GDCLASS(AudioInteractable, Interactable);

private:

    // Audio member variables
    AudioStreamPlayer3D* audio;
    int file_num;

    // Array of audio file names
    char* audio_names[NUM_AUDIO_FILES] = {
		"susie1",
        "susie2",
        "susie3",
        "susie4"
	};
    
protected:
    static void _bind_methods();

public:
    AudioInteractable();
    AudioInteractable(Player* p, int type, int col_type, bool glow, double rad, int file);
    
    void _enter_tree ( ) override;
    void _ready ( ) override;

    // Member function for setting the audio
    void SetAudio(int file);

    // The function that triggers when an interactable is interacted with
    void Interact() override;

    // the return type represents whether it existed already; true if it is brand-new; false if it was retrieved from the SceneTree
	// search defines whether the scenetree should be checked for an instance
	template <class T>
	bool create_and_add_as_sub_child(T *&pointer, String name, bool search = false);

};

}

#endif