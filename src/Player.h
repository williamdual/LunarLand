#ifndef PLAYER_H
#define PLAYER_H

// parent class
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
#include <godot_cpp/classes/input.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/variant/quaternion.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/cylinder_mesh.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/cylinder_shape3d.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/audio_listener3d.hpp>

#include "Inventory.h"
#include "PlayerCamera.h"

#include "defs.h"
// everything in gdextension is defined in this namespace
namespace godot
{
    class Player : public CharacterBody3D
    {
        GDCLASS(Player, CharacterBody3D);

    private:
        double time_passed;
        MeshInstance3D *mesh;
        ShaderMaterial *mat;
        Area3D *area;
        CollisionShape3D *collider;
        CollisionShape3D *hit_shape;
        CylinderShape3D *cylinder_shape;
        PlayerCamera *camera;
        AudioListener3D *listener;
        Inventory *inventory;
        Vector3 saved_velocity;
        Vector3 movementDelta;
        Vector3 savedMovementDelta;
        float moveSpeed;
        Vector3 gravityDelta;
        bool paused;
        float rot_speed;
        Ref<Mesh> idle;
        Ref<Mesh> first_move;
        Ref<Mesh> second_move;

        // Vectors for light and camera positions and colours as well as the number of lights
        int num_lights;
        TypedArray<Vector3> light_positions;
        TypedArray<Vector3> light_colours;
        TypedArray<int> specular_power;
        Vector3 camera_position;

        float WrapDegree(float value);
        float Clamp(float value, float min, float max);
        float Sign(float value);

    protected:
        // a static function that Godot will call to find out which methods can be called and which properties it exposes
        static void _bind_methods();

    public:
        Player();
        ~Player();
        void _enter_tree() override;
        void _ready() override;
        void _process(double delta) override;
        PlayerCamera *GetCamera();
        inline Inventory *GetInventory(void) { return inventory; }
        void SetCamera(PlayerCamera *);

        // Member function that adds a light to the environment object
        void AddLight(Vector3 light_pos, Vector3 light_col, int spec_power);

        // Member function that updates the view position for the environment object
        void SetCameraPosition(Vector3 camera_pos);

        // Member function that registers camera triggers for signal purposes
        //void RegisterCameraTrigs(Vector<CameraTrigger*> cam_trigs);

        // the return type represents whether it existed already; true if it is brand-new; false if it was retrieved from the SceneTree
        // search defines whether the scenetree should be checked for an instance
        template <class T>
        bool create_and_add_as_child(T *&pointer, String name, bool search = false);
        float GetMoveSpeed();
    };
}

#endif