#ifndef INTERACTABLE_H
#define INTERACTABLE_H

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

#include <godot_cpp/variant/vector3.hpp>

#include "player.h"

// everything in gdextension is defined in this namespace
namespace godot {

// Enum for better control over what interactable is created
enum InteractableType {
    OBJECT_COMPUTER_SCREEN,
    INTERACTABLE_AMOUNT
};

class Interactable : public Node3D {
    GDCLASS(Interactable, Node3D);

private:
    double time_passed; // maybe you don't need this, just an example
    double radius;
    bool in_range;
    bool glow_in_range;

    // Shader and shader material material for interactable
    ShaderMaterial* mat;
    Texture2D* tex;
    MeshInstance3D* mesh;

    // A pointer to the player
    Player* player;

    // Number corresponding to what interactable is created
    int interactable_type;

    // Array of shader names
    char* shader_names[INTERACTABLE_AMOUNT] = {
		"interactable"
	};

    // Array of model names
    char* model_names[INTERACTABLE_AMOUNT] = {
		"ComputerTerminalSceen"
	};

    // Array of texture names
    char* texture_names[INTERACTABLE_AMOUNT] = {
		"ComputerTerminalSceen_Texture"
	};

    // Array of mesh offsets
    Vector3 mesh_offsets[INTERACTABLE_AMOUNT] = {
		Vector3(-6, -0.5, 0)
	};
    
protected:
    static void _bind_methods();

public:
    Interactable();
    Interactable(Player* p, int type, bool glow, double rad);
    
    void _enter_tree ( ) override;
    void _ready ( ) override;
    void _process(double delta);

    // Member function that acts as a contructor in the event the default contructor is used
    void SetValues(Player* p, int type, bool glow, double rad);

    // Member function that determines if the player is in range of the interactable
    bool IsInRange(void);

    // The function that triggers when an interactable is interacted with
    virtual void Interact();

    // Getters for the member variables
    inline double GetRadius() { return radius; }
    inline Player* GetPlayer() { return player; }

    // the return type represents whether it existed already; true if it is brand-new; false if it was retrieved from the SceneTree
	// search defines whether the scenetree should be checked for an instance
	template <class T>
	bool create_and_add_as_child(T *&pointer, String name, bool search = false);
};

}

#endif