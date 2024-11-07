#include "custom_scene_3501.h"
#include <godot_cpp/core/memory.hpp> // for memnew
#include <godot_cpp/classes/random_number_generator.hpp>

using namespace godot;

void CustomScene3501::_bind_methods() {}

CustomScene3501::CustomScene3501() : Node3D()
{
	time_passed = 0.0;
	num_beacons = 5.0f;
	min_feild = Vector3(-15.0f, -8.0f, -15.0f);
	max_field = Vector3(15.0f, 10.0f, 10.0f);
	gameState = GameState::ongoing;
}

CustomScene3501::~CustomScene3501()
{
	// Add your cleanup here.
}

void CustomScene3501::_enter_tree()
{
	if (DEBUG)
		UtilityFunctions::print("Enter Tree - CustomScene3501.");

	create_and_add_as_child<PlayerCamera>(main_camera, "QuatCamera", true);

	bool is_new = create_and_add_as_child<MeshInstance3D>(playerTestObject_ptr, "PlayerTestObject", true);
	if(is_new == true)
	{
		//make new mesh
		CylinderMesh* newPlayerTestObjectMesh_ptr = memnew(CylinderMesh);
		newPlayerTestObjectMesh_ptr->set_height(2);
		newPlayerTestObjectMesh_ptr->set_top_radius(1);
		newPlayerTestObjectMesh_ptr->set_bottom_radius(1);
		
		//set the new meshes color
		StandardMaterial3D*  newPlayerTestObjectMaterial_ptr = memnew(StandardMaterial3D);
		newPlayerTestObjectMaterial_ptr->set_albedo(Color(1.0f, 1.0f, 0.0f, 1.0f));
		newPlayerTestObjectMesh_ptr->surface_set_material(0, newPlayerTestObjectMaterial_ptr);

		//set testMesh's mesh to the new mesh
		playerTestObject_ptr->set_mesh(newPlayerTestObjectMesh_ptr);
	}
	

	// The vectors are brand new every time you run the simulation or reload the project.
	// setup_reference_boxes();
}

void CustomScene3501::_ready()
{
	if (DEBUG)
		UtilityFunctions::print("Ready - CustomScene3501.");



	// set the player's position (the camera)
	main_camera->set_global_position(Vector3(0.0, 0.0, 0.0f));

	// now that we have set the camera's starting state, let's reinitialize its variables
	main_camera->_ready();

	main_camera->SetTarget(playerTestObject_ptr);
	
	playerTestObject_ptr->set_global_position(Vector3(0.0, -10.0, -10.0f));



	/*
	if (!Engine::get_singleton()->is_editor_hint()) // because im lazy
	{
		create_and_add_as_child<Node>(bulletGroup, "Bullets Group", true);
		setup_beacons();
		setup_enemys();
		setup_powerups();
	}
	*/
}

// called every frame (as often as possible)
void CustomScene3501::_process(double delta)
{
	if (Engine::get_singleton()->is_editor_hint())
		return; // Early return if we are in editor
	// Game loop stuff HERE

	if (gameState == GameState::ongoing)
	{
		/*
		//  beacons
		if (beacons[0]->isColliding(main_camera))
		{
			Beacon *tempBeacon = beacons[0];
			beacons.remove_at(0);
			tempBeacon->queue_free();
			if (beacons.size() > 0)
			{
				beacons.get(0)->becomeTargeted();
			}
			else
			{
				overscreen = memnew(ColorRect);
				create_and_add_as_child<ColorRect>(overscreen, "Won Screen", true);
				overscreen->set_color(Color(0.0f, 1.0f, 0.0f, 1.0f));
				overscreen->set_size(Vector2(9000, 9000));
				gameState = GameState::won;
				get_tree()->set_pause(true);
			}
		}
		*/ 

		// Enemys and enemys getting shot
		for (int i = enemys.size() - 1; i >= 0; i--)
		{
			/*
			if (enemys[i]->isColliding(main_camera))
			{
				overscreen = memnew(ColorRect);
				create_and_add_as_child<ColorRect>(overscreen, "Lost Screen", true);
				overscreen->set_color(Color(1.0f, 0.0f, 0.0f, 1.0f));
				overscreen->set_size(Vector2(9000, 9000));
				gameState = GameState::lost;
				get_tree()->set_pause(true);
			}
			for (int j = bullets.size() - 1; j >= 0; j--)
			{
				if (enemys[i]->isColliding(bullets[j]))
				{
					UtilityFunctions::print("HIT");
					Bullet *tempBull = bullets[j];
					Enemy *tempEnemy = enemys[i];
					bullets.remove_at(j);
					enemys.remove_at(i);
					tempBull->queue_free();
					tempEnemy->queue_free();
				}
			}
			*/
		}
		// power ups
		for (int i = power_ups.size() - 1; i >= 0; i--)
		{
			/*
			if (power_ups[i]->isColliding(main_camera))
			{
				PowerUpSpeed *tempPower = power_ups[i];
				power_ups.remove_at(i);
				tempPower->queue_free();
				main_camera->addSpeed(0.5f);
			}
			*/
		}
		// Bullet creation
		Input *_input = Input::get_singleton();
		if (_input->is_action_just_released("shoot"))
		{
			Bullet *bullet;
			Vector3 pos = main_camera->get_position();
			Vector3 dir = main_camera->GetForward();
			Color color = Color(1.0f, 0.5f, 0.0f, 1.0f);
			float sped = 8.0f;
			float size = 0.25f;
			bullet = memnew(Bullet(pos, dir, color, sped, size));
			bullet->set_name(vformat("bullet_%d", bullets.size()));
			bulletGroup->add_child(bullet);
			bullet->set_owner(get_tree()->get_edited_scene_root());
			bullets.push_back(bullet);
		}
		for (int i = bullets.size() - 1; i >= 0; i--)
		{
			if (bullets[i]->overdue())
			{
				Bullet *tempBull = bullets[i];
				bullets.remove_at(i);
				tempBull->queue_free();
			}
		}
		time_passed += delta;
	}
}

void CustomScene3501::setup_beacons()
{
	Node *ref_group;
	bool is_new = create_and_add_as_child<Node>(ref_group, "Beacons Group", true);
	if (is_new)
	{
		RandomNumberGenerator *rng = memnew(RandomNumberGenerator);
		for (int i = 0; i < num_beacons; i++)
		{
			float x = rng->randf_range(min_feild.x, max_field.x);
			float y = rng->randf_range(min_feild.y, max_field.y);
			float z = rng->randf_range(min_feild.z, max_field.z);
			Beacon *beacon;
			Vector3 pos = Vector3(x, y, z);
			Color color = Color(0.0f, 0.0f, 1.0f, 1.0f);
			float size = 2.0f;
			bool targeted = false;
			if (i == 0)
				targeted = true;
			beacon = memnew(Beacon(pos, color, size, targeted));
			beacon->set_name(vformat("Beacon_%d", i));
			ref_group->add_child(beacon);
			beacon->set_owner(get_tree()->get_edited_scene_root());
			beacons.push_back(beacon);
		}
	}
}
void CustomScene3501::setup_enemys()
{
	/*
	Node *ref_group;
	bool is_new = create_and_add_as_child<Node>(ref_group, "Enemys Group", true);
	if (is_new)
	{
		for (int i = 0; i < beacons.size(); i++)
		{
			Enemy *enemy;
			Vector3 pos = beacons[i]->get_position();
			float size = 2.0f;
			enemy = memnew(Enemy(pos, main_camera, size));
			enemy->set_name(vformat("Enemy_%d", i));
			ref_group->add_child(enemy);
			enemy->set_owner(get_tree()->get_edited_scene_root());
			enemys.push_back(enemy);
		}
	}
	*/
}
void CustomScene3501::setup_powerups()
{
	Node *ref_group;
	bool is_new = create_and_add_as_child<Node>(ref_group, "Speed PW Group", true);
	if (is_new)
	{
		RandomNumberGenerator *rng = memnew(RandomNumberGenerator);
		for (int i = 0; i < beacons.size() + floor(beacons.size() * 0.75); i++)
		{
			PowerUpSpeed *powerup;
			Vector3 pos;
			if (i < beacons.size())
				pos = beacons[i]->get_position();
			else
			{
				float x = rng->randf_range(min_feild.x, max_field.x);
				float y = rng->randf_range(min_feild.y, max_field.y);
				float z = rng->randf_range(min_feild.z, max_field.z);
				pos = Vector3(x, y, z);
			}
			float size = 1.0f;
			Color color = Color(0.0f, 1.0f, 0.0f, 1.0f);
			powerup = memnew(PowerUpSpeed(pos, color, size));
			powerup->set_name(vformat("Power Up Speed_%d", i));
			ref_group->add_child(powerup);
			powerup->set_owner(get_tree()->get_edited_scene_root());
			power_ups.push_back(powerup);
		}
	}
}

// this is just so that you have references when you are coding the camera movement.
// REMOVE THIS WHEN YOU GO TO MAKE THE RACETRACK
void CustomScene3501::setup_reference_boxes()
{
	// IMPORTANT !!!!!!! read the comments in this section if you want to understand more about adding nodes to a grouping node (so that you can collapse it in the GUI for the SceneTree)
	// also important: I am making the assumption that the whole group is there, or not.
	// I am adding this node so that you can minimize the entire group in the scene tree. It's a bit of an eyesore.
	Node *ref_group;
	bool is_new = create_and_add_as_child<Node>(ref_group, "Reference Points Group", true);
	if (is_new)
	{
		int num_references = 100;									// if you change this number for some reason, remove the CustomScene3501 from the scene tree because odds are it WILL crash.
		RandomNumberGenerator *rng = memnew(RandomNumberGenerator); // do not *need* to manually set the seed, unless you care about it being consistently the same between runs (also between the simulation and the editor)
		// rng->set_seed(999); // what is the effect of setting the seed, or not? experiment and/or check the docs if you are curious!

		for (int index = 0; index < num_references; index++)
		{
			ExampleDerivedClass *obj_instance;

			// This is an example of how to add a new node as a child of a child node of the CustomScene3501 node.
			// 		You could make a function VERY similar to create_and_add_as_child to do this block in a ****safer**** way.
			// 		If you want to be able to use the "grouping" demonstrated when making your racetrack, you probably should (but do not have to) (you can build the racetrack however you want).
			obj_instance = memnew(ExampleDerivedClass);
			obj_instance->set_name(vformat("Object_%d", index));
			ref_group->add_child(obj_instance);
			obj_instance->set_owner(get_tree()->get_edited_scene_root());
			// (up to here)

			float x = rng->randf_range(-100.0f, 100.0f);
			float y = rng->randf_range(-30.0f, 30.0f);
			float z = rng->randf_range(-50.0f, 50.0f);

			float r = rng->randf_range(0.0f, 1.0f);
			float g = rng->randf_range(0.0f, 1.0f);
			float b = rng->randf_range(0.0f, 1.0f);

			float x_dim = rng->randf_range(1.0f, 4.0f);
			float y_dim = rng->randf_range(1.0f, 4.0f);
			float z_dim = rng->randf_range(1.0f, 4.0f);
			BoxMesh *box = memnew(BoxMesh);
			box->set_size(Vector3(x_dim, y_dim, z_dim));
			StandardMaterial3D *box_material = memnew(StandardMaterial3D);
			box_material->set_albedo(Color(r, g, b, 1.0f));
			box->surface_set_material(0, box_material);
			obj_instance->set_mesh(box);
			obj_instance->set_global_position(Vector3(x, y, z));

			// every new simulation run/editor restart, this is new; we can safely push_back.
			// this is just to show you how to use the built-in Vector class
			reference_instances.push_back(obj_instance);
		}
	}
}

template <class T>
// returns true if pointer is brand-new; false if retrieved from SceneTree
bool CustomScene3501::create_and_add_as_child(T *&pointer, String name, bool search)
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

/*
 *
 * The usual container class added to the demos. This one should be used to create a racetrack.
 *
 * Copyright (c) 2024 Samantha Beilman (samanthabeilman@cmail.carleton.ca)
 *
 */