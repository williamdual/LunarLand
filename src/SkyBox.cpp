#include "defs.h"
#include "SkyBox.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void SkyBox::_bind_methods() {}

SkyBox::SkyBox() : WorldEnvironment() {}

void SkyBox::_enter_tree ( ){
    if(DEBUG) UtilityFunctions::print("Enter Tree - SkyBox.");

    SetupSkyBox();
}

void SkyBox::_ready ( ){
    if(DEBUG) UtilityFunctions::print("Ready - SkyBox."); 
}

void SkyBox::_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) return;
}

void SkyBox::SetupSkyBox() {

    // Initializing the environment and sky
    env = memnew(Environment);
    sky = memnew(Sky);

    // Setting up shader
    mat = memnew(ShaderMaterial);
    Ref<Shader> shader = ResourceLoader::get_singleton()->load(vformat("%s%s.gdshader", "Shaders/", "sky"), "Shader");
    mat->set_shader(shader);

    // Starfield texture courtesy of Space Spheremaps: https://space-spheremaps.itch.io/space-spheremaps

    // Planet texture courtesy of Deep-Fold: https://deep-fold.itch.io/pixel-planet-generator

    // Setting the texture
    Ref<Texture2D> texture = ResourceLoader::get_singleton()->load(vformat("%s%s%s", "Sky/", "plain_starfield_1", ".png"), "CompressedTexture2D");
    mat->set_shader_parameter("sampler", texture);

    // Setting the environment and sky
    sky->set_material(mat);
    env->set_sky(sky);
    env->set_background(Environment::BG_SKY);
    this->set_environment(env);
}