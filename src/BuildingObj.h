#ifndef BUILDINGOBJ_H
#define BUILDINGOBJ_H

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/box_shape3d.hpp>
#include <godot_cpp/classes/cylinder_shape3d.hpp>

#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/texture2d.hpp>

#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/vector3.hpp>

#include "CameraTrigger.h"

// everything in gdextension is defined in this namespace
namespace godot
{

    // Enum for better control over what building object is created
    enum BuildObjType
    {
        BUILDING_TEST_BUILDING, // test level
        BUILDING_MAP_BUILDING,  // real level
        BUILDING_OBJECT_AMOUNT
    };

    class BuildingObj : public Node3D
    {
        GDCLASS(BuildingObj, Node3D);

    private:
        double time_passed; // maybe you don't need this, just an example

        // Shader and shader material material for building object
        ShaderMaterial *mat;
        Texture2D *tex;
        MeshInstance3D *mesh;

        // Collision mesh for if the mesh is manually set
        MeshInstance3D *col_mesh;

        // Indicator for if the building is textured
        bool textured;

        // Vectors for light and camera positions and colours as well as the number of lights
        int num_lights;
        TypedArray<Vector3> light_positions;
        TypedArray<Vector3> light_colours;
        TypedArray<int> specular_power;
        Vector3 camera_position;

        // Array of shader names
        char *shader_names[BUILDING_OBJECT_AMOUNT] = {
            "buildingobj"};

        // Array of model names
        char *model_names[BUILDING_OBJECT_AMOUNT] = {
            "BasicTestLevelOpenTop_reexport",
            "World_VisualModel"};

        // Array of texture names
        char *texture_names[BUILDING_OBJECT_AMOUNT] = {
            "Texture",     // Test building
            "20x20squares" // World building(s)
        };

        // Array of texture formats for each texture file
        char *texture_formats[BUILDING_OBJECT_AMOUNT] = {
            ".png", // Test Building
            ".png"  // World Model
        };

        // Array of mesh offsets
        Vector3 mesh_offsets[BUILDING_OBJECT_AMOUNT] = {
            Vector3(0.0, 0.0, 0.0), // Test building
            Vector3(0.0, 0.0, 0.0)  // Map model
        };

        // Values that darken the texture since the loaded texture can often appear faded
        float tex_darken_values[BUILDING_OBJECT_AMOUNT] = {
            0.8, // Test building
            0.8  // Map model
        };

        // Array of meshes to be used for collisions
        char *collision_boxes[BUILDING_OBJECT_AMOUNT] = {
            "None",                // Test Building
            "World_CollisionModel" // World Model
        };

        // Array of collision mesh offsets
        Vector3 col_offsets[BUILDING_OBJECT_AMOUNT] = {
            Vector3(0.0, 0.0, 0.0), // Test building
            Vector3(0.0, 0.0, 0.0)  // Map model
        };

    protected:
        static void _bind_methods();

    public:
        BuildingObj();

        void _enter_tree() override;
        void _ready() override;
        void _process(double delta);

        // Member function that adds a light to the building object
        void AddLight(Vector3 light_pos, Vector3 light_col, int spec_power);

        // Member function that updates the view position for the building object
        void SetCameraPosition(Vector3 camera_pos);

        // Member function that registers camera triggers for signal purposes
        void RegisterCameraTrigs(Vector<CameraTrigger *> cam_trigs);

        // Member function that sets initial values for the building object
        void SetValues(int build_type, bool is_textured, bool auto_collision);

        // the return type represents whether it existed already; true if it is brand-new; false if it was retrieved from the SceneTree
        // search defines whether the scenetree should be checked for an instance
        template <class T>
        bool create_and_add_as_child(T *&pointer, String name, bool search = false);
    };

}

#endif