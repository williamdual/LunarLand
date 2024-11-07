#ifndef PLAYER_H
#define PLAYER_H

// parent class
#include <godot_cpp/classes/scene_tree.hpp>

#include <godot_cpp/classes/input.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/variant/quaternion.hpp>

#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/cylinder_mesh.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/cylinder_shape3d.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

#include "defs.h"
// everything in gdextension is defined in this namespace
namespace godot
{
    class Player : public MeshInstance3D
    {
        GDCLASS(Player, MeshInstance3D);

    private:
        CylinderMesh *mesh;
        StandardMaterial3D *mat;
        Area3D *area;
        CollisionShape3D *collider;
        CylinderShape3D *cylinder_shape;


    protected:
        // a static function that Godot will call to find out which methods can be called and which properties it exposes
        static void _bind_methods();

    public:
        Player();
        ~Player();
        void _enter_tree() override;
        void _ready() override;
        void _process(double delta) override;

    };
}

#endif