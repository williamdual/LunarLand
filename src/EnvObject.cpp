#include "defs.h"
#include "EnvObject.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void EnvObject::_bind_methods() {
    ClassDB::bind_method(D_METHOD("SetCameraPosition"), &EnvObject::SetCameraPosition);
}

EnvObject::EnvObject() : StaticBody3D() {}

void EnvObject::_enter_tree ( ){
    if(DEBUG) UtilityFunctions::print("Enter Tree - EnvObject.");
}

void EnvObject::_ready ( ){
    if(DEBUG) UtilityFunctions::print("Ready - EnvObject."); 
}

void EnvObject::_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) return;

    // Sending the necessary values to the shader
    mat->set_shader_parameter("init_light_positions", light_positions);
    mat->set_shader_parameter("light_colours", light_colours);
    mat->set_shader_parameter("view_pos", camera_position);
    mat->set_shader_parameter("spec_power", specular_power);
    mat->set_shader_parameter("num_lights", num_lights);
}

// Member function that registers camera triggers for signal purposes
void EnvObject::RegisterCameraTrigs(Vector<CameraTrigger*> cam_trigs) {

    // Connecting each camera trigger
    for (int i = 0; i < cam_trigs.size(); i++) {
        cam_trigs[i]->connect("NewCamPos", Callable(this, "SetCameraPosition"));
    }
}

// Member function that sets starting values for the environment object
void EnvObject::SetValues(int obj_type, int col_type) {

    // Setting proper array sizes
    light_positions.resize(32);
    light_colours.resize(32);
    specular_power.resize(32);
    num_lights = 0;

    // Setting up shader
    mat = memnew(ShaderMaterial);
    Ref<Shader> shader = ResourceLoader::get_singleton()->load(vformat("%s%s.gdshader", "Shaders/", shader_names[0]), "Shader");
    mat->set_shader(shader);

    // Setting the mesh
    create_and_add_as_child<MeshInstance3D>(mesh, "EnvObjMesh", true);
    Ref<Mesh> new_mesh = ResourceLoader::get_singleton()->load(vformat("%s%s.obj", "Models/", model_names[obj_type]), "Mesh");
    new_mesh->surface_set_material(0, mat);
    mesh->set_mesh(new_mesh);
    mesh->set_material_override(mat);
    mesh->set_position(mesh_offsets[obj_type]);

    // Setting the texture
    Ref<Texture2D> texture = ResourceLoader::get_singleton()->load(vformat("%s%s%s", "Textures/", texture_names[obj_type], texture_formats[obj_type]), "CompressedTexture2D");
    mat->set_shader_parameter("sampler", texture);

    // Setting the darking value for the texture
    mat->set_shader_parameter("darkening_val", tex_darken_values[obj_type]);

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

// Member function that sets starting values for the environment object
void EnvObject::SetPrimValues(int prim_type, int col_type) {

    // Setting proper array sizes
    light_positions.resize(32);
    light_colours.resize(32);
    specular_power.resize(32);
    num_lights = 0;

    // Setting up shader
    mat = memnew(ShaderMaterial);
    Ref<Shader> shader = ResourceLoader::get_singleton()->load(vformat("%s%s.gdshader", "Shaders/", shader_names[0]), "Shader");
    mat->set_shader(shader);

    // Setting the mesh based on what kind of mesh is desired
    create_and_add_as_child<MeshInstance3D>(mesh, "EnvObjMesh", true);
    switch (prim_type) {

        // Box shaped mesh
        case PRIM_BOX: {
            BoxMesh* box_mesh = memnew(BoxMesh);
            box_mesh->surface_set_material(0, mat);
            mesh->set_mesh(box_mesh);
            mesh->set_material_override(mat);
            break;
        }

        // Trapezoid prism shaped mesh
        case PRIM_CAPSULE: {
            CapsuleMesh* capsule_mesh = memnew(CapsuleMesh);
            capsule_mesh->surface_set_material(0, mat);
            mesh->set_mesh(capsule_mesh);
            mesh->set_material_override(mat);
            break;
        }

        default:
            break;
    }

    // Setting the texture
    Ref<Texture2D> texture = ResourceLoader::get_singleton()->load(vformat("%s%s%s", "Textures/", texture_names[ENV_OBJECT_ALIEN], texture_formats[ENV_OBJECT_ALIEN]), "CompressedTexture2D");
    mat->set_shader_parameter("sampler", texture);

    // Setting the darking value for the texture
    mat->set_shader_parameter("darkening_val", tex_darken_values[ENV_OBJECT_ALIEN]);

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
void EnvObject::SetHitBox() {

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
void EnvObject::SetHitCylinder() {

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
void EnvObject::SetCameraPosition(Vector3 camera_pos) {
    camera_position = camera_pos;
}

// Member function that adds a light position and colour to the environment object
void EnvObject::AddLight(Vector3 light_pos, Vector3 light_col, int spec_power) {

    // Adding light position and colour to the necessary arrays
    light_positions[num_lights] = light_pos;
    light_colours[num_lights] = light_col;
    specular_power[num_lights] = spec_power;
    num_lights++;
}

template <class T>
// returns true if pointer is brand-new; false if retrieved from SceneTree
bool EnvObject::create_and_add_as_child(T *&pointer, String name, bool search)
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