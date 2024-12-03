#ifndef CUSTOM_SCENE_H
#define CUSTOM_SCENE_H

#include <godot_cpp/classes/node3d.hpp>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/area3d.hpp>

#include <godot_cpp/variant/quaternion.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/transform3d.hpp>

#include <godot_cpp/templates/vector.hpp> // GODOT EQUIVALENT TO std::vector !!!!

#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/quad_mesh.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

#include <godot_cpp/variant/rect2.hpp>		 // for viewport size
#include <godot_cpp/classes/canvas_item.hpp> // for viewport size
#include <godot_cpp/classes/control.hpp>	 // for the anchors preset
#include <godot_cpp/classes/color_rect.hpp>

#include <godot_cpp/classes/cylinder_mesh.hpp>

#include "defs.h"
#include "PlayerCamera.h"
#include "CameraTrigger.h"
#include "Player.h"
#include "Interactable.h"
#include "CounterInteractable.h"
#include "AudioInteractable.h"
#include "ItemInteractable.h"
#include "LockoutInteractable.h"
#include "EnvObject.h"
#include "BuildingObj.h"
#include "SkyBox.h"
#include "HeightMapTerrain.h"
#include "particle_system_3501.h"
#include "Mascot.h"

#define DEBUG true

// everything in gdextension is defined in this namespace
namespace godot
{
	class CustomScene3501 : public Node3D
	{
		// this macro sets up a few internal things
		GDCLASS(CustomScene3501, Node3D);

	private:
		double time_passed;
		ColorRect *overscreen;
		Vector<PlayerCamera *> cameras;
		Vector<CameraTrigger *> cam_triggs;
		Vector<AudioInteractable *> audio_interactables;
		Vector<ParticleSystem3501 *> particle_systems;
		MeshInstance3D *playerTestObject_ptr;
		Player *player;
		GameState gameState;
		SkyBox *skybox;
		HeightMapTerrain *hmt;
		Mascot *mascot;

		AudioInteractable *testInt;
		ItemInteractable *testItem;

		CounterInteractable *testCount;
		LockoutInteractable *testLock1;
		LockoutInteractable *testLock2;

		EnvObject *testEnvObj;

		BuildingObj *testBuilding;

		// If the screen should glitch
		bool is_static;
		float start_static;
		float end_static;
		bool current_cam;

		// Values for the screen space effect
		MeshInstance3D *screen_quad_instance;
		ShaderMaterial *screen_space_shader_material;

		// create and setup the boxes; for this one they don't need to have separate create and setup functions.
		// This shouldn't be called in the assignment that you hand in. You can choose to delete the code if you want to.
		void setup_cameras();
		void create_cameras();
		void setup_interactables();
		void create_interactables();
		void create_lights();
		void create_env_objects();
		void create_building_objects();

		template <class T, class U>
		void re_parent(T *parent, U *child);

	protected:
		// a static function that Godot will call to find out which methods can be called and which properties it exposes
		static void _bind_methods();

	public:
		CustomScene3501();
		~CustomScene3501();

		void _process(double delta) override;
		void _enter_tree() override;
		void _ready() override;

		// Member function that creates a particle system
		void create_particle_system(String node_name, String shader_name);

		// the return type represents whether it existed already; true if it is brand-new; false if it was retrieved from the SceneTree
		// search defines whether the scenetree should be checked for an instance
		template <class T>
		bool create_and_add_as_child(T *&pointer, String name, bool search = false);

		// this variant you have probably seen before; it allows you to create a CustomScene3501 will a bit more complex of a hierarchy
		// we will assume that the node should always be searched for in this variant
		template <class T>
		bool create_and_add_as_child_of_parent(T *&pointer, String name, Node *parent);

		// the return type represents whether it existed already; true if it is brand-new; false if it was retrieved from the SceneTree
		// search defines whether the scenetree should be checked for an instance
		// SEE THE .CPP FOR MORE DETAIL ON THIS VARIANT (HINT: It is for pointers which have already been created).
		template <class T>
		bool add_as_child(T *&pointer, String name, bool search = false);
	};

}

#endif