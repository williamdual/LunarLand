#ifndef POWER_UP_SPEED
#define POWER_UP_SPEED
#include "game_object.h"
#include <godot_cpp/classes/mesh_instance3d.hpp>   // for the debug statements
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/sphere_mesh.hpp>
namespace godot
{
    class PowerUpSpeed : public GameObject
    {
        GDCLASS(PowerUpSpeed, GameObject);

    protected:
        static void _bind_methods();
        SphereMesh *mesh = nullptr;

    public:
        PowerUpSpeed(Vector3 = Vector3(1.0f, 1.0f, 1.0f), Color = Color(0.0f, 1.0f, 0.0f, 1.0f), float = 2.0f);
        void _enter_tree() override;
        void _ready() override;
        void _process(double delta) override;
        void collide(EntityType) override;
    };
}

#endif
