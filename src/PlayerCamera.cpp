#include "PlayerCamera.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event.hpp>

using namespace godot;

void PlayerCamera::_bind_methods() {}

PlayerCamera::PlayerCamera(float r) : Camera3D()
{
	// Initialize any variables here.
	our_quaternion = Quaternion(Vector3(0, 0, 1), 0.0f);
	radius = r;
	target_ptr = nullptr;
}

void PlayerCamera::_enter_tree()
{
}

void PlayerCamera::_ready()
{
	// this is the Godot-assisted equivalent of SetView
	// Setup the initial unit vectors for each
	// using this setup allows the user to set the position of the camera in the editor.

	// Don't change this
	forward_ = Vector3(get_global_transform().basis[0][2], get_global_transform().basis[1][2], get_global_transform().basis[2][2]);
	side_ = Vector3(get_global_transform().basis[0][0], get_global_transform().basis[1][0], get_global_transform().basis[2][0]);
	// do not save _up as a member -- derive it. You will get a bad grade if you add it as a member variable like these two are.
}

PlayerCamera::~PlayerCamera()
{
	// Add your cleanup here.
}

void PlayerCamera::_process(double delta)
{
	if (Engine::get_singleton()->is_editor_hint())
		return; // Early return if we are in editor

	// if we have a target, point at it
	if (target_ptr == nullptr)
	{
	}
	else if (is_current())
	{
		this->look_at(target_ptr->get_global_position());
	}

	// if (target_ptr != nullptr && is_current())
	// {
	// }
}

Vector3 PlayerCamera::GetForward(void) const
{
	Vector3 current_forward = (get_quaternion().xform(forward_));
	return -current_forward.normalized(); // Return -forward since the camera coordinate system points in the opposite direction
}

Vector3 PlayerCamera::GetSide(void) const
{
	Vector3 current_side = (get_quaternion().xform(side_));
	return current_side;
}

Vector3 PlayerCamera::GetUp(void) const
{
	// how do you get the up vector?
	Vector3 current_up = vec3_cross(GetSide(), GetForward());
	return current_up;
}

Vector3 godot::PlayerCamera::GetMovementPlaneForward()
{
	Vector3 movementPlaneForwardVector = GetForward();
	movementPlaneForwardVector.y = 0; // the player moves on the X,Z plane, thefore we can flatten the forward vector to this plane
	movementPlaneForwardVector.normalize();
	return movementPlaneForwardVector;
}

Vector3 godot::PlayerCamera::GetMovementPlaneSide()
{
	Vector3 movementPlaneSideVector = GetSide();
	movementPlaneSideVector.y = 0; // the player moves on the X,Z plane, thefore we can flatten the forward vector to this plane
	movementPlaneSideVector.normalize();
	return movementPlaneSideVector;
}

void godot::PlayerCamera::SetTarget(Node3D *newTarget_ptr)
{
	this->target_ptr = newTarget_ptr;
}