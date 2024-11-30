#include "defs.h"
#include "BuildingObj.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void BuildingObj::_bind_methods() {
    ClassDB::bind_method(D_METHOD("SetCameraPosition"), &BuildingObj::SetCameraPosition);
}

BuildingObj::BuildingObj() : Node3D() {}

void BuildingObj::_enter_tree ( ){
    if(DEBUG) UtilityFunctions::print("Enter Tree - BuildingObj.");
}

void BuildingObj::_ready ( ){
    if(DEBUG) UtilityFunctions::print("Ready - BuildingObj."); 
}

void BuildingObj::_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) return;

    // Sending the necessary values to the shader
    mat->set_shader_parameter("init_light_positions", light_positions);
    mat->set_shader_parameter("light_colours", light_colours);
    mat->set_shader_parameter("view_pos", camera_position);
    mat->set_shader_parameter("spec_power", specular_power);
    mat->set_shader_parameter("num_lights", num_lights);
}

// Member function that registers camera triggers for signal purposes
void BuildingObj::RegisterCameraTrigs(Vector<CameraTrigger*> cam_trigs) {

    // Connecting each camera trigger
    for (int i = 0; i < cam_trigs.size(); i++) {
        cam_trigs[i]->connect("NewCamPos", Callable(this, "SetCameraPosition"));
    }
}

// Member function that sets starting values for the building object
void BuildingObj::SetValues(int building_type, bool is_textured, bool auto_collision) {

    // Setting proper array sizes
    light_positions.resize(32);
    light_colours.resize(32);
    specular_power.resize(32);
    num_lights = 0;
    textured = is_textured;

    // Setting up shader
    mat = memnew(ShaderMaterial);
    Ref<Shader> shader = ResourceLoader::get_singleton()->load(vformat("%s%s.gdshader", "Shaders/", shader_names[0]), "Shader");
    mat->set_shader(shader);

    // Setting the mesh
    create_and_add_as_child<MeshInstance3D>(mesh, "BuildingObjMesh", true);
    Ref<Mesh> new_mesh = ResourceLoader::get_singleton()->load(vformat("%s%s.obj", "Models/", model_names[building_type]), "Mesh");
    new_mesh->surface_set_material(0, mat);
    mesh->set_mesh(new_mesh);
    mesh->set_material_override(mat);
    mesh->set_position(mesh_offsets[building_type]);

    // Setting the texture
    if (textured) {
        Ref<Texture2D> texture = ResourceLoader::get_singleton()->load(vformat("%s%s%s", "Textures/", texture_names[building_type], texture_formats[building_type]), "CompressedTexture2D");
        mat->set_shader_parameter("sampler", texture);
    } else {
        //Vector3 grey = Vector3(0.5020, 0.5020, 0.5020);
        Vector3 magenta = Vector3(1.0, 0.0, 1.0);
        mat->set_shader_parameter("static_colour", magenta);
    }

    // Setting the darking value for the texture
    mat->set_shader_parameter("darkening_val", tex_darken_values[building_type]);

    // Setting the textured value for the shader
    mat->set_shader_parameter("is_textured", textured);

    // Setting hitboxes for map
    if (auto_collision) {

        // Creating a collision mesh based on the building model
        mesh->create_trimesh_collision();

    } else {

        // Establishing a new model as the basis for the collisions
        create_and_add_as_child<MeshInstance3D>(col_mesh, "BuildingColMesh", true);
        Ref<Mesh> col_shape = ResourceLoader::get_singleton()->load(vformat("%s%s.obj", "Models/", collision_boxes[building_type]), "Mesh");
        StandardMaterial3D* col_mat = memnew(StandardMaterial3D);
        col_mat->set_transparency(StandardMaterial3D::TRANSPARENCY_ALPHA);
        col_mat->set_albedo(Color(1.0, 1.0, 1.0, 0.0));
        //col_mesh->surface_set_material(0, col_mat);
        col_mesh->set_mesh(col_shape);
        col_mesh->set_material_override(col_mat);
        col_mesh->set_position(col_offsets[building_type]);
        col_mesh->create_trimesh_collision();
    }
}

// Member function that sets a camera position
void BuildingObj::SetCameraPosition(Vector3 camera_pos) {
    camera_position = camera_pos;
}

// Member function that adds a light position and colour to the building object
void BuildingObj::AddLight(Vector3 light_pos, Vector3 light_col, int spec_power) {

    // Adding light position and colour to the necessary arrays
    light_positions[num_lights] = light_pos;
    light_colours[num_lights] = light_col;
    specular_power[num_lights] = spec_power;
    num_lights++;
}

template <class T>
// returns true if pointer is brand-new; false if retrieved from SceneTree
bool BuildingObj::create_and_add_as_child(T *&pointer, String name, bool search)
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