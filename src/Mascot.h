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
#include "particle_system_3501.h"

#define DEBUG true


// everything in gdextension is defined in this namespace
namespace godot {
class Mascot : public Node3D {
    // this macro sets up a few internal things
	GDCLASS(Mascot, Node3D);

private:

protected:
    // a static function that Godot will call to find out which methods can be called and which properties it exposes
	static void _bind_methods();


public:
	Mascot();
	~Mascot();

	void _enter_tree ( ) override;
	void _ready ( ) override;
	void _process (double delta);

	// the return type represents whether it existed already; true if it is brand-new; false if it was retrieved from the SceneTree
	// search defines whether the scenetree should be checked for an instance
	template <class T>
	bool create_and_add_as_child(T* &pointer, String name, bool search = false);

	template <class T>
	bool create_and_add_as_child_of_node(T* &pointer, String name, Node* parent, bool search = true);

};

}

#endif // JACK_H