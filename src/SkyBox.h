#ifndef SKYBOX_H
#define SKYBOX_H

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>

#include <godot_cpp/classes/world_environment.hpp>
#include <godot_cpp/classes/environment.hpp>
#include <godot_cpp/classes/sky.hpp>

#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/texture2d.hpp>

#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/vector3.hpp>

// everything in gdextension is defined in this namespace
namespace godot {

class SkyBox : public WorldEnvironment {
    GDCLASS(SkyBox, WorldEnvironment);

private:

    // Environment
    Environment* env;
    Sky* sky;
    ShaderMaterial* mat;

    // This member function sets up the skybox
    void SetupSkyBox(void);
    
protected:
    static void _bind_methods();

public:
    SkyBox();
    
    void _enter_tree ( ) override;
    void _ready ( ) override;
    void _process(double delta);
};

}

#endif