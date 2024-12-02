#ifndef PLAYERCAMERA_H
#define PLAYERCAMERA_H

// parent class
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

#include <godot_cpp/classes/input.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/variant/quaternion.hpp>

#include "defs.h"
// everything in gdextension is defined in this namespace
namespace godot
{
	class PlayerCamera : public Camera3D
	{
		// this macro sets up a few internal things
		GDCLASS(PlayerCamera, Camera3D);

	private:
		// this one is the one we will use to transform our side, up and forward vectors for the GetX functions.
		// why would it be a bad idea to transform by the built-in quaternion?
		Quaternion our_quaternion;
		Transform3D aim;
		Vector3 forward_;
		Vector3 local_forward;
		Vector3 side_;
		Node3D *target_ptr;
		// CameraMoveType move_type;
		CameraTrackType track_type;
		float moveSpeed; // Ideally matches player move speed (or is kinda close)
		float cam_err;

	protected:
		// a static function that Godot will call to find out which methods can be called and which properties it exposes
		static void _bind_methods();

	public:
		PlayerCamera();
		~PlayerCamera();
		void _enter_tree() override;
		void _ready() override;
		void _process(double delta) override;
		Vector3 GetForward();
		Vector3 GetUp();
		Vector3 GetSide();
		Vector3 GetMovementPlaneForward();
		Vector3 GetMovementPlaneSide();
		void SetTarget(Node3D *newTarget_ptr);
		// void SetType(CameraMoveType, CameraTrackType);
		// CameraMoveType GetMoveType();
		void SetTrackType(CameraTrackType);
		CameraTrackType GetTrackType();
		void SafelyRotate(Vector3 axis, float angle);
	};
}

#endif