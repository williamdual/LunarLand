#ifndef HEIGHT_MAP_TERRAIN_H
#define HEIGHT_MAP_TERRAIN_H

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>

#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/image.hpp>

#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/plane_mesh.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/fast_noise_lite.hpp>
#include <godot_cpp/classes/height_map_shape3d.hpp>

#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/vector3.hpp>

#include "CameraTrigger.h"

// everything in gdextension is defined in this namespace
namespace godot {

// Enum for better control over the type of terrain
enum TerrainType {
    MOON,
    SAND,
    TERRAIN_AMOUNT
};

class HeightMapTerrain : public StaticBody3D {
    GDCLASS(HeightMapTerrain, StaticBody3D);

private:

    // Shader material for the height map
    ShaderMaterial* mat;

    // Values and arrays important to the height map
    Ref<Image> terrain;
    MeshInstance3D* mesh;
    int terrain_type;
    CollisionShape3D* hit_shape;

    // Vectors for light and camera positions and colours as well as the number of lights
    int num_lights;
    TypedArray<Vector3> light_positions;
    TypedArray<Vector3> light_colours;
    TypedArray<int> specular_power;
    Vector3 camera_position;

    // Array of texture names
    char* texture_names[TERRAIN_AMOUNT] = {
        "moon_surface",  // Moon texture designed by Freepik: https://www.freepik.com/free-photo/black-white-details-moon-texture-concept_29662110.htm#fromView=keyword&page=1&position=2&uuid=817eafe0-6585-4bc4-ba7c-eac145809aef&new_detail=true
        "sand_texture" // Sand texture designed by Freepik: https://www.freepik.com/free-photo/yellow-textured-wall-background-texture_1198462.htm#fromView=keyword&page=1&position=14&uuid=b3a5d0e7-395c-41ca-b51f-492bbf48e2d7&new_detail=true
	};

    // Array of texture formats for each texture file
    char* texture_formats[TERRAIN_AMOUNT] = {
		".jpg",
        ".jpg"
	};
    
protected:
    static void _bind_methods();

public:
    HeightMapTerrain();
    
    void _enter_tree ( ) override;
    void _ready ( ) override;
    void _process(double delta);

    // This member function generates the height map
    void SetupHeightMap(int type, float max_height);

        // Member function that adds a light to the building object
    void AddLight(Vector3 light_pos, Vector3 light_col, int spec_power);

    // Member function that updates the view position for the building object
    void SetCameraPosition(Vector3 camera_pos);

    // Member function that registers camera triggers for signal purposes
    void RegisterCameraTrigs(Vector<CameraTrigger*> cam_trigs);

    // the return type represents whether it existed already; true if it is brand-new; false if it was retrieved from the SceneTree
	// search defines whether the scenetree should be checked for an instance
	template <class T>
	bool create_and_add_as_child(T *&pointer, String name, bool search = false);
};

}

#endif