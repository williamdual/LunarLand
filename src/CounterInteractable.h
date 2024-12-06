#ifndef COUNTER_INTERACTABLE_H
#define COUNTER_INTERACTABLE_H

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
#include <godot_cpp/classes/audio_stream_wav.hpp>

#include <godot_cpp/variant/vector3.hpp>

#include "interactable.h"

// everything in gdextension is defined in this namespace
namespace godot {

class CounterInteractable : public Interactable {
    GDCLASS(CounterInteractable, Interactable);

private:

    // Counter member variables
    int counter;
    int trigger;

    // Audio player for sound effects
    AudioStreamPlayer3D* crank;
    float crank_pos;

    // Member functions to reset audio positions
    void ResetCrankEffectPos(void);
    
protected:
    static void _bind_methods();

public:
    CounterInteractable();
    CounterInteractable(Player* p, int type, int col_type, bool glow, double rad, int c, int t);
    
    void _enter_tree ( ) override;
    void _ready ( ) override;

    // This member function sets values for the counter, trigger, audio
    void SetInit(int c, int t);
    void SetCounter(int c);
    void SetTrigger(int t);

    // The function that triggers when an interactable is interacted with
    void Interact() override;

    // This member function is triggered when
    void Trigger();

    // Functions to pause and resume all audio
    void PauseAudio(void);
    void ResumeAudio(void);

    // the return type represents whether it existed already; true if it is brand-new; false if it was retrieved from the SceneTree
	// search defines whether the scenetree should be checked for an instance
	template <class T>
	bool create_and_add_as_sub_child(T *&pointer, String name, bool search = false);
};

}

#endif