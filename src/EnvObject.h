#ifndef ENVOBJECT_H
#define ENVOBJECT_H

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/box_shape3d.hpp>
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/capsule_mesh.hpp>
#include <godot_cpp/classes/cylinder_shape3d.hpp>

#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/texture2d.hpp>

#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/vector3.hpp>

#include "Interactable.h"
#include "CameraTrigger.h"

// everything in gdextension is defined in this namespace
namespace godot {

// Enum for better control over what environment object is created
enum EnvObjectType {
    ENV_OBJECT_ALIEN,
    ENV_OBJECT_CAFETERIA_TABLE,
    ENV_OBJECT_COMPUTER_TERMINAL,
    ENV_OBJECT_DART_BOARD,
    ENV_OBJECT_DARTS,
    ENV_OBJECT_FILE_CABINET,
    ENV_OBJECT_LOST_AND_FOUND,
    ENV_OBJECT_MECHANICAL_BULL,
    ENV_OBJECT_MENU_BOARD,
    ENV_OBJECT_MICROPHONE,
    ENV_OBJECT_PASTA_WORD_BIN,
    ENV_OBJECT_SMALL_STAGE,
    ENV_OBJECT_AMOUNT
};

// Enum that contains options for primitive mesh shapes
enum PrimShapeType {
    PRIM_BOX,
    PRIM_CAPSULE,
    PRIM_SIZE
};

class EnvObject : public StaticBody3D {
    GDCLASS(EnvObject, StaticBody3D);

private:
    double time_passed; // maybe you don't need this, just an example

    // Shader and shader material material for environment object
    Texture2D* tex;

    // Collision values for the environment object
    bool has_col_shape;

    // Array of shader names
    char* shader_names[ENV_OBJECT_AMOUNT] = {
		"envobj"
	};

    // Array of model names
    char* model_names[ENV_OBJECT_AMOUNT] = {
        "Alian",
        "CafeteriaTable",
        "ComputerTerminal",
        "DartBoard",
        "DartBoardDarts",
		"FileCabinet",
        "LostAndFoundBin",
        "MechanicalBull",
        "Menu",
        "Microphone",
        "PastaWordBin",
        "SmallStage"
	};

    // Array of texture names
    char* texture_names[ENV_OBJECT_AMOUNT] = {
        "Alian_Texture",
        "CafeteriaTable_Texture",
        "ComputerTerminal_Texture",
        "DartBoard_Texture",
        "DartBoard_Texture",
		"FileCabinet_Texture",
        "LostAndFoundBin_Texture",
        "MechanicalBull_Texture",
        "MenuBoard_Texture",
        "Microphone_Texture",
        "PastaWordBin_Texture",
        "SmallStage_Texture"
	};

    // Array of texture formats for each texture file
    char* texture_formats[ENV_OBJECT_AMOUNT] = {
		".jpg", // Alien
        ".png", // Caf Table
        ".png", // Computer Terminal
        ".png", // Dart Board
        ".png", // Darts
        ".png", // File Cabinet
        ".png", // Lost and Found Bin
        ".png", // Mechanical Bull
        ".png", // Menu Board
        ".jpg", // Microphone
        ".png", // Pasta Word Bin
        ".png"  // Small Stage
	};

    // Array of mesh offsets
    Vector3 mesh_offsets[ENV_OBJECT_AMOUNT] = {
		Vector3(0.0, -0.25, 0.2), // Alien
        Vector3(0.0, -1.0, 0.0), // Caf Table
        Vector3(-6.0, -0.4, 0.0), // Computer Terminal
        Vector3(0.0, 0.0, 0.0), // Dart Board
        Vector3(0.1, 0.0, -0.5), // Darts
        Vector3(0.0, -1.5, 0.0), // File Cabinet
        Vector3(0.0, -0.8, 0.0), // Lost and Found Bin
        Vector3(0.0, -2.3, -1.0), // Mechanical Bull
        Vector3(0.0, -2.0, 0.0), // Menu Board
        Vector3(0.0, -1.3, 0.0), // Microphone
        Vector3(0.0, -0.9, 0.0), // Pasta Word Bin
        Vector3(0.0, -0.75, 0.0) // Small Stage
	};

    // Values that darken the texture since the loaded texture can often appear faded
    float tex_darken_values[ENV_OBJECT_AMOUNT] = {
        0.8, // Alien
        0.7, // Caf Table
        0.7, // Computer Terminal
        0.8, // Dart Board
        0.8, // Darts
        0.7, // File Cabinet
        0.8, // Lost and Found Bin
        0.8, // Mechanical Bull
        0.8, // Menu Board
        0.2, // Microphone
        0.8, // Pasta Word Bin
        0.2 // Small Stage
    };

    // This member function creates a hitbox
    void SetHitBox(void);

    // This member function creates a hit cylinder
    void SetHitCylinder(void);
    
protected:
    static void _bind_methods();

    // Mesh and hit shape
    MeshInstance3D* mesh;
    CollisionShape3D* hit_shape;

    // Shader material
    ShaderMaterial* mat;

    // Vectors for light and camera positions and colours as well as the number of lights
    int num_lights;
    TypedArray<Vector3> light_positions;
    TypedArray<Vector3> light_colours;
    TypedArray<int> specular_power;
    Vector3 camera_position;

public:
    EnvObject();
    
    void _enter_tree ( ) override;
    void _ready ( ) override;
    void _process(double delta);

    // Member function that adds a light to the environment object
    void AddLight(Vector3 light_pos, Vector3 light_col, int spec_power);

    // Member function that updates the view position for the environment object
    void SetCameraPosition(Vector3 camera_pos);

    // Member function that registers camera triggers for signal purposes
    void RegisterCameraTrigs(Vector<CameraTrigger*> cam_trigs);

    // Member function that sets initial values for the environment object
    void SetValues(int obj_type, int col_type);

    // Setter for more primitive mesh values
    void SetPrimValues(int prim_type, int col_type);

    // the return type represents whether it existed already; true if it is brand-new; false if it was retrieved from the SceneTree
	// search defines whether the scenetree should be checked for an instance
	template <class T>
	bool create_and_add_as_child(T *&pointer, String name, bool search = false);
};

}

#endif