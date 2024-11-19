#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/box_shape3d.hpp>
#include <godot_cpp/classes/cylinder_shape3d.hpp>

#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/texture2d.hpp>

#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/vector3.hpp>

#include "player.h"
#include "CameraTrigger.h"

// everything in gdextension is defined in this namespace
namespace godot {

// Enum for better control over what interactable is created
enum InteractableType {
    INTER_OBJECT_COMPUTER_TERMINAL_SCREEN,
    INTERACTABLE_AMOUNT
};

// Enum for hit box type
enum CollisionShapeType {
    SHAPE_NONE,
    SHAPE_BOX,
    SHAPE_CYLINDER,
    SHAPE_AMOUNT
};

class Interactable : public StaticBody3D {
    GDCLASS(Interactable, StaticBody3D);

private:
    double time_passed; // maybe you don't need this, just an example
    double radius;
    bool in_range;
    bool glow_in_range;

    // Shader and shader material material for interactable
    ShaderMaterial* mat;
    Texture2D* tex;
    MeshInstance3D* mesh;

    // Collision values for the interactable
    bool has_col_shape;
    CollisionShape3D* hit_shape;

    // A pointer to the player
    Player* player;

    // Number corresponding to what interactable is created
    int interactable_type;

    // Vectors for light and camera positions and colours as well as the number of lights
    int num_lights;
    TypedArray<Vector3> light_positions;
    TypedArray<Vector3> light_colours;
    TypedArray<int> specular_power;
    Vector3 camera_position;

    // Array of shader names
    char* shader_names[INTERACTABLE_AMOUNT] = {
		"interactable"
	};

    // Array of model names
    char* model_names[INTERACTABLE_AMOUNT] = {
		"ComputerTerminalSceen",
	};

    // Array of texture names
    char* texture_names[INTERACTABLE_AMOUNT] = {
		"ComputerTerminalSceen_Texture",
	};

    char* texture_formats[INTERACTABLE_AMOUNT] = {
		".png",
	};

    // Array of mesh offsets
    Vector3 mesh_offsets[INTERACTABLE_AMOUNT] = {
		Vector3(-6, -0.60, -0.1)
	};

    // Values that darken the texture since the loaded texture can often appear faded
    float tex_darken_values[INTERACTABLE_AMOUNT] = {
        1.0
    };

    // This member function creates a hitbox
    void SetHitBox(void);

    // This member function creates a hit cylinder
    void SetHitCylinder(void);
    
protected:
    static void _bind_methods();

public:
    Interactable();
    Interactable(Player* p, int type, int col_type, bool glow, double rad);
    
    void _enter_tree ( ) override;
    void _ready ( ) override;
    void _process(double delta);

    // Member function that acts as a contructor in the event the default contructor is used
    void SetValues(Player* p, int type, int col_type, bool glow, double rad);

    // Member function that adds a light to the interactable
    void AddLight(Vector3 light_pos, Vector3 light_col, int spec_power);

    // Member function that updates the view position for the interactable
    void SetCameraPosition(Vector3 camera_pos);

    // Member function that registers camera triggers for signal purposes
    void RegisterCameraTrigs(Vector<CameraTrigger*> cam_trigs);

    // Member function that determines if the player is in range of the interactable
    bool IsInRange(void);

    // The function that triggers when an interactable is interacted with
    virtual void Interact();

    // Getters for the member variables
    inline void SetRadius(float rad) { radius = rad; }
    inline double GetRadius() { return radius; }
    inline Player* GetPlayer() { return player; }

    // the return type represents whether it existed already; true if it is brand-new; false if it was retrieved from the SceneTree
	// search defines whether the scenetree should be checked for an instance
	template <class T>
	bool create_and_add_as_child(T *&pointer, String name, bool search = false);
};

}

#endif