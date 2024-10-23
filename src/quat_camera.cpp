#include "quat_camera.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event.hpp>

using namespace godot;

void QuatCamera::_bind_methods() {}

QuatCamera::QuatCamera(float r) : Camera3D()
{
	// Initialize any variables here.
	our_quaternion = Quaternion(Vector3(0, 0, 1), 0.0f);
	radius = r;
	speed = 4.0f;
}

void QuatCamera::_enter_tree()
{
}

void QuatCamera::_ready()
{
	// this is the Godot-assisted equivalent of SetView
	// Setup the initial unit vectors for each
	// using this setup allows the user to set the position of the camera in the editor.

	// Don't change this
	forward_ = Vector3(get_global_transform().basis[0][2], get_global_transform().basis[1][2], get_global_transform().basis[2][2]);
	side_ = Vector3(get_global_transform().basis[0][0], get_global_transform().basis[1][0], get_global_transform().basis[2][0]);
	// do not save _up as a member -- derive it. You will get a bad grade if you add it as a member variable like these two are.
}

QuatCamera::~QuatCamera()
{
	// Add your cleanup here.
}

void QuatCamera::_process(double delta)
{
	if (Engine::get_singleton()->is_editor_hint())
		return; // Early return if we are in editor

	// you can speed up by changing these, if desired.
	float rotation_factor = 8.0f;
	float translation_factor = 4.0f;

	Input *_input = Input::get_singleton();
	set_position(get_position() + GetForward() * delta * speed);

	if (_input->is_action_pressed("yaw_increase"))
	{
		Yaw(1.0f * delta * rotation_factor);
	}
	if (_input->is_action_pressed("yaw_decrease"))
	{
		Yaw(-1.0f * delta * rotation_factor);
	}
	if (_input->is_action_pressed("pitch_increase"))
	{
		Pitch(-1.0f * delta * rotation_factor);
	}
	if (_input->is_action_pressed("pitch_decrease"))
	{
		Pitch(1.0f * delta * rotation_factor);
	}
	if (_input->is_action_pressed("roll_increase"))
	{
		Roll(1.0f * delta * rotation_factor);
	}
	if (_input->is_action_pressed("roll_decrease"))
	{
		Roll(-1.0f * delta * rotation_factor);
	}
}

Vector3 QuatCamera::GetForward(void) const
{
	Vector3 current_forward = (our_quaternion.xform(forward_));
	return -current_forward.normalized(); // Return -forward since the camera coordinate system points in the opposite direction
}

Vector3 QuatCamera::GetSide(void) const
{
	// what should this be replaced with?
	Vector3 current_side = (our_quaternion.xform(side_));
	return current_side;
}

Vector3 QuatCamera::GetUp(void) const
{
	// how do you get the up vector?
	Vector3 current_up = vec3_cross(GetSide(), GetForward());
	return current_up;
}

void QuatCamera::Pitch(float angle)
{
	// put the proper functionality in here
	Quaternion rotation = GetSide() * angle;
	Quaternion new_quat = rotation * our_quaternion;
	our_quaternion = (new_quat.normalized());
	set_quaternion((rotation * get_quaternion()).normalized());
}

void QuatCamera::Yaw(float angle)
{
	Quaternion rotation = GetUp() * angle; // not the correct axis
	Quaternion new_quat = rotation * our_quaternion;
	our_quaternion = (new_quat.normalized());
	set_quaternion((rotation * get_quaternion()).normalized()); // we need to keep the internal quaternion separate from our quaternion representation
}

void QuatCamera::Roll(float angle)
{
	Quaternion rotation = GetForward() * angle; // what axis is used for rolling?
	Quaternion new_quat = rotation * our_quaternion;
	our_quaternion = (new_quat.normalized());
	set_quaternion((rotation * get_quaternion()).normalized()); // we need to keep the internal quaternion separate from our quaternion representation
}

void QuatCamera::collide(EntityType other)
{
}

void QuatCamera::addSpeed(float s)
{
	speed += s;
}

/*
 *
 * A camera class which uses an internal quaternion representation to allow students to do motion calculations themselves.
 *
 * Copyright (c) 2024 Samantha Beilman (samanthabeilman@cmail.carleton.ca)
 * Code based on previous versions of COMP 3501 using OpenGL at Carleton.
 *
 */