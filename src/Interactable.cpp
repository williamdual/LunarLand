#include "defs.h"
#include "Interactable.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void Interactable::_bind_methods() {}

Interactable::Interactable() {}

Interactable::Interactable(Player* p, int type, bool glow, double rad) : Node3D() {
    time_passed = 0.0;
    radius = rad;
    player = p;
    interactable_type = type;
    in_range = false;
    glow_in_range = glow;
}

void Interactable::_enter_tree ( ){
    if(DEBUG) UtilityFunctions::print("Enter Tree - Interactable.");
}

void Interactable::_ready ( ){
    if(DEBUG) UtilityFunctions::print("Ready - Interactable."); 
}

void Interactable::_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) return;

    // Updating whether or not the player is in range
    in_range = IsInRange();

    // Sending the necessary values to the shader
    mat->set_shader_parameter("in_range", in_range);
    mat->set_shader_parameter("glows", glow_in_range);

    // Getting input and determining if the interactable should trigger
    Input *_input = Input::get_singleton();
    if (_input->is_action_just_pressed("interact") && in_range) {
        UtilityFunctions::print("Interacted");
        Interact();
    }
}

// Nothing in this function it is meant to be overriden
void Interactable::Interact() {}

// Member function that acts as a contructor in the event the default contructor is used
void Interactable::SetValues(Player* p, int type, bool glow, double rad) {
    time_passed = 0.0;
    radius = rad;
    player = p;
    interactable_type = type;
    in_range = false;
    glow_in_range = glow;

    // Setting up shader
    mat = memnew(ShaderMaterial);
    Ref<Shader> shader = ResourceLoader::get_singleton()->load(vformat("%s%s.gdshader", "Shaders/", shader_names[interactable_type]), "Shader");
    mat->set_shader(shader);

    // Setting the mesh
    create_and_add_as_child<MeshInstance3D>(mesh, "InterMesh", true);
    Ref<Mesh> new_mesh = ResourceLoader::get_singleton()->load(vformat("%s%s.obj", "Models/", model_names[interactable_type]), "Mesh");
    new_mesh->surface_set_material(0, mat);
    mesh->set_mesh(new_mesh);
    mesh->set_position(mesh_offsets[interactable_type]);

    // Setting the texture
    Ref<Texture2D> texture = ResourceLoader::get_singleton()->load(vformat("%s%s.png", "Textures/", texture_names[interactable_type]), "CompressedTexture2D");
    mat->set_shader_parameter("sampler", texture);
}

// This member function determines if the player is in range
bool Interactable::IsInRange() {
    if ((player->get_global_position() - this->get_global_position()).length() <= radius) {
        return true;
    } else {
        return false;
    }
}

template <class T>
// returns true if pointer is brand-new; false if retrieved from SceneTree
bool Interactable::create_and_add_as_child(T *&pointer, String name, bool search)
{
	// this is the default behaviour
	// added the search parameter so that we can skip the slow "find_child" call during runtime (not applicable to this demo, you should always use search = true until next assignment)
	if (search == false)
	{
		pointer = memnew(T);
		pointer->set_name(name);
		add_child(pointer);
		pointer->set_owner(get_tree()->get_edited_scene_root());
		return true;
	}

	// always only have to search once if we save it here
	Node *child = find_child(name);

	if (child == nullptr)
	{
		pointer = memnew(T);
		pointer->set_name(name);
		add_child(pointer);
		pointer->set_owner(get_tree()->get_edited_scene_root());
		return true;
	}
	else
	{
		pointer = dynamic_cast<T *>(child);
		return false;
	}
}