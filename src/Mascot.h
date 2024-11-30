#ifndef MASCOT_H
#define MASCOT_H

// parent class
#include <godot_cpp/classes/node3d.hpp>

#include <godot_cpp/classes/input.hpp>

#include <godot_cpp/templates/vector.hpp> // GODOT EQUIVALENT TO std::vector !!!!

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/quaternion.hpp>
#include <godot_cpp/variant/transform3d.hpp>

#include "MascotPart.h"
#include "MascotBody.h"
#include "MascotArm.h"
#include "MascotHead.h"
#include "particle_system_3501.h"

#define DEBUG true


// everything in gdextension is defined in this namespace
namespace godot {
class Mascot : public Node3D {
    // this macro sets up a few internal things
	GDCLASS(Mascot, Node3D);

private:

    // Body parts of the mascot
    MascotBody* body;
    MascotHead* head;
    MascotArm* upper_upper_left;
    MascotArm* upper_lower_left;
    MascotArm* upper_upper_right;
    MascotArm* upper_lower_right;
    MascotArm* lower_upper_left;
    MascotArm* lower_lower_left;
    MascotArm* lower_upper_right;
    MascotArm* lower_lower_right;

protected:
    // a static function that Godot will call to find out which methods can be called and which properties it exposes
	static void _bind_methods();


public:
	Mascot();
	~Mascot();

	void _enter_tree ( ) override;
	void _ready ( ) override;
	void _process (double delta);

    // Member function that adds a light to the environment object
    void AddLight(Vector3 light_pos, Vector3 light_col, int spec_power);

    // Member function that updates the view position for the environment object
    void SetCameraPosition(Vector3 camera_pos);

    // Member function that registers camera triggers for signal purposes
    void RegisterCameraTrigs(Vector<CameraTrigger*> cam_trigs);

    // Setting the mascot's position
    void SetPosition(Vector3 pos);

    // Setting the mascot's rotation
    void SetRotation(Vector3 rot);

	// the return type represents whether it existed already; true if it is brand-new; false if it was retrieved from the SceneTree
	// search defines whether the scenetree should be checked for an instance
	template <class T>
	bool create_and_add_as_child(T* &pointer, String name, bool search = false);

	template <class T>
	bool create_and_add_as_child_of_node(T* &pointer, String name, Node* parent, bool search = true);

};

}

#endif // JACK_H