#include "custom_scene_3501.h"
#include <godot_cpp/core/memory.hpp> // for memnew
#include <godot_cpp/classes/random_number_generator.hpp>

using namespace godot;

void CustomScene3501::_bind_methods() {}

CustomScene3501::CustomScene3501() : Node3D()
{
	gameState = GameState::ongoing;
	time_passed = 0.0;
	start_static = 10.0 + rand() % 10;
	end_static = start_static + 1.0;
	is_static = false;
	paused = false;
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

	// screen quad is a child of the camera so that it will follow it around (even though the shader positions it into clip space, this can prevent culling)
	create_and_add_as_child<MeshInstance3D>(screen_quad_instance, "Static Quad", true);

	// Setup the screen-space shader
	QuadMesh *quad_mesh = memnew(QuadMesh);
	quad_mesh->set_size(Vector2(2, 2)); // this will cover the whole screen
	quad_mesh->set_flip_faces(true);

	screen_space_shader_material = memnew(ShaderMaterial);
	// make sure to tell your TA in your README how they should test different shaders; maybe it's to change the string below, maybe it's some other way of your own design
	Ref<Shader> shader = ResourceLoader::get_singleton()->load("Shaders/static.gdshader", "Shader"); // choose your screen space shader here
	screen_space_shader_material->set_shader(shader);
	quad_mesh->surface_set_material(0, screen_space_shader_material);
	screen_quad_instance->set_mesh(quad_mesh);
	screen_quad_instance->set_extra_cull_margin(50.0f); // as suggested in the Godot docs to prevent culling
	screen_space_shader_material->set_shader_parameter("static", is_static);

	// Creating the skybox
	skybox = memnew(SkyBox);
	create_and_add_as_child(skybox, "Sky Box", true);

	create_cameras();
	create_interactables();
	// create_env_objects();
	create_building_objects();

	// Testing particle system
	// create_particle_system("Blazing Fire", "lonefire");

	// Test terrain
	// When creating this terrain all this code must appear together
	// hmt = memnew(HeightMapTerrain);
	// create_and_add_as_child(hmt, "Test Height Map", true);
	// hmt->SetupHeightMap(TERRAIN_MOON, 20.0);
	// hmt->set_scale(Vector3(100.0, 100.0, 100.0));
	// hmt->set_global_position(Vector3(0,-20,0));
	// hmt->RegisterCameraTrigs(cam_triggs);
	// hmt->SetCameraPosition(cameras[2]->get_global_position());
	// hmt->AddLight(cameras[0]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);
	// hmt->AddLight(cameras[2]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);

	// Creating the mascot
	// create_and_add_as_child(mascot, "Mascot", true);
}

void CustomScene3501::_ready()
{
	if (DEBUG)
		UtilityFunctions::print("Ready - CustomScene3501.");

	// set the player's position (the camera)
	player->set_global_position(Vector3(0, -10.15762, -32.28558));
	setup_cameras();
	setup_interactables();

	// Setting up the test interactable
	// testInt->set_global_position(Vector3(0.0, -11.0, -35.0f));
	// testInt->set_global_rotation(Vector3(0.0, 0.785398, 0.0));
	// testInt->RegisterCameraTrigs(cam_triggs);
	// testInt->SetCameraPosition(cameras[2]->get_global_position());
	// testInt->AddLight(cameras[0]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);
	// testInt->AddLight(cameras[2]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);

	// Test item interactable
	// testItem->set_global_position(Vector3(3.0, -13.5, -15.0f));
	// testItem->set_global_rotation(Vector3(0.0, 0.785398, 0.0));
	// testItem->RegisterCameraTrigs(cam_triggs);
	// testItem->SetCameraPosition(cameras[2]->get_global_position());
	// testItem->AddLight(cameras[0]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);
	// testItem->AddLight(cameras[2]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);

	// Setting up the test environment object
	// testEnvObj->set_global_position(Vector3(3.0, -11.5, -10.0f));
	// testEnvObj->set_global_rotation(Vector3(0.0, 0.785398, 0.0));
	// testEnvObj->RegisterCameraTrigs(cam_triggs);
	// testEnvObj->SetCameraPosition(cameras[2]->get_global_position());
	// testEnvObj->AddLight(cameras[0]->get_global_position(), Vector3(1.0, 1.0, 1.0), 40.0);
	// testEnvObj->AddLight(cameras[2]->get_global_position(), Vector3(1.0, 1.0, 1.0), 40.0);

	// Setting up test counter interactable with lockout interactables
	// testCount->set_global_position(Vector3(6.0, -11.0, -15.0f));
	// testCount->RegisterCameraTrigs(cam_triggs);
	// testCount->SetCameraPosition(cameras[2]->get_global_position());
	// testCount->AddLight(cameras[0]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);
	// testCount->AddLight(cameras[2]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);

	// testLock1->set_global_position(Vector3(-6.0, -11.0, -15.0f));
	// testLock1->RegisterCameraTrigs(cam_triggs);
	// testLock1->SetCameraPosition(cameras[2]->get_global_position());
	// testLock1->AddLight(cameras[0]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);
	// testLock1->AddLight(cameras[2]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);

	// testLock2->set_global_position(Vector3(-6.0, -11.0, -5.0f));
	// testLock2->RegisterCameraTrigs(cam_triggs);
	// testLock2->SetCameraPosition(cameras[2]->get_global_position());
	// testLock2->AddLight(cameras[0]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);
	// testLock2->AddLight(cameras[2]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);

	// Setting up the test building
	// testBuilding->set_scale(Vector3(1.5, 1.5, 1.5));
	testBuilding->set_global_position(Vector3(-0.023, -12.92, -5.635));
	testBuilding->RegisterCameraTrigs(cam_triggs);
	testBuilding->SetCameraPosition(cameras[2]->get_global_position());
	testBuilding->AddLight(cameras[0]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0); // pos, color, specular compponent power
	testBuilding->AddLight(cameras[2]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);

	// Setting up the test particles
	// GPUParticles3D* particle_system = particle_systems[0];
	// ShaderMaterial* shader_material = dynamic_cast<ShaderMaterial*>(*particle_system->get_draw_pass_mesh(0)->surface_get_material(0));
	// particle_system->set_amount(20000);
	// shader_material->set_shader_parameter("texture_image", ResourceLoader::get_singleton()->load("res://Textures/flame4x4orig.png"));
	// particle_system->set_global_position(Vector3(-70, 0, 0));

	// Setting up the mascot
	// mascot->SetPosition(Vector3(3.0, -11.0, -15.0f));
	// //mascot->SetRotation(Vector3(0.0, 0.785398, 0.0));
	// mascot->RegisterCameraTrigs(cam_triggs);
	// mascot->SetCameraPosition(cameras[2]->get_global_position());
	// mascot->AddLight(cameras[0]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);
	// mascot->AddLight(cameras[2]->get_global_position(), Vector3(1.0, 1.0, 1.0), 5.0);
}

// called every frame (as often as possible)
void CustomScene3501::_process(double delta)
{
	if (Engine::get_singleton()->is_editor_hint())
		return; // Early return if we are in editor
				// Game loop stuff HERE

	// Increasing the time passed
	time_passed += delta;

	// Seeing whether or not to apply the camera glitch effect
	if (time_passed > start_static && time_passed < end_static)
	{
		screen_space_shader_material->set_shader_parameter("static", true);
	}
	else
	{
		screen_space_shader_material->set_shader_parameter("static", false);
	}

	// Setting a new time for the glitch effect
	if (time_passed > start_static && time_passed > end_static)
	{
		start_static = time_passed + 10.0 + rand() % 10;
		end_static = start_static + 1.0;
	}

	// Getting player input
	Input *_input = Input::get_singleton();

	// Getting if the game was paused
    if (_input->is_action_just_pressed("pause")) {
        paused = !paused;

		// Stopping audio if the game was paused
		if (paused) {

			// Audio interactables
			for (int i = 0; i < audio_interactables.size(); i++) {
				audio_interactables[i]->PauseAudio();
			}

			// Item interactables
			for (int i = 0; i < item_interactables.size(); i++) {
				item_interactables[i]->PauseAudio();
			}

			// Counter interactables
			for (int i = 0; i < counter_interactables.size(); i++) {
				counter_interactables[i]->PauseAudio();
			}

			// Lockout interactables
			for (int i = 0; i < lockout_interactables.size(); i++) {
				lockout_interactables[i]->PauseAudio();
			}

		// Unpausing audio
		} else {

			// Audio interactables
			for (int i = 0; i < audio_interactables.size(); i++) {
				audio_interactables[i]->ResumeAudio();
			}

			// Item interactables
			for (int i = 0; i < item_interactables.size(); i++) {
				item_interactables[i]->ResumeAudio();
			}

			// Counter interactables
			for (int i = 0; i < counter_interactables.size(); i++) {
				counter_interactables[i]->ResumeAudio();
			}

			// Lockout interactables
			for (int i = 0; i < lockout_interactables.size(); i++) {
				lockout_interactables[i]->ResumeAudio();
			}

		}
    } 

	// Checking if the game should end
	if (player->GetInventory()->GetCapacity() >= 3)
	{
		this->get_tree()->change_scene_to_file("res://end.tscn");
	}
}

void CustomScene3501::create_cameras()
{
	// Starting bulding
	PlayerCamera *cam_1;
	create_and_add_as_child<PlayerCamera>(cam_1, "SEC", true);
	cam_1->set_global_position(Vector3(4.632536, -1.755016, -26.03943));
	cam_1->SafelyRotate(Vector3(-48.20439, 48.67517, 0));
	cam_1->SetTrackType(CameraTrackType::statics);
	CameraTrigger *trigg_1;
	create_and_add_as_child<CameraTrigger>(trigg_1, "SEC_trigg_1", true);

	PlayerCamera *cam_2;
	create_and_add_as_child<PlayerCamera>(cam_2, "SHC", true);
	cam_2->set_global_position(Vector3(-4.818558, -9.923786, -39.83393));
	cam_2->SafelyRotate(Vector3(-0.704379, -88.32508, 0));
	cam_2->SetTrackType(CameraTrackType::panning);
	CameraTrigger *trigg_2;
	create_and_add_as_child<CameraTrigger>(trigg_2, "SHC_trigg_1", true);
	CameraTrigger *trigg_3;
	create_and_add_as_child<CameraTrigger>(trigg_3, "SHC_trigg_2", true);

	PlayerCamera *cam_3;
	create_and_add_as_child<PlayerCamera>(cam_3, "SBC", true);
	cam_3->set_global_position(Vector3(-0.176132, 23.11914, -75.44937));
	// cam_3->SafelyRotate(Vector3(-90.0, 0, 0));
	cam_3->SetTrackType(CameraTrackType::tracking);
	CameraTrigger *trigg_4;
	create_and_add_as_child<CameraTrigger>(trigg_4, "SBC_trigg_1", true);
	CameraTrigger *trigg_5;
	create_and_add_as_child<CameraTrigger>(trigg_5, "SBC_trigg_2", true);
	CameraTrigger *trigg_6;
	create_and_add_as_child<CameraTrigger>(trigg_6, "SBC_trigg_3", true);
	CameraTrigger *trigg_7;
	create_and_add_as_child<CameraTrigger>(trigg_7, "SBC_trigg_4", true);

	// Wiliams Cameras
	PlayerCamera *cam_4;
	create_and_add_as_child<PlayerCamera>(cam_4, "WHC", true);
	cam_4->set_global_position(Vector3(7.65308, -8.397577, -95.25987));
	cam_4->SafelyRotate(Vector3(-0.45437, 90.15166, 0));
	cam_4->SetTrackType(CameraTrackType::panning);
	CameraTrigger *trigg_8;
	create_and_add_as_child<CameraTrigger>(trigg_8, "WHC_trigg_1", true);
	CameraTrigger *trigg_9;
	create_and_add_as_child<CameraTrigger>(trigg_9, "WHC_trigg_2", true);

	PlayerCamera *cam_5;
	create_and_add_as_child<PlayerCamera>(cam_5, "WEC", true);
	cam_5->set_global_position(Vector3(-8.092813, -2.793831, -138.3976));
	// cam_5->SafelyRotate(Vector3(-41.7042, -18.33477, 0));
	cam_5->SetTrackType(CameraTrackType::tracking);
	CameraTrigger *trigg_10;
	create_and_add_as_child<CameraTrigger>(trigg_10, "WEC_trigg_1", true);
	CameraTrigger *trigg_11;
	create_and_add_as_child<CameraTrigger>(trigg_11, "WEC_trigg_2", true);
	CameraTrigger *trigg_12;
	create_and_add_as_child<CameraTrigger>(trigg_12, "WEC_trigg_3", true);
	CameraTrigger *trigg_13;
	create_and_add_as_child<CameraTrigger>(trigg_13, "WEC_trigg_4", true);
	CameraTrigger *trigg_14;
	create_and_add_as_child<CameraTrigger>(trigg_14, "WEC_trigg_5", true);

	PlayerCamera *cam_6; // Cafeateria cam
	create_and_add_as_child<PlayerCamera>(cam_6, "WCC", true);
	cam_6->set_global_position(Vector3(-36.53674, -6.390055, -189.2349));
	cam_6->SafelyRotate(Vector3(0, 180, 0));
	cam_6->SetTrackType(CameraTrackType::statics);
	CameraTrigger *trigg_15;
	create_and_add_as_child<CameraTrigger>(trigg_15, "WCC_trigg_1", true);
	CameraTrigger *trigg_16;
	create_and_add_as_child<CameraTrigger>(trigg_16, "WCC_trigg_2", true);

	PlayerCamera *cam_7;
	create_and_add_as_child<PlayerCamera>(cam_7, "WBoBoC", true);
	cam_7->set_global_position(Vector3(0.652023, -5.790375, -173.8483));
	cam_7->SafelyRotate(Vector3(-31.7042, -90.0, 0));
	cam_7->SetTrackType(CameraTrackType::panning);
	CameraTrigger *trigg_17;
	create_and_add_as_child<CameraTrigger>(trigg_17, "WBoBoC_trigg_1", true);

	PlayerCamera *cam_8;
	create_and_add_as_child<PlayerCamera>(cam_8, "WELC", true);
	cam_8->set_global_position(Vector3(10.29093, -0.882858, -182.5928));
	cam_8->SafelyRotate(Vector3(-22.20435, 3.440094, 0));
	cam_8->SetTrackType(CameraTrackType::statics);
	CameraTrigger *trigg_18;
	create_and_add_as_child<CameraTrigger>(trigg_18, "WELC_trigg_1", true);
	CameraTrigger *trigg_19;
	create_and_add_as_child<CameraTrigger>(trigg_19, "WELC_trigg_2", true);
	CameraTrigger *trigg_20;
	create_and_add_as_child<CameraTrigger>(trigg_20, "WELC_trigg_3", true);
	CameraTrigger *trigg_21;
	create_and_add_as_child<CameraTrigger>(trigg_21, "WELC_trigg_4", true);
	CameraTrigger *trigg_22;
	create_and_add_as_child<CameraTrigger>(trigg_22, "WELC_trigg_5", true);
	CameraTrigger *trigg_23;
	create_and_add_as_child<CameraTrigger>(trigg_23, "WELC_trigg_6", true);
	CameraTrigger *trigg_24;
	create_and_add_as_child<CameraTrigger>(trigg_24, "WELC_trigg_7", true);

	PlayerCamera *cam_9; // Pool Cam
	create_and_add_as_child<PlayerCamera>(cam_9, "W_pool_C", true);
	cam_9->set_global_position(Vector3(21.46057, -0.807379, -207.8787));
	cam_9->SafelyRotate(Vector3(-35.79578, -130.5289, 0));
	cam_9->SetTrackType(CameraTrackType::statics);
	CameraTrigger *trigg_25;
	create_and_add_as_child<CameraTrigger>(trigg_25, "W_pool_C_trigg_1", true);

	PlayerCamera *cam_10; // Pasta Cam
	create_and_add_as_child<PlayerCamera>(cam_10, "W_pasta_C", true);
	cam_10->set_global_position(Vector3(-34.69436, -5.034119, -220.5239));
	cam_10->SafelyRotate(Vector3(-47.45443, -115.7982, 0));
	cam_10->SetTrackType(CameraTrackType::statics);
	CameraTrigger *trigg_26;
	create_and_add_as_child<CameraTrigger>(trigg_26, "W_pasta_C_trigg_1", true);

	PlayerCamera *cam_11; // Steals Cam
	create_and_add_as_child<PlayerCamera>(cam_11, "W_steals_C", true);
	cam_11->set_global_position(Vector3(3.999357, 1.749868, -267.9107));
	cam_11->SafelyRotate(Vector3(-24.04568, 7.460113, 0));
	cam_11->SetTrackType(CameraTrackType::statics);
	CameraTrigger *trigg_27;
	create_and_add_as_child<CameraTrigger>(trigg_27, "W_steals_C_trigg_1", true);

	PlayerCamera *cam_12; // Security Office Cam
	create_and_add_as_child<PlayerCamera>(cam_12, "W_office_C", true);
	cam_12->set_global_position(Vector3(-19.39669, 0.244593, -228.493));
	// cam_12->SafelyRotate(Vector3(-31.29581, 41.95086, 0));
	cam_12->SetTrackType(CameraTrackType::tracking);
	CameraTrigger *trigg_28;
	create_and_add_as_child<CameraTrigger>(trigg_28, "W_office_C_trigg_1", true);

	cameras.append(cam_1);
	cameras.append(cam_2);
	cameras.append(cam_3);
	cameras.append(cam_4);
	cameras.append(cam_5);
	cameras.append(cam_6);
	cameras.append(cam_7);
	cameras.append(cam_8);
	cameras.append(cam_9);
	cameras.append(cam_10);
	cameras.append(cam_11);
	cameras.append(cam_12);
	cam_triggs.append(trigg_1);
	cam_triggs.append(trigg_2);
	cam_triggs.append(trigg_3);
	cam_triggs.append(trigg_4);
	cam_triggs.append(trigg_5);
	cam_triggs.append(trigg_6);
	cam_triggs.append(trigg_7);
	cam_triggs.append(trigg_8);
	cam_triggs.append(trigg_9);
	cam_triggs.append(trigg_10);
	cam_triggs.append(trigg_11);
	cam_triggs.append(trigg_12);
	cam_triggs.append(trigg_13);
	cam_triggs.append(trigg_14);
	cam_triggs.append(trigg_15);
	cam_triggs.append(trigg_16);
	cam_triggs.append(trigg_17);
	cam_triggs.append(trigg_18);
	cam_triggs.append(trigg_19);
	cam_triggs.append(trigg_20);
	cam_triggs.append(trigg_21);
	cam_triggs.append(trigg_22);
	cam_triggs.append(trigg_23);
	cam_triggs.append(trigg_24);
	cam_triggs.append(trigg_25);
	cam_triggs.append(trigg_26);
	cam_triggs.append(trigg_27);
	cam_triggs.append(trigg_28);

	// Jacobs Cameras
	PlayerCamera *cam_13;
	create_and_add_as_child<PlayerCamera>(cam_13, "JEC", true);
	cam_13->set_global_position(Vector3(66.74512, -2.127899, -64.9141));
	// cam_13->SafelyRotate(Vector3(-21.20438, 50.28828, 0));
	cam_13->SetTrackType(CameraTrackType::tracking);
	CameraTrigger *trigg_29;
	create_and_add_as_child<CameraTrigger>(trigg_29, "JEC_trigg_1", true);
	CameraTrigger *trigg_30;
	create_and_add_as_child<CameraTrigger>(trigg_30, "JEC_trigg_2", true);
	CameraTrigger *trigg_31;
	create_and_add_as_child<CameraTrigger>(trigg_31, "JEC_trigg_3", true);

	PlayerCamera *cam_14;
	create_and_add_as_child<PlayerCamera>(cam_14, "JCC", true);
	cam_14->set_global_position(Vector3(167.4163, 43.29909, -98.47244));
	cam_14->SetTrackType(CameraTrackType::tracking);
	CameraTrigger *trigg_32;
	create_and_add_as_child<CameraTrigger>(trigg_32, "JCC_trigg_1", true);
	CameraTrigger *trigg_33;
	create_and_add_as_child<CameraTrigger>(trigg_33, "JCC_trigg_2", true);

	cameras.append(cam_13);
	cameras.append(cam_14);
	cam_triggs.append(trigg_29);
	cam_triggs.append(trigg_30);
	cam_triggs.append(trigg_31);
	cam_triggs.append(trigg_32);
	cam_triggs.append(trigg_33);
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
			cameras[i]->SetTarget(player);
			cameras[i]->_ready();
			if (i == 0)
			{
				cameras[i]->set_current(true);
				player->SetCamera(cameras[i]);
			}
		}
		Vector3 right_angle_turn = Vector3(0, 90, 0);
		Vector3 long_box = Vector3(1.1f, 10.0f, 23.8f);
		Vector3 cube_box = Vector3(19.513f, 10.0f, 16.016f); // Pool trigger shape
		Vector3 giant_rect = Vector3(2.0, 10.0f, 100);
		for (int i = 0; i < cam_triggs.size(); i++)
		{
			re_parent<Node, CameraTrigger>(trigg_ref_group, cam_triggs[i]);
			cam_triggs[i]->_ready();
			cam_triggs[i]->SetPlayer(player);
			cam_triggs[i]->setColliderTransformation(Vector3(1.1f, 10.0f, 5.3f)); // default, can change per collider if desired
			cam_triggs[i]->set_global_rotation_degrees(Vector3(0, 0, 0));
			if (i == 0) // SEC Trig 1
			{
				cam_triggs[i]->set_global_position(Vector3(0, -11.5, -35));
				cam_triggs[i]->set_rotation_degrees(right_angle_turn);
				cam_triggs[i]->SetCamera(cameras[0]);
			}
			if (i == 1) // SHC Trig 1 and 2
			{
				cam_triggs[i]->set_global_position(Vector3(0, -11.5, -39.02035));
				cam_triggs[i]->set_global_rotation_degrees(right_angle_turn);
				cam_triggs[i]->SetCamera(cameras[1]);
			}
			else if (i == 2)
			{
				cam_triggs[i]->set_global_position(Vector3(0, -11.5, -58.64443));
				cam_triggs[i]->set_rotation_degrees(right_angle_turn);
				cam_triggs[i]->SetCamera(cameras[1]);
			}
			else if (i == 3) // SBC Trig 1-5
			{
				cam_triggs[i]->set_global_position(Vector3(0.23662, -11.5, -87.31493));
				cam_triggs[i]->set_rotation_degrees(right_angle_turn);
				cam_triggs[i]->SetCamera(cameras[2]);
			}
			else if (i == 4)
			{
				cam_triggs[i]->set_global_position(Vector3(0, -11.5, -63.13406));
				cam_triggs[i]->set_rotation_degrees(right_angle_turn);
				cam_triggs[i]->SetCamera(cameras[2]);
			}
			else if (i == 5)
			{
				cam_triggs[i]->set_global_position(Vector3(-10.23, -11.5, -69.77975));
				cam_triggs[i]->SetCamera(cameras[2]);
			}
			else if (i == 6)
			{
				cam_triggs[i]->set_global_position(Vector3(9.455, -11.5, -81.07484));
				cam_triggs[i]->SetCamera(cameras[2]);
			}
			else if (i == 7) // WHC Trig 1 and 2
			{
				cam_triggs[i]->set_global_position(Vector3(1.043307, -11.5, -93.37697));
				cam_triggs[i]->set_rotation_degrees(right_angle_turn);
				cam_triggs[i]->SetCamera(cameras[3]);
			}
			else if (i == 8)
			{
				cam_triggs[i]->set_global_position(Vector3(1.043307, -11.5, -134.7595));
				cam_triggs[i]->set_rotation_degrees(right_angle_turn);
				cam_triggs[i]->SetCamera(cameras[3]);
			}
			else if (i == 9) // Employe lounge main area
			{
				cam_triggs[i]->set_global_position(Vector3(0, -11.5, -140.3351));
				cam_triggs[i]->set_rotation_degrees(right_angle_turn);
				cam_triggs[i]->SetCamera(cameras[4]);
			}
			else if (i == 10)
			{
				cam_triggs[i]->set_global_position(Vector3(-19.71, -11.5, -146.5542));
				cam_triggs[i]->SetCamera(cameras[4]);
			}
			else if (i == 11)
			{
				cam_triggs[i]->set_global_position(Vector3(-20.198, -11.5, -177.7186));
				cam_triggs[i]->SetCamera(cameras[4]);
			}
			else if (i == 12)
			{
				cam_triggs[i]->set_global_position(Vector3(1.243914, -11.5, -173.0238));
				cam_triggs[i]->SetCamera(cameras[4]);
			}
			else if (i == 13)
			{
				cam_triggs[i]->set_global_position(Vector3(-7.495, -11.5, -180.482));
				cam_triggs[i]->setColliderTransformation(long_box);
				cam_triggs[i]->set_rotation_degrees(right_angle_turn);
				cam_triggs[i]->SetCamera(cameras[4]);
			}
			else if (i == 14) // Cafeteria triggs
			{
				cam_triggs[i]->set_global_position(Vector3(-23.8, -11.5, -146.5542));
				cam_triggs[i]->SetCamera(cameras[5]);
			}
			else if (i == 15)
			{
				cam_triggs[i]->set_global_position(Vector3(-23.8, -11.5, -177.7186));
				cam_triggs[i]->SetCamera(cameras[5]);
			}
			else if (i == 16) // Bobos tent trigg
			{
				cam_triggs[i]->set_global_position(Vector3(6.230962, -11.5, -173.556));
				cam_triggs[i]->SetCamera(cameras[6]);
			}
			else if (i == 17) // Northern Employee Lounge
			{
				cam_triggs[i]->set_global_position(Vector3(-7.495, -11.5, -183.877));
				cam_triggs[i]->set_rotation_degrees(right_angle_turn);
				cam_triggs[i]->setColliderTransformation(long_box);
				cam_triggs[i]->SetCamera(cameras[7]);
			}
			else if (i == 18)
			{
				cam_triggs[i]->set_global_position(Vector3(-18.24476, -11.5, -214.492));
				cam_triggs[i]->setColliderTransformation(long_box);
				cam_triggs[i]->SetCamera(cameras[7]);
			}
			else if (i == 19)
			{
				cam_triggs[i]->set_global_position(Vector3(2.575, -11.5, -268.966));
				cam_triggs[i]->setColliderTransformation(long_box);
				cam_triggs[i]->set_rotation_degrees(right_angle_turn);
				cam_triggs[i]->SetCamera(cameras[7]);
			}
			else if (i == 20)
			{
				cam_triggs[i]->set_global_position(Vector3(22.506, -11.5, -192.554));
				cam_triggs[i]->setColliderTransformation(long_box);
				cam_triggs[i]->set_rotation_degrees(right_angle_turn);
				cam_triggs[i]->SetCamera(cameras[7]);
			}
			else if (i == 21)
			{
				cam_triggs[i]->set_global_position(Vector3(17.213, -11.5, -210.88));
				cam_triggs[i]->setColliderTransformation(long_box);
				cam_triggs[i]->set_rotation_degrees(right_angle_turn);
				cam_triggs[i]->SetCamera(cameras[7]);
			}
			else if (i == 22)
			{
				cam_triggs[i]->set_global_position(Vector3(17.21342, -11.5, -196.1647));
				cam_triggs[i]->setColliderTransformation(long_box);
				cam_triggs[i]->SetCamera(cameras[7]);
			}
			else if (i == 23)
			{
				cam_triggs[i]->set_global_position(Vector3(-12.03628, -10.15762, -248.1444));
				cam_triggs[i]->setColliderTransformation(long_box);
				cam_triggs[i]->SetCamera(cameras[7]);
			}
			else if (i == 24) // Pool cam trigger
			{
				cam_triggs[i]->set_global_position(Vector3(27.884, -11.5, -201.195));
				cam_triggs[i]->setColliderTransformation(cube_box);
				cam_triggs[i]->SetCamera(cameras[8]);
			}
			else if (i == 25) // PASTA Trigger
			{
				cam_triggs[i]->set_global_position(Vector3(-22.51009, -11.5, -214.362));
				cam_triggs[i]->setColliderTransformation(long_box);
				cam_triggs[i]->SetCamera(cameras[9]);
			}
			else if (i == 26) // STEALS Trigger
			{
				cam_triggs[i]->set_global_position(Vector3(2.575, -11.5, -276.073));
				cam_triggs[i]->setColliderTransformation(long_box);
				cam_triggs[i]->set_rotation_degrees(right_angle_turn);
				cam_triggs[i]->SetCamera(cameras[10]);
			}
			else if (i == 27) // Security Office Trigger
			{
				cam_triggs[i]->set_global_position(Vector3(-17.02117, -10.15762, -248.1444));
				cam_triggs[i]->setColliderTransformation(long_box);
				cam_triggs[i]->SetCamera(cameras[11]);
			}
			else if (i == 28) // JEC Triggers FIRST JACOB TRIGGER ---------------------------------------------------------------------------------
			{
				cam_triggs[i]->set_global_position(Vector3(16, -10.158, -82.563));
				cam_triggs[i]->SetCamera(cameras[12]);
			}
			else if (i == 29)
			{
				cam_triggs[i]->set_global_position(Vector3(138.2366, -8.6, -60.5618));
				cam_triggs[i]->setColliderTransformation(giant_rect);
				cam_triggs[i]->SetCamera(cameras[12]);
			}
			else if (i == 30)
			{
				cam_triggs[i]->set_global_position(Vector3(104.6677, -8.6, -92.3312));
				cam_triggs[i]->setColliderTransformation(giant_rect);
				cam_triggs[i]->set_rotation_degrees(right_angle_turn);
				cam_triggs[i]->SetCamera(cameras[12]);
			}
			else if (i == 31) // JCC Triggers
			{
				cam_triggs[i]->set_global_position(Vector3(150.2366, -0.151898, -60.5618));
				cam_triggs[i]->setColliderTransformation(giant_rect);
				cam_triggs[i]->SetCamera(cameras[13]);
			}
			else if (i == 32)
			{
				cam_triggs[i]->set_global_position(Vector3(104.6677, -0.151898, -100.0273));
				cam_triggs[i]->setColliderTransformation(giant_rect);
				cam_triggs[i]->set_rotation_degrees(right_angle_turn);
				cam_triggs[i]->SetCamera(cameras[13]);
			}
		}
	}
}

// Member function to create interactables
void CustomScene3501::create_interactables()
{
	AudioInteractable *log_1;
	create_and_add_as_child(log_1, "log_joe_intro", true);
	AudioInteractable *log_2;
	create_and_add_as_child(log_2, "log_joe_Johnny", true);
	AudioInteractable *log_3;
	create_and_add_as_child(log_3, "log_joe_Bobo", true);
	AudioInteractable *log_4;
	create_and_add_as_child(log_4, "log_joe_Timmy", true);
	AudioInteractable *log_5;
	create_and_add_as_child(log_5, "log_Johnny_Timmy", true);
	AudioInteractable *log_6;
	create_and_add_as_child(log_6, "log_Generator", true);
	AudioInteractable *log_7;
	create_and_add_as_child(log_7, "log_Bobo_Timmy", true);

	audio_interactables.append(log_1);
	audio_interactables.append(log_2);
	audio_interactables.append(log_3);
	audio_interactables.append(log_4);
	audio_interactables.append(log_5);
	audio_interactables.append(log_6);
	audio_interactables.append(log_7);

	// To be set when more of the environment is ready

	// Audio interactable test stuff
	// testInt = memnew(AudioInteractable);
	// create_and_add_as_child(testInt, "Test Interactable", true);
	// testInt->SetValues(player, INTER_OBJECT_STEALS_TIMMY, SHAPE_BOX, true, 3.0);
	// testInt->SetAudio(AUDIO_JOHNNY_TIMMY);

	// Test item interactable
	// testItem = memnew(ItemInteractable);
	// create_and_add_as_child(testItem, "Test Interactable", true);
	// testItem->SetValues(player, INTER_OBJECT_BEIGE_BAG, SHAPE_BOX, true, 3.0);
	// testItem->SetItem(ITEM_PAPERS);

	// Additional test stuff

	// create_and_add_as_child(testCount, "CounterInteractable", true);
	// testCount->SetValues(player, INTER_OBJECT_COMPUTER_TERMINAL_SCREEN, SHAPE_BOX, true, 5.0);
	// testCount->SetInit(0, 15);
	// testCount->SetCounter(0);
	// testCount->SetTrigger(15);

	// create_and_add_as_child(testLock1, "NoneLockout", true);
	// testLock1->SetValues(player, INTER_OBJECT_COMPUTER_TERMINAL_SCREEN, SHAPE_BOX, true, 5.0);

	// create_and_add_as_child(testLock2, "ItemLockout", true);
	// testLock2->SetValues(player, INTER_OBJECT_COMPUTER_TERMINAL_SCREEN, SHAPE_BOX, true, 5.0);

	// Vector<LockoutInteractable*> dependents;
	// dependents.append(testLock1);
	// dependents.append(testLock2);

	// testLock1->SetLockout(ITEM_NONE, testCount, dependents);
	// testLock2->SetLockout(ITEM_PAPERS, testCount, dependents);
}

void CustomScene3501::setup_interactables()
{
	Node *interact_ref_group;
	bool is_new = create_and_add_as_child<Node>(interact_ref_group, "Interactables", true);
	Vector3 right_angle = Vector3(0, 90, 0);
	if (true)
	{
		for (int i = 0; i < audio_interactables.size(); i++) // Audio Logs
		{
			//re_parent<Node, AudioInteractable>(interact_ref_group, audio_interactables[i]);
			//audio_interactables[i]->_ready();
			if (i == 0) // Williams Audio Logs
			{
				audio_interactables[i]->set_global_position(Vector3(-22.48615, -7.101642, -254.866));
				audio_interactables[i]->SetValues(player, INTER_OBJECT_COMPUTER_TERMINAL_SCREEN, SHAPE_BOX, true, 7.0);
				audio_interactables[i]->SetAudio(AUDIO_JOE_LAW_INTRO);
			}
			else if (i == 1)
			{
				audio_interactables[i]->set_global_position(Vector3(-30.0, -7.409349, -250.75));
				audio_interactables[i]->SetValues(player, INTER_OBJECT_COMPUTER_TERMINAL_SCREEN, SHAPE_BOX, true, 7.0);
				audio_interactables[i]->set_global_rotation_degrees(right_angle);
				audio_interactables[i]->SetAudio(AUDIO_JOE_LAW_JOHNNY);
			}
			else if (i == 2)
			{
				audio_interactables[i]->set_global_position(Vector3(-30.0, -7.409349, -241.1618));
				audio_interactables[i]->SetValues(player, INTER_OBJECT_COMPUTER_TERMINAL_SCREEN, SHAPE_BOX, true, 7.0);
				audio_interactables[i]->set_global_rotation_degrees(right_angle);
				audio_interactables[i]->SetAudio(AUDIO_JOE_LAW_BOBO);
			}
			else if (i == 3)
			{
				audio_interactables[i]->set_global_position(Vector3(-30.0, -7.409349, -232.7377));
				audio_interactables[i]->SetValues(player, INTER_OBJECT_COMPUTER_TERMINAL_SCREEN, SHAPE_BOX, true, 7.0);
				audio_interactables[i]->set_global_rotation_degrees(right_angle);
				audio_interactables[i]->SetAudio(AUDIO_JOE_LAW_TIMMY);
			}
			else if (i == 4)
			{
				audio_interactables[i]->set_global_position(Vector3(-39.23154, -7.359558, -186.8915));
				audio_interactables[i]->SetValues(player, INTER_OBJECT_COMPUTER_TERMINAL_SCREEN, SHAPE_BOX, true, 6.0);
				audio_interactables[i]->set_global_rotation_degrees(Vector3(0, 180, 0));
				audio_interactables[i]->SetAudio(AUDIO_JOHNNY_TIMMY);
			}
			else if (i == 5)
			{
				audio_interactables[i]->set_global_position(Vector3(-12.75724, -10.04689, -283.267));
				audio_interactables[i]->SetValues(player, INTER_OBJECT_COMPUTER_TERMINAL_SCREEN, SHAPE_BOX, true, 4.0);
				audio_interactables[i]->set_global_rotation_degrees(right_angle);
				audio_interactables[i]->SetAudio(AUDIO_JOE_LAW_GENERATOR);
			}
			else if (i == 6)
			{
				audio_interactables[i]->set_global_position(Vector3(21.46068, -10.4836, -167.2278));
				audio_interactables[i]->SetValues(player, INTER_OBJECT_COMPUTER_TERMINAL_SCREEN, SHAPE_BOX, true, 4.0);
				audio_interactables[i]->set_global_rotation_degrees(-right_angle);
				audio_interactables[i]->SetAudio(AUDIO_BOBO_TIMMY);
			}
			re_parent<Node, AudioInteractable>(interact_ref_group, audio_interactables[i]);
		}
	}
}

void CustomScene3501::create_lights()
{
}

// Member function to create environment objects
void CustomScene3501::create_env_objects()
{
	// To be set when more of environment is ready

	// All test stuff
	testEnvObj = memnew(EnvObject);
	create_and_add_as_child(testEnvObj, "Test EnvObject", true);
	testEnvObj->SetValues(ENV_OBJECT_LOST_AND_FOUND, SHAPE_BOX);
}

// Member function to create building objects
void CustomScene3501::create_building_objects()
{
	// To be set when more of environment is ready

	// All test stuff
	testBuilding = memnew(BuildingObj);
	create_and_add_as_child(testBuilding, "Test Building", true);
	// testBuilding->SetValues(BUILDING_TEST_BUILDING, false, true);
	testBuilding->SetValues(BUILDING_MAP_BUILDING, true, false);
}

// it felt a bit cleaner in my eyes to bundle this together
// not full file name for the shader; see the particle system code for more detail
void CustomScene3501::create_particle_system(String node_name, String shader_name)
{
	// if you want to use non-zero argument constructors, here is an example of how to do that
	ParticleSystem3501 *system = memnew(ParticleSystem3501(shader_name));
	add_as_child(system, node_name, true);
	particle_systems.push_back(system);
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

// This is a variant of the usual one. It allows you to more easily use a non-zero argument constructor, which I noticed some of you have struggled with. Hope this helps!
// returns true if pointer is brand-new; false if retrieved from SceneTree
// deletes the memory if the node exists in the scenetree and isn't null when passed in
// IMPORTANT: IF SEARCH IS FALSE, IT ASSUMES THAT THE POINTER IS TO A VALID INSTANCE ALREADY AKA MEMNEW HAS ALREADY BEEN CALLED
template <class T>
bool CustomScene3501::add_as_child(T *&pointer, String name, bool search)
{
	// this is the default behaviour
	// added the search parameter so that we can skip the slow "find_child" call during runtime
	if (search == false)
	{
		pointer->set_name(name);
		add_child(pointer);
		pointer->set_owner(get_tree()->get_edited_scene_root());
		return true;
	}

	// always only have to search once if we save it here
	Node *child = find_child(name);

	// if the node hasn't been added to the SceneTree yet
	if (child == nullptr)
	{
		pointer->set_name(name);
		add_child(pointer);
		pointer->set_owner(get_tree()->get_edited_scene_root());
		return true;
	}
	// if we are grabbing the existent one, clean up the memory to the new one that was just made and passed as an argument
	else
	{
		if (pointer == nullptr)
		{
			UtilityFunctions::print("There is a nullptr being passed to add_as_child...");
		}
		else
		{
			memdelete(pointer);
		}
		pointer = dynamic_cast<T *>(child);
		return false;
	}
}

template <class T>
// returns true if pointer is brand-new; false if retrieved from SceneTree
// variant allows you to create a child of a node pointer other than 'this'
bool CustomScene3501::create_and_add_as_child_of_parent(T *&pointer, String name, Node *parent)
{
	Node *child = parent->find_child(name);

	if (child == nullptr)
	{
		pointer = memnew(T);
		pointer->set_name(name);
		parent->add_child(pointer);
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