#ifndef GAME_OBJECT
#define GAME_OBJECT
#include "defs.h"
#include <godot_cpp/classes/mesh_instance3d.hpp>   // for the debug statements
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/variant/vector3.hpp>

#include "quat_camera.h"
// everything in gdextension is defined in this namespace
namespace godot
{
    class GameObject : public MeshInstance3D
    {
        GDCLASS(GameObject, MeshInstance3D);

    protected:
        static void _bind_methods();
        bool gameOver;
        float radius;

    public:
        GameObject(EntityType = EntityType::game_obj, float = 4.0f);
        EntityType entity;
        virtual void _enter_tree() override = 0;
        virtual void _ready() override = 0;
        virtual void _process(double delta) override = 0;

        bool isColliding(GameObject *);
        virtual bool isColliding(QuatCamera *);

    private:
        virtual void collide(EntityType) = 0;
    };

}

#endif