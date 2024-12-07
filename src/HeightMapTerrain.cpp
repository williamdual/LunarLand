#include "defs.h"
#include "HeightMapTerrain.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void HeightMapTerrain::_bind_methods() {
    ClassDB::bind_method(D_METHOD("SetCameraPosition"), &HeightMapTerrain::SetCameraPosition);
}

HeightMapTerrain::HeightMapTerrain() : StaticBody3D() {}

void HeightMapTerrain::_enter_tree ( ){
    if(DEBUG) UtilityFunctions::print("Enter Tree - HeightMapTerrain.");
}

void HeightMapTerrain::_ready ( ){
    if(DEBUG) UtilityFunctions::print("Ready - HeightMapTerrain."); 
}

void HeightMapTerrain::_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) return;

    // Sending the necessary values to the shader
    mat->set_shader_parameter("init_light_positions", light_positions);
    mat->set_shader_parameter("light_colours", light_colours);
    mat->set_shader_parameter("view_pos", camera_position);
    mat->set_shader_parameter("spec_power", specular_power);
    mat->set_shader_parameter("num_lights", num_lights);
}

// Member function that registers camera triggers for signal purposes
void HeightMapTerrain::RegisterCameraTrigs(Vector<CameraTrigger*> cam_trigs) {

    // Connecting each camera trigger
    for (int i = 0; i < cam_trigs.size(); i++) {
        cam_trigs[i]->connect("NewCamPos", Callable(this, "SetCameraPosition"));
    }
}

// Member function that sets starting values for height map terrain
void HeightMapTerrain::SetupHeightMap(int type, float max_height) {

    // Setting proper array sizes
    light_positions.resize(32);
    light_colours.resize(32);
    specular_power.resize(32);
    num_lights = 0;

    // Setting up shader
    mat = memnew(ShaderMaterial);
    Ref<Shader> shader = ResourceLoader::get_singleton()->load(vformat("%s%s.gdshader", "Shaders/", "terrain"), "Shader");
    mat->set_shader(shader);

    // Setting up the terrain image
    Ref<Texture2D> loaded_texture = ResourceLoader::get_singleton()->load(vformat("%s%s%s", "Terrain/", texture_names[type], texture_formats[type]), "CompressedTexture2D");
    Ref<Image> loaded_image = loaded_texture->get_image();
    float image_width = loaded_image->get_width();
    float image_height = loaded_image->get_height();

    // Seeing if a height map needs to be generated

    // This code generates the height map but Godot will need to be restarted if generating for the first time
    // The images containing _heightmap in their names were all generated with this code
    // So that the game editor does not crash on first launch all generated heightmaps were included with this submission
    // if (ResourceLoader::get_singleton()->load(vformat("%s%s%s%s", "res://Terrain/", texture_names[type], "_heightmap", ".png"), "Image") == NULL) {

    //     // Generating noise based on pixels
    //     FastNoiseLite noise_gen;
    //     srand(time(NULL));
    //     noise_gen.set_seed(rand() % 100);
    //     noise_gen.set_noise_type(FastNoiseLite::TYPE_PERLIN);
    //     noise_gen.set_frequency(0.003);
    //     PackedFloat64Array pixels;
    //     for (int i = 0; i < image_height; i++) {
    //         for (int j = 0; j < image_width; j++) {

    //             // Determining noise per pixel
    //             float noise = noise_gen.get_noise_2d(i, j);
    //             pixels.append(noise);
    //         }
    //     }

    //     // Creating a new image and buffer for the noise image
    //     Image new_noise_image;
    //     new_noise_image.create(image_width, image_height, false, Image::FORMAT_RGB8);
    //     PackedByteArray noise_image_buf;
    //     noise_image_buf.resize(image_width * image_height * 4); // Multiplied by 4 to accomodate for all values in RGB8
    //     noise_image_buf = new_noise_image.get_data();

    //     // Copying collected noise to the buffer
    //     for (int i = 0; i < image_height; i++) {
    //         for (int j = 0; j < image_width; j++) {

    //             // Converting value to grayscale
    //             int val = int((pixels[i * image_width + j] + 1.0) * 127.5);

    //             // Appending three times for r g and b
    //             noise_image_buf.append(val);
    //             noise_image_buf.append(val);
    //             noise_image_buf.append(val);
    //         }
    //     }

    //     // Setting the image data
    //     new_noise_image.set_data(image_width, image_height, false, Image::FORMAT_RGB8, noise_image_buf);
    //     new_noise_image.save_png(vformat("%s%s%s%s", "res://Terrain/", texture_names[type], "_heightmap", ".png"));
    // }

    // Establishing the surface that will be used for the terrain
    create_and_add_as_child<MeshInstance3D>(mesh, "TerrainMesh", true);
    PlaneMesh* surface = memnew(PlaneMesh);
    surface->surface_set_material(0, mat);
    surface->set_subdivide_depth(50);
    surface->set_subdivide_width(50);

    // Setting the heightmap in the shader
    Ref<Texture2D> heightmap = ResourceLoader::get_singleton()->load(vformat("%s%s%s%s", "res://Terrain/", texture_names[type], "_heightmap", ".png"), "CompressedTexture2D");
    mat->set_shader_parameter("heightmap", heightmap);
    mat->set_shader_parameter("max_height", max_height);

    // Creating the shape for the surface
    hit_shape = memnew(CollisionShape3D);
    create_and_add_as_child<CollisionShape3D>(hit_shape, "CollisionShape", true);

    // Getting the heightmap data for the collision shape
    HeightMapShape3D* col_shape = memnew(HeightMapShape3D);
    Ref<Image> height_image = heightmap->get_image();
    height_image->decompress();
    height_image->convert(Image::FORMAT_RF);
    height_image->resize(height_image->get_width() * 0.1, height_image->get_height() * 0.1);
    PackedFloat32Array image_data = height_image->get_data().to_float32_array();

    // Multiplying each data by the max height
    image_data.resize(height_image->get_width() * height_image->get_height());
    for (int i = 0; i < image_data.size(); i++) {
        image_data[i] *= max_height;
    }

    // Setting collision shape values
    col_shape->set_map_depth(height_image->get_height());
    col_shape->set_map_width(height_image->get_width());
    col_shape->set_map_data(image_data);
    hit_shape->set_shape(col_shape);

    // Setting the texture
    Ref<Texture2D> texture = ResourceLoader::get_singleton()->load(vformat("%s%s%s", "Terrain/", texture_names[type], texture_formats[type]), "CompressedTexture2D");
    mat->set_shader_parameter("sampler", texture);

    // Setting the rest of the mesh values
    surface->set_size(Vector2(height_image->get_width(), height_image->get_height()));
    mesh->set_mesh(surface);
    mesh->set_material_override(mat);
}

// Member function that sets a camera position
void HeightMapTerrain::SetCameraPosition(Vector3 camera_pos) {
    camera_position = camera_pos;
}

// Member function that adds a light position and colour to the building object
void HeightMapTerrain::AddLight(Vector3 light_pos, Vector3 light_col, int spec_power) {

    // Adding light position and colour to the necessary arrays
    light_positions[num_lights] = light_pos;
    light_colours[num_lights] = light_col;
    specular_power[num_lights] = spec_power;
    num_lights++;
}

//
template <class T>
// returns true if pointer is brand-new; false if retrieved from SceneTree
bool HeightMapTerrain::create_and_add_as_child(T *&pointer, String name, bool search)
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