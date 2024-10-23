#ifndef ENEMY
#define ENEMY
#include "game_object.h"
#include "quat_camera.h"
#include "defs.h"
#include <godot_cpp/classes/mesh_instance3d.hpp>   // for the debug statements
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/capsule_mesh.hpp>

namespace godot
{
    class Enemy : public GameObject
    {
        GDCLASS(Enemy, GameObject);

    protected:
        static void _bind_methods();
        CapsuleMesh *mesh = nullptr;
        QuatCamera *target;
        float speed = 2.0f;

    public:
        Enemy(Vector3 = Vector3(1.0f, 1.0f, 1.0f), QuatCamera * = nullptr, float = 2.0f);
        void _enter_tree() override;
        void _ready() override;
        void _process(double delta) override;
        void collide(EntityType) override;
    };
}

#endif