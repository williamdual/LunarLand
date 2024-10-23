#ifndef BULLET
#define BULLET
#include "game_object.h"
#include <godot_cpp/classes/mesh_instance3d.hpp>   // for the debug statements
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/sphere_mesh.hpp>
namespace godot
{
    class Bullet : public GameObject
    {
        GDCLASS(Bullet, GameObject);

    protected:
        static void _bind_methods();
        SphereMesh *mesh = nullptr;
        float timeSinceBirth;
        float speed;
        Vector3 forward;

    public:
        Bullet(Vector3 = Vector3(1.0f, 1.0f, 1.0f), Vector3 = Vector3(1.0f, 1.0f, 1.0f), Color = Color(1.0f, 0.5f, 0.0f, 1.0f), float = 7.0f, float = 0.5f);
        void _enter_tree() override;
        void _ready() override;
        void _process(double delta) override;
        void collide(EntityType) override;
        bool overdue();
    };
}

#endif