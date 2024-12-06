#ifndef LOCKOUT_INTERACTABLE_H
#define LOCKOUT_INTERACTABLE_H

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

#include "Interactable.h"
#include "CounterInteractable.h"
#include "Inventory.h"

// everything in gdextension is defined in this namespace
namespace godot {

class LockoutInteractable : public Interactable {
    GDCLASS(LockoutInteractable, Interactable);

private:

    // Indicator on if the interactable is locked out or not
    bool is_locked_out;

    // Saving the interact radius so that it can be reset
    double stored_radius;

    // Item the interactable pertains to
    int item;

    // The generator connected to the interactable
    CounterInteractable* generator;

    // Audio streams for sound effects
    AudioStreamPlayer3D* power_down;
    AudioStreamPlayer3D* power_back;
    AudioStreamPlayer3D* pick_up;
    float power_down_pos;
    float power_back_pos;
    float pick_up_pos;

    // Member functions to reset audio positions
    void ResetPowerDownPos(void);
    void ResetPowerBackPos(void);
    void ResetPickUpPos(void);
    
protected:
    static void _bind_methods();

public:
    LockoutInteractable();
    LockoutInteractable(Player* p, int type, int col_type, bool glow, double rad, int lost_item, CounterInteractable* gen);
    
    void _enter_tree ( ) override;
    void _ready ( ) override;

    // The function that triggers when an interactable is interacted with
    void Interact() override;

    // This member function either starts or ends a lockout
    void Lockout(bool lock);

    // Member function that sets the item
    void SetLockout(int lost_item, CounterInteractable* gen, Vector<LockoutInteractable*> dependents);

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