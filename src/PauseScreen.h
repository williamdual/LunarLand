#ifndef PAUSE_SCREEN_H
#define PAUSE_SCREEN_H

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

#include <godot_cpp/variant/rect2.hpp>		 // for viewport size
#include <godot_cpp/classes/canvas_item.hpp> // for viewport size
#include <godot_cpp/classes/viewport.hpp> // for viewport size
#include <godot_cpp/classes/canvas_layer.hpp>

#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/center_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/label_settings.hpp>
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/font_file.hpp>
#include <godot_cpp/classes/compressed_texture2d.hpp>

#include <godot_cpp/variant/vector3.hpp>

// everything in gdextension is defined in this namespace
namespace godot {

class PauseScreen : public Node3D {
    GDCLASS(PauseScreen, Node3D);

private:

    // Visual members of the pause screen
    CanvasLayer* canvas_layer;
    ColorRect* background;
    CenterContainer* margin_center;
    MarginContainer* margin_container;
    VBoxContainer* main_container;
    CenterContainer* title_container;
    CenterContainer* pause_container;
    CenterContainer* map_container;
    CenterContainer* quote_container;
    Label* title_text;
    Label* pause_text;
    Label* quote_text;
    TextureRect* map;

protected:
    static void _bind_methods();

public:
    PauseScreen();

    void _enter_tree ( ) override;
    void _ready ( ) override;

    // Member function that sets up items for the screen
    void SetupScreen(void);

    // Member functions to set and unset the pause screen
    void Pause(void);
    void Resume(void);

    // the return type represents whether it existed already; true if it is brand-new; false if it was retrieved from the SceneTree
    // search defines whether the scenetree should be checked for an instance
    template <class T>
    bool create_and_add_as_child(T *&pointer, String name, bool search = false);

    // this variant you have probably seen before; it allows you to create a CustomScene3501 will a bit more complex of a hierarchy
    // we will assume that the node should always be searched for in this variant
    template <class T>
    bool create_and_add_as_child_of_parent(T *&pointer, String name, Node *parent);
};

}

#endif