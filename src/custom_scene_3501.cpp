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
}

void CustomScene3501::_ready()
{
	if (DEBUG)
		UtilityFunctions::print("Ready - CustomScene3501.");

	// set the player's position (the camera)
	player->set_global_position(Vector3(0.0, -11.5, -10.0f));
	setup_cameras();
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
	cam_2->set_global_position(Vector3(7.2f, -9.7, -24.5f));
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
	Node *ref_group;
	bool is_new = create_and_add_as_child<Node>(ref_group, "Cameras", true);
	if (is_new)
	{
		for (int i = 0; i < cameras.size(); i++)
		{
			re_parent<Node, PlayerCamera>(ref_group, cameras[i]);
			cameras[i]->_ready();
			if (i == 0)
				cameras[i]->set_current(true);
		}
		for (int i = 0; i < cam_triggs.size(); i++)
		{
			cam_triggs[i]->_ready();
			cam_triggs[i]->SetPlayer(player);
			cam_triggs[i]->set_as_top_level(true); // if you remove this the game breaks into tiny little pieces PLEASE do not remove without substituting it with an equivalent
			if (i == 0)
			{
				cam_triggs[i]->set_global_position(Vector3(5.0f, -12.0f, -6.0f));
				re_parent<PlayerCamera, CameraTrigger>(cameras[0], cam_triggs[i]);
				cam_triggs[i]->SetCamera(cameras[0]);
				cam_triggs[i]->setColliderTransformation(Vector3(1.1f, 5.0f, 3.3f));
			}
			else if (i == 1) // TODO change to approprite number later
			{
				cam_triggs[i]->set_global_position(Vector3(-3.3f, -6.0f, -16.5f));
				re_parent<PlayerCamera, CameraTrigger>(cameras[2], cam_triggs[i]);
				cam_triggs[i]->SetCamera(cameras[2]);
				cam_triggs[i]->setColliderTransformation(Vector3(1.1f, 5.0f, 3.3f));
			}
			cam_triggs[i]->set_global_rotation_degrees(Vector3(0, 0, 0));
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