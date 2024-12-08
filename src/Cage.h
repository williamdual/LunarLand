#ifndef CAGE_H
#define CAGE_H

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

// everything in gdextension is defined in this namespace
namespace godot {

class Cage : public Node3D {
    GDCLASS(Cage, Node3D);

private:
    // Static bodies for the walls
    StaticBody3D* wall_1;
    StaticBody3D* wall_2;
    StaticBody3D* wall_3;
    StaticBody3D* wall_4;
    StaticBody3D* wall_5;
    StaticBody3D* wall_6;
    StaticBody3D* wall_7;

    // Collision shapes for the walls
    CollisionShape3D* col_1;
    CollisionShape3D* col_2;
    CollisionShape3D* col_3;
    CollisionShape3D* col_4;
    CollisionShape3D* col_5;
    CollisionShape3D* col_6;
    CollisionShape3D* col_7;
    
protected:
    static void _bind_methods();

public:
    Cage();
    
    void _enter_tree ( ) override;
    void _ready ( ) override;

    // the return type represents whether it existed already; true if it is brand-new; false if it was retrieved from the SceneTree
	// search defines whether the scenetree should be checked for an instance
	template <class T>
	bool create_and_add_as_child(T *&pointer, String name, bool search = false);

    // this variant you have probably seen before; it allows you to create a CustomScene3501 will a bit more complex of a hierarchy
    // we will assume that the node should always be searched for in this variant
    template <class T>
    bool create_and_add_as_child_of_parent(T *&pointer, String name, Node *parent);
};

}

#endif