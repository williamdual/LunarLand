#include "PlayerCamera.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event.hpp>

using namespace godot;

void PlayerCamera::_bind_methods() {}

PlayerCamera::PlayerCamera() : Camera3D()
{
	// Initialize any variables here.
	our_quaternion = Quaternion(Vector3(0, 0, 1), 0.0f);
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
	moveSpeed = 10.0f;
	cam_err = 0.5f;
}

PlayerCamera::~PlayerCamera()
{
	// Add your cleanup here.
}

void PlayerCamera::_process(double delta)
{
	if (Engine::get_singleton()->is_editor_hint())
		return; // Early return if we are in editor

	if (target_ptr == nullptr)
	{
		// if we dont have a target, dont do anything
	}
	else if (is_current() && track_type == CameraTrackType::tracking)
	{
		// turn to face the player

		// step 1) generate the yaw rotation, from the camera to the player
		Vector3 flatTarget = (target_ptr->get_global_position() - this->get_global_position()).normalized();
		flatTarget.y = 0;
		Vector3 flatForward = GetForward();
		flatForward.y = 0;

		// step 2) make the shortest rotational arc from the flatforward vector to the flatTarget vector
		Quaternion desiredWorldYawRotation = Quaternion(flatForward, flatTarget).normalized();

		// step 3) update "our_quaternion" so that forward_ and side_ work properly
		our_quaternion = (desiredWorldYawRotation * our_quaternion).normalized();

		// step 4) apply the yaw rotation
		set_quaternion((desiredWorldYawRotation * get_quaternion()).normalized());

		// step 5) pivot the camera down so that it looks directly at the player
		// Note: this is very very jank, but this angles the camera down towards the player without messing with anything else.
		this->look_at(target_ptr->get_global_position());
	}
	else if (is_current() && track_type == CameraTrackType::panning)
	{
		Vector3 forward = GetMovementPlaneForward();
		Vector3 move_right = GetMovementPlaneSide();
		Vector3 move_left = GetMovementPlaneSide() * -1.0f;
		Vector3 to_player = (target_ptr->get_global_position() - this->get_global_position()).normalized();
		if (forward.distance_to(to_player) < cam_err) // if player is very close to the center of our screen
		{
			// DO NOTHING, WE ALL GOOD
		}
		else if (move_right.distance_to(to_player) < move_left.distance_to(to_player)) // if player is on our right hand side
		{
			this->set_global_position(this->get_global_position() + move_right * moveSpeed * delta);
		}
		else // if player is on our left hand side
		{
			this->set_global_position(this->get_global_position() + move_left * moveSpeed * delta);
		}
	}
	// if its static then do nothing
}

Vector3 PlayerCamera::GetForward(void)
{
	Vector3 current_forward = (our_quaternion.xform(forward_));
	return -current_forward.normalized(); // Return -forward since the camera coordinate system points in the opposite direction

	/*
	Vector3 current_forward = Vector3(0, 0, 0);
	if (track_type == CameraTrackType::tracking)
		current_forward = (get_quaternion().xform(forward_));
	else
		current_forward = (get_quaternion().xform(forward_.rotated(Vector3(0, 1, 0), get_rotation().y)));

	return current_forward.normalized(); // Return -forward since the camera coordinate system points in the opposite direction
	*/
}

Vector3 PlayerCamera::GetSide(void)
{
	Vector3 current_side = (our_quaternion.xform(side_));
	return current_side.normalized();

	/*
	Vector3 current_side = Vector3(0, 0, 0);
	if (track_type == CameraTrackType::tracking)
		current_side = (get_quaternion().xform(side_));
	else
		current_side = (get_quaternion().xform(side_.rotated(Vector3(0, 1, 0), get_rotation().y)));
	return current_side;
	*/
}

Vector3 PlayerCamera::GetUp(void)
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

void godot::PlayerCamera::SetTrackType(CameraTrackType tt)
{
	track_type = tt;
}

CameraTrackType godot::PlayerCamera::GetTrackType()
{
	return track_type;
}

void godot::PlayerCamera::SafelyRotate(Vector3 worldAxis, float angleDegrees)
{
	Quaternion desiredWorldRotation = Quaternion(worldAxis, Math::deg_to_rad(angleDegrees));
	set_quaternion((desiredWorldRotation * get_quaternion()).normalized());
	// set_quaternion(get_quaternion().normalized());
}

void godot::PlayerCamera::SafelyRotate(Vector3 rotation)
{
	if (rotation.x != 0.0)
		SafelyRotate(Vector3(1, 0, 0), rotation.x);
	if (rotation.y != 0.0)
		SafelyRotate(Vector3(0, 1, 0), rotation.y);
}

// void godot::PlayerCamera::SetType(CameraMoveType mt, CameraTrackType tt)
// {
// 	move_type = mt;
// 	track_type = tt;
// }
// CameraMoveType godot::PlayerCamera::GetMoveType()
// {
// 	return move_type;
// }