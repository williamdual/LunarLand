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
	// create_interactables();
	// create_env_objects();
	create_building_objects();
}

void CustomScene3501::_ready()
{
	if (DEBUG)
		UtilityFunctions::print("Ready - CustomScene3501.");

	// set the player's position (the camera)
	player->set_global_position(Vector3(0.0, -11.5, -20.0f));
	setup_cameras();

	// Setting up the test interactable
	// testInt->set_global_position(Vector3(3.0, -10.0, -15.0f));
	// testInt->set_global_rotation(Vector3(0.0, 0.785398, 0.0));
	// testInt->RegisterCameraTrigs(cam_triggs);
	// testInt->SetCameraPosition(cameras[2]->get_global_position());
	// testInt->AddLight(cameras[0]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);
	// testInt->AddLight(cameras[2]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);

	// Setting up the test environment object
	// testEnvObj->set_global_position(Vector3(3.0, -10.0, -10.0f));
	// testEnvObj->set_global_rotation(Vector3(0.0, 0.785398, 0.0));
	// testEnvObj->RegisterCameraTrigs(cam_triggs);
	// testEnvObj->SetCameraPosition(cameras[2]->get_global_position());
	// testEnvObj->AddLight(cameras[0]->get_global_position(), Vector3(1.0, 1.0, 1.0), 40.0);
	// testEnvObj->AddLight(cameras[2]->get_global_position(), Vector3(1.0, 1.0, 1.0), 40.0);

	// Setting up test counter interactable with lockout interactables
	// testCount->set_global_position(Vector3(6.0, -10.0, -15.0f));
	// testCount->RegisterCameraTrigs(cam_triggs);
	// testCount->SetCameraPosition(cameras[2]->get_global_position());
	// testCount->AddLight(cameras[0]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);
	// testCount->AddLight(cameras[2]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);

	// testLock1->set_global_position(Vector3(-6.0, -10.0, -15.0f));
	// testLock1->RegisterCameraTrigs(cam_triggs);
	// testLock1->SetCameraPosition(cameras[2]->get_global_position());
	// testLock1->AddLight(cameras[0]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);
	// testLock1->AddLight(cameras[2]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);

	// testLock2->set_global_position(Vector3(-6.0, -10.0, -5.0f));
	// testLock2->RegisterCameraTrigs(cam_triggs);
	// testLock2->SetCameraPosition(cameras[2]->get_global_position());
	// testLock2->AddLight(cameras[0]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);
	// testLock2->AddLight(cameras[2]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);

	// Setting up the test building
	testBuilding->set_scale(Vector3(1.5, 1.5, 1.5));
	testBuilding->set_global_position(Vector3(-0.023, -12.92, -5.635));
	testBuilding->RegisterCameraTrigs(cam_triggs);
	testBuilding->SetCameraPosition(cameras[2]->get_global_position());
	testBuilding->AddLight(cameras[0]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);
	testBuilding->AddLight(cameras[2]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);
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

	// All test stuff
	// testInt = memnew(AudioInteractable);
	// create_and_add_as_child(testInt, "Test Interactable", true);
	// testInt->SetValues(player, INTER_OBJECT_COMPUTER_TERMINAL_SCREEN, SHAPE_BOX, true, 3.0);
	// testInt->SetAudio(AUDIO_JOE_LAW_JOHNNY);

	// Additional test stuff
	
	// testCount = memnew(CounterInteractable);
	create_and_add_as_child(testCount, "CounterInteractable", true);
	testCount->SetValues(player, INTER_OBJECT_COMPUTER_TERMINAL_SCREEN, SHAPE_BOX, true, 2.0);
	testCount->SetCounter(0);
	testCount->SetTrigger(15);

	create_and_add_as_child(testLock1, "NoneLockout", true);
	testLock1->SetValues(player, INTER_OBJECT_COMPUTER_TERMINAL_SCREEN, SHAPE_BOX, true, 2.0);

	create_and_add_as_child(testLock2, "ItemLockout", true);
	testLock2->SetValues(player, INTER_OBJECT_COMPUTER_TERMINAL_SCREEN, SHAPE_BOX, true, 2.0);

	Vector<LockoutInteractable*> dependents;
	dependents.append(testLock1);
	dependents.append(testLock2);

	testLock1->SetLockout(ITEM_NONE, testCount, dependents);
	testLock2->SetLockout(ITEM_PAPERS, testCount, dependents);
}

// Member function to create environment objects
void CustomScene3501::create_env_objects() {
	// To be set when more of environment is ready

	// All test stuff
	testEnvObj = memnew(EnvObject);
	create_and_add_as_child(testEnvObj, "Test EnvObject", true);
	testEnvObj->SetValues(ENV_OBJECT_SMALL_STAGE, SHAPE_BOX);
}

// Member function to create building objects
void CustomScene3501::create_building_objects() {
	// To be set when more of environment is ready

	// All test stuff
	testBuilding = memnew(BuildingObj);
	create_and_add_as_child(testBuilding, "Test Building", true);
	testBuilding->SetValues(BUILDING_TEST_BUILDING, false);
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