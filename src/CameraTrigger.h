#ifndef CAMERATRIGGER_H
#define CAMERATRIGGER_H

// parent class
#include <godot_cpp/classes/scene_tree.hpp>

#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/box_shape3d.hpp>
#include "PlayerCamera.h"
#include "Player.h"

#include <godot_cpp/classes/input.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/variant/quaternion.hpp>

#include "defs.h"
// everything in gdextension is defined in this namespace
namespace godot
{
    class CameraTrigger : public Area3D
    {
        // this macro sets up a few internal things
        GDCLASS(CameraTrigger, Area3D);

    private:
        // this one is the one we will use to transform our side, up and forward vectors for the GetX functions.
        // why would it be a bad idea to transform by the built-in quaternion?
        PlayerCamera *target_cam;
        CollisionShape3D *collider;
        BoxShape3D *box_shape;
        Player *player;

        void SetTrigger();

    protected:
        // a static function that Godot will call to find out which methods can be called and which properties it exposes
        static void _bind_methods();

    public:
        CameraTrigger();
        ~CameraTrigger();
        void _enter_tree() override;
        void _ready() override;
        void _process(double delta) override;
        void setColliderTransformation(Vector3 size);
        void SetCamera(PlayerCamera *cam);
        void SetPlayer(Player *p);

        void Collision(Area3D *);
    };
}

#endif