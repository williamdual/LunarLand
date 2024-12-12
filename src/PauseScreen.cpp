#include "defs.h"
#include "PauseScreen.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp> // for the debug statements
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void PauseScreen::_bind_methods() {}

PauseScreen::PauseScreen() : Node3D() {}

void PauseScreen::_enter_tree ( ){
    if(DEBUG) UtilityFunctions::print("Enter Tree - PauseScreen.");
}

void PauseScreen::_ready ( ){
    if(DEBUG) UtilityFunctions::print("Ready - PauseScreen.");
}

// Member function for setting pause screen
void PauseScreen::Pause() {
    Size2 size = get_viewport()->get_visible_rect().size;
    background->set_global_position(Vector2(-33, -21));
    margin_center->set_global_position(Vector2(-100.0, -300.0));
}

// Member function for unsetting pause screen
void PauseScreen::Resume() {
    background->set_global_position(Vector2(-10000, -10000));
    margin_center->set_global_position(Vector2(-10000, -10000));
}

// Member function that sets up title screen attributes
void PauseScreen::SetupScreen() {

    // Setting up canvas layer for rest of components
    create_and_add_as_child<CanvasLayer>(canvas_layer, "CanvasLayer", true);
    canvas_layer->set_follow_viewport(true);

    // Setting up background
    create_and_add_as_child_of_parent<ColorRect>(background, "Background", canvas_layer);
    background->set_color(Color(0.2275, 0.2980, 0.4784, 1.0));
    background->set_size(Vector2(24, 27));
    background->set_scale(Vector2(200, 150));
    background->set_global_position(Vector2(-10000, -10000));
    // Transform position: Vector2(-33, -21)

    // Center box for margin container
    create_and_add_as_child_of_parent<CenterContainer>(margin_center, "CenterMargin", canvas_layer);
    margin_center->set_anchors_preset(Control::PRESET_FULL_RECT);
    margin_center->set_global_position(Vector2(-10000, -10000));

    // Setting up the margin container
    create_and_add_as_child_of_parent<MarginContainer>(margin_container, "MarginContainer", margin_center);
    //margin_container->set_anchors_preset(Control::PRESET_FULL_RECT);
    //margin_container->set_size(Vector2(1152, 667));
    //margin_container->set_global_position(Vector2(-10000, -10000));
    // margin_container->add_theme_constant_override("margin_left", 120);
    // margin_container->add_theme_constant_override("margin_top", 80);
    // margin_container->add_theme_constant_override("margin_right", 120);
    // margin_container->add_theme_constant_override("margin_bottom", 80);
    // Transform position: Vector2(0, -59.5)

    // Setting up the box container
    create_and_add_as_child_of_parent<VBoxContainer>(main_container, "MainContainer", margin_container);
    main_container->add_theme_constant_override("separation", 50);

    // Setting up the containers for the text
    create_and_add_as_child_of_parent<CenterContainer>(title_container, "TitleContainer", main_container);
    create_and_add_as_child_of_parent<CenterContainer>(pause_container, "PauseContainer", main_container);
    create_and_add_as_child_of_parent<CenterContainer>(map_container, "MapContainer", main_container);
    create_and_add_as_child_of_parent<CenterContainer>(quote_container, "QuoteContainer", main_container);

    // Grabbing font for future use
    Ref<FontFile> font = ResourceLoader::get_singleton()->load("Fonts/ThaleahFat.ttf", "FontFile");

    // Setting up the title text
    create_and_add_as_child_of_parent<Label>(title_text, "TitleText", title_container);
    LabelSettings* title_style = memnew(LabelSettings);
    title_style->set_font(font);
    title_style->set_font_size(100);
    title_style->set_font_color(Color(0.2980, 0.7882, 0.9412, 1.0));
    title_style->set_outline_size(15);
    title_style->set_outline_color(Color(1.0, 1.0, 1.0, 1.0));
    title_text->set_label_settings(title_style);
    title_text->set_text("Game Paused");
    title_text->set_horizontal_alignment(HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);

    // Setting up the pause text
    create_and_add_as_child_of_parent<Label>(pause_text, "PauseText", pause_container);
    LabelSettings* pause_style = memnew(LabelSettings);
    pause_style->set_font(font);
    pause_style->set_font_size(50);
    pause_style->set_font_color(Color(1.0, 1.0, 1.0, 1.0));
    pause_text->set_label_settings(pause_style);
    pause_text->set_text("Press 'Esc' again to unpause game");
    pause_text->set_horizontal_alignment(HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);

    // Setting up the map
    create_and_add_as_child_of_parent<TextureRect>(map, "Map", map_container);
    Ref<CompressedTexture2D> map_image = ResourceLoader::get_singleton()->load("InventorySprites/map.png", "CompressedTexture2D");
    map->set_texture(map_image);

    // Setting up quote text
    create_and_add_as_child_of_parent<Label>(quote_text, "QuoteText", quote_container);
    LabelSettings* quote_style = memnew(LabelSettings);
    quote_style->set_font(font);
    quote_style->set_font_size(50);
    quote_style->set_font_color(Color(1.0, 1.0, 1.0, 1.0));
    quote_text->set_label_settings(quote_style);
    quote_text->set_text("\"We strive for immersive space themed experiences,\naccidental deaths and malfunctions are all part of that\"\n-Rob Yublind, former PR representative, serving life imprisonment");
    quote_text->set_horizontal_alignment(HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);
}

template <class T>
// returns true if pointer is brand-new; false if retrieved from SceneTree
bool PauseScreen::create_and_add_as_child(T *&pointer, String name, bool search)
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
bool PauseScreen::create_and_add_as_child_of_parent(T *&pointer, String name, Node *parent)
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