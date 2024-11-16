#include "custom_scene_3501.h"
#include <godot_cpp/core/memory.hpp> // for memnew
#include <godot_cpp/classes/random_number_generator.hpp>

using namespace godot;

void CustomScene3501::_bind_methods() {}

CustomScene3501::CustomScene3501() : Node3D()
{
	time_passed = 0.0;
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

	bool is_new = create_and_add_as_child<Player>(player, "PlayerTestObject", true);
	if (is_new == true)
	{
	}
	create_cameras();
	create_interactables();
}

void CustomScene3501::_ready()
{
	if (DEBUG)
		UtilityFunctions::print("Ready - CustomScene3501.");

	// set the player's position (the camera)
	player->set_global_position(Vector3(0.0, -11.5, -10.0f));
	setup_cameras();


	testInt->set_global_position(Vector3(0.0, -10.0, -20.0f));
}

// called every frame (as often as possible)
void CustomScene3501::_process(double delta)
{
	if (Engine::get_singleton()->is_editor_hint())
		return; // Early return if we are in editor
				// Game loop stuff HERE
}

void CustomScene3501::create_cameras()
{
	PlayerCamera *cam_1;
	create_and_add_as_child<PlayerCamera>(cam_1, "Absalute Camera", true);
	cam_1->set_global_position(Vector3(0.0, 0.0, 0.0f));
	cam_1->SetTarget(player);
	CameraTrigger *trigg_1;
	create_and_add_as_child<CameraTrigger>(trigg_1, "cam_1_trigg_1", true);

	PlayerCamera *cam_2;
	create_and_add_as_child<PlayerCamera>(cam_2, "Pans Camera", true);
	cam_2->set_global_position(Vector3(7.2f, -10.6, -24.5f));
	cam_2->set_rotation_degrees(Vector3(-0.0f, 180.0f, 0.0f));
	cam_2->SetTarget(player);

	PlayerCamera *cam_3;
	create_and_add_as_child<PlayerCamera>(cam_3, "Stiff Camera", true);
	cam_3->set_global_position(Vector3(20.5f, 0.0, 0.0f));
	cam_3->set_rotation_degrees(Vector3(-23.0f, 46.0f, 0.0f));
	cam_3->SetTarget(player);
	CameraTrigger *trigg_3;
	create_and_add_as_child<CameraTrigger>(trigg_3, "cam_3_trigg_1", true);

	cameras.append(cam_1);
	cameras.append(cam_2);
	cameras.append(cam_3);
	cam_triggs.append(trigg_1);
	cam_triggs.append(trigg_3);
}
void CustomScene3501::setup_cameras()
{
	Node *cam_ref_group;
	bool is_new = create_and_add_as_child<Node>(cam_ref_group, "Cameras", true);
	Node *trigg_ref_group;
	create_and_add_as_child<Node>(trigg_ref_group, "Triggers", true);
	if (true)
	{
		for (int i = 0; i < cameras.size(); i++)
		{
			re_parent<Node, PlayerCamera>(cam_ref_group, cameras[i]);
			cameras[i]->_ready();
			if (i == 0)
			{
				cameras[i]->set_current(true);
				player->SetCamera(cameras[i]);
			}
		}
		for (int i = 0; i < cam_triggs.size(); i++)
		{
			re_parent<Node, CameraTrigger>(trigg_ref_group, cam_triggs[i]);
			cam_triggs[i]->_ready();
			cam_triggs[i]->SetPlayer(player);
			if (i == 0)
			{
				cam_triggs[i]->set_global_position(Vector3(5.0f, -12.0f, -6.0f));
				cam_triggs[i]->SetCamera(cameras[0]);
				cam_triggs[i]->setColliderTransformation(Vector3(1.1f, 5.0f, 3.3f));
			}
			else if (i == 1) // TODO change to approprite number later
			{
				cam_triggs[i]->set_global_position(Vector3(8.5f, -11.11f, -5.5f));
				cam_triggs[i]->SetCamera(cameras[2]);
				cam_triggs[i]->setColliderTransformation(Vector3(1.1f, 5.0f, 3.3f));
			}
			cam_triggs[i]->set_global_rotation_degrees(Vector3(0, 0, 0));
		}
	}
}

// Member function to create interactables
void CustomScene3501::create_interactables() {
	// To be set when more of the environment is ready
	testInt = memnew(AudioInteractable);
	create_and_add_as_child(testInt, "Test Interactable", true);
	testInt->SetValues(player, FILE_CABINET, SHAPE_BOX, true, 3.0);
	testInt->SetAudio(AUDIO_SUSIE_OPENING);
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

template <class T, class U>
void CustomScene3501::re_parent(T *parent, U *child)
{
	child->get_parent()->remove_child(child);
	parent->add_child(child);
	child->set_owner(get_tree()->get_edited_scene_root());
}

/*
 *
 * Skeleton for code previded by
 *
 * Copyright (c) 2024 Samantha Beilman (samanthabeilman@cmail.carleton.ca)
 *
 */