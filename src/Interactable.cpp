#include "defs.h"
#include "Interactable.h"
#include "CameraTrigger.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void Interactable::_bind_methods() {
    ClassDB::bind_method(D_METHOD("SetCameraPosition"), &Interactable::SetCameraPosition);
}

Interactable::Interactable() {}

Interactable::Interactable(Player* p, int type, int col_type, bool glow, double rad) : StaticBody3D() {
    time_passed = 0.0;
    radius = rad;
    player = p;
    interactable_type = type;
    in_range = false;
    glow_in_range = glow;
    has_col_shape = false;
    num_lights = 0;
    light_positions.resize(32);
    light_colours.resize(32);
    specular_power.resize(32);
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
    mat->set_shader_parameter("init_light_positions", light_positions);
    mat->set_shader_parameter("light_colours", light_colours);
    mat->set_shader_parameter("view_pos", camera_position);
    mat->set_shader_parameter("spec_power", specular_power);
    mat->set_shader_parameter("num_lights", num_lights);
    mat->set_shader_parameter("test_col", light_colours[0]);

    // Getting input and determining if the interactable should trigger
    Input *_input = Input::get_singleton();
    if (_input->is_action_just_pressed("interact") && in_range) {
        UtilityFunctions::print("Interacted");
        Interact();
    }
}

// Member function that registers camera triggers for signal purposes
void Interactable::RegisterCameraTrigs(Vector<CameraTrigger*> cam_trigs) {

    // Connecting each camera trigger
    for (int i = 0; i < cam_trigs.size(); i++) {
        cam_trigs[i]->connect("NewCamPos", Callable(this, "SetCameraPosition"));
    }
}

// Nothing in this function it is meant to be overriden
void Interactable::Interact() {}

// Member function that acts as a contructor in the event the default contructor is used
void Interactable::SetValues(Player* p, int type, int col_type, bool glow, double rad) {
    time_passed = 0.0;
    radius = rad;
    player = p;
    interactable_type = type;
    in_range = false;
    glow_in_range = glow;
    num_lights = 0;
    camera_position = Vector3(0, 0, 0);

    // Setting proper array sizes
    light_positions.resize(32);
    light_colours.resize(32);
    specular_power.resize(32);

    // Setting up shader
    mat = memnew(ShaderMaterial);
    Ref<Shader> shader = ResourceLoader::get_singleton()->load(vformat("%s%s.gdshader", "Shaders/", shader_names[0]), "Shader");
    mat->set_shader(shader);

    // Setting the mesh
    create_and_add_as_child<MeshInstance3D>(mesh, "InterMesh", true);
    Ref<Mesh> new_mesh = ResourceLoader::get_singleton()->load(vformat("%s%s.obj", "Models/", model_names[interactable_type]), "Mesh");
    new_mesh->surface_set_material(0, mat);
    mesh->set_mesh(new_mesh);
    mesh->set_position(mesh_offsets[interactable_type]);

    // Setting the texture
    Ref<Texture2D> texture = ResourceLoader::get_singleton()->load(vformat("%s%s%s", "Textures/", texture_names[interactable_type], texture_formats[interactable_type]), "CompressedTexture2D");
    mat->set_shader_parameter("sampler", texture);

    // Seeing if the object already has a collision shape
    if (find_child("CollisionShape") == NULL) {
        has_col_shape = false;
    } else {
        return;
    }

    // Seeing what type of collision to make
    switch(col_type) {
        case SHAPE_NONE:
            break;
        case SHAPE_BOX:
            SetHitBox();
            break;
        case SHAPE_CYLINDER:
            SetHitCylinder();
            break;
        default:
            break;
    }
}

// This member function creates a hitbox
void Interactable::SetHitBox() {

    // If the object already has a collision shape then return
    if (has_col_shape) {
        return;
    } else {
        has_col_shape = true;
    }

    // Adding a collision shape as a child
    hit_shape = memnew(CollisionShape3D);
    create_and_add_as_child<CollisionShape3D>(hit_shape, "CollisionShape", false);
    BoxShape3D* box_shape = memnew(BoxShape3D);
    box_shape->set_size(mesh->get_mesh()->get_aabb().size);
    hit_shape->set_shape(box_shape);
}

// This member function creates a hit cylinder
void Interactable::SetHitCylinder() {

    // If the object already has a collision shape then return
    if (has_col_shape) {
        return;
    } else {
        has_col_shape = true;
    }

    // Adding a collision shape as a child
    hit_shape = memnew(CollisionShape3D);
    create_and_add_as_child<CollisionShape3D>(hit_shape, "CollisionShape", false);

    // Determining widest part of scale
    CylinderShape3D* cyl_shape = memnew(CylinderShape3D);
    Vector3 obj_size = mesh->get_mesh()->get_aabb().size;
    float diameter = 1.0;
    if (obj_size.x > obj_size.z) {
        diameter = obj_size.x;
    } else {
        diameter = obj_size.z;
    }

    // Setting the height and radius of the shape
    cyl_shape->set_height(obj_size.y);
    cyl_shape->set_radius(diameter / 2.0);

    // Setting the collision shape
    hit_shape->set_shape(cyl_shape);
}

// Member function that sets a camera position
void Interactable::SetCameraPosition(Vector3 camera_pos) {
    camera_position = camera_pos;
}

// Member function that adds a light position and colour to the interactable
void Interactable::AddLight(Vector3 light_pos, Vector3 light_col, int spec_power) {

    // Adding light position and colour to the necessary arrays
    light_positions[num_lights] = light_pos;
    light_colours[num_lights] = light_col;
    specular_power[num_lights] = spec_power;
    num_lights++;
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