#ifndef BEACON
#define BEACON

#include <godot_cpp/classes/mesh_instance3d.hpp>   // for the debug statements
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/box_mesh.hpp>

#include "game_object.h"
// everything in gdextension is defined in this namespace
namespace godot
{
    class Beacon : public GameObject
    {
        GDCLASS(Beacon, GameObject);

    protected:
        static void _bind_methods();
        bool targeted;
        BoxMesh *notTargetedMesh = nullptr;
        SphereMesh *targetedMesh = nullptr;
        Color color;

    public:
        Beacon(Vector3 = Vector3(1.0f, 1.0f, 1.0f), Color = Color(0.0f, 0.0f, 1.0f, 1.0f), float = 2.0f, bool = false);

        void _enter_tree() override;
        void _ready() override;
        void _process(double delta) override;
        void collide(EntityType) override;
        void becomeTargeted();
    };

}

#endif