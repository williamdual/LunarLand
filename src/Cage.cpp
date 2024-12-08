#include "defs.h"
#include "Cage.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void Cage::_bind_methods() {}

Cage::Cage() : Node3D() {}

void Cage::_enter_tree ( ){
    if(DEBUG) UtilityFunctions::print("Enter Tree - Cage.");

    // Creating walls
    create_and_add_as_child<StaticBody3D>(wall_1, "Wall 1", true);
    create_and_add_as_child<StaticBody3D>(wall_2, "Wall 2", true);
    create_and_add_as_child<StaticBody3D>(wall_3, "Wall 3", true);
    create_and_add_as_child<StaticBody3D>(wall_4, "Wall 4", true);
    create_and_add_as_child<StaticBody3D>(wall_5, "Wall 5", true);
    create_and_add_as_child<StaticBody3D>(wall_6, "Wall 6", true);
    create_and_add_as_child<StaticBody3D>(wall_7, "Wall 7", true);

    // Creating the collisions for each wall
    create_and_add_as_child_of_parent<CollisionShape3D>(col_1, "ColWall1", wall_1);
    create_and_add_as_child_of_parent<CollisionShape3D>(col_2, "ColWall2", wall_2);
    create_and_add_as_child_of_parent<CollisionShape3D>(col_3, "ColWall3", wall_3);
    create_and_add_as_child_of_parent<CollisionShape3D>(col_4, "ColWall4", wall_4);
    create_and_add_as_child_of_parent<CollisionShape3D>(col_5, "ColWall5", wall_5);
    create_and_add_as_child_of_parent<CollisionShape3D>(col_6, "ColWall6", wall_6);
    create_and_add_as_child_of_parent<CollisionShape3D>(col_7, "ColWall7", wall_7);

    // Setting up first wall
    BoxShape3D* shape_1 = memnew(BoxShape3D);
    shape_1->set_size(Vector3(50, 50, 1));
    col_1->set_shape(shape_1);
    wall_1->set_global_position(Vector3(27.473, -31.481, 18.22));

    // Setting up second wall
    BoxShape3D* shape_2 = memnew(BoxShape3D);
    shape_2->set_size(Vector3(50, 50, 1));
    col_2->set_shape(shape_2);
    wall_2->set_global_position(Vector3(-27.695, -31.481, 18.22));

    // Setting up third wall
    BoxShape3D* shape_3 = memnew(BoxShape3D);
    shape_3->set_size(Vector3(1, 70, 100));
    col_3->set_shape(shape_3);
    wall_3->set_global_position(Vector3(-52.094, -31.665, 67.451));

    // Setting up fourth wall
    BoxShape3D* shape_4 = memnew(BoxShape3D);
    shape_4->set_size(Vector3(1, 70, 100));
    col_4->set_shape(shape_4);
    wall_4->set_global_position(Vector3(51.963, -31.665, 67.451));

    // Setting up fifth wall
    BoxShape3D* shape_5 = memnew(BoxShape3D);
    shape_5->set_size(Vector3(200, 70, 1));
    col_5->set_shape(shape_5);
    wall_5->set_global_position(Vector3(0.0, -42.258, 116.654));

    // Setting up sixth wall
    BoxShape3D* shape_6 = memnew(BoxShape3D);
    shape_6->set_size(Vector3(1, 50, 30));
    col_6->set_shape(shape_6);
    wall_6->set_global_position(Vector3(3.093, -16.385, 4.675));

    // Setting up seventh wall
    BoxShape3D* shape_7 = memnew(BoxShape3D);
    shape_7->set_size(Vector3(1, 50, 30));
    col_7->set_shape(shape_7);
    wall_7->set_global_position(Vector3(-2.925, -16.385, 4.675));
}

void Cage::_ready ( ){
    if(DEBUG) UtilityFunctions::print("Ready - Cage."); 
}

template <class T>
// returns true if pointer is brand-new; false if retrieved from SceneTree
bool Cage::create_and_add_as_child(T *&pointer, String name, bool search)
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

template <class T>
// returns true if pointer is brand-new; false if retrieved from SceneTree
// variant allows you to create a child of a node pointer other than 'this'
bool Cage::create_and_add_as_child_of_parent(T *&pointer, String name, Node *parent)
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