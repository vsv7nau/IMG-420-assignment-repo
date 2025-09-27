#include "enhanced_sprite.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void EnhancedSprite::_bind_methods() {
	// bind methods so Godot can poke our caffeinated pixels
	// Bounce
	ClassDB::bind_method(D_METHOD("set_bounce_speed", "speed"), &EnhancedSprite::set_bounce_speed);
	ClassDB::bind_method(D_METHOD("get_bounce_speed"), &EnhancedSprite::get_bounce_speed);
	ClassDB::add_property("EnhancedSprite", PropertyInfo(Variant::FLOAT, "bounce_speed", PROPERTY_HINT_RANGE, "0,1000,10"), "set_bounce_speed", "get_bounce_speed");

	ClassDB::bind_method(D_METHOD("set_bounce_area_size", "size"), &EnhancedSprite::set_bounce_area_size);
	ClassDB::bind_method(D_METHOD("get_bounce_area_size"), &EnhancedSprite::get_bounce_area_size);
	ClassDB::add_property("EnhancedSprite", PropertyInfo(Variant::VECTOR2, "bounce_area_size"), "set_bounce_area_size", "get_bounce_area_size");

	ClassDB::bind_method(D_METHOD("set_bounce_enabled", "enabled"), &EnhancedSprite::set_bounce_enabled);
	ClassDB::bind_method(D_METHOD("get_bounce_enabled"), &EnhancedSprite::get_bounce_enabled);
	ClassDB::add_property("EnhancedSprite", PropertyInfo(Variant::BOOL, "bounce_enabled"), "set_bounce_enabled", "get_bounce_enabled");

	//color cycle
	ClassDB::bind_method(D_METHOD("set_cycle_time", "time"), &EnhancedSprite::set_cycle_time);
	ClassDB::bind_method(D_METHOD("get_cycle_time"), &EnhancedSprite::get_cycle_time);
	ClassDB::add_property("EnhancedSprite", PropertyInfo(Variant::FLOAT, "cycle_time", PROPERTY_HINT_RANGE, "0.1,10.0,0.1"), "set_cycle_time", "get_cycle_time");

	ClassDB::bind_method(D_METHOD("set_color_cycle_colors", "colors"), &EnhancedSprite::set_color_cycle_colors);
	ClassDB::bind_method(D_METHOD("get_color_cycle_colors"), &EnhancedSprite::get_color_cycle_colors);
	ClassDB::add_property("EnhancedSprite", PropertyInfo(Variant::ARRAY, "color_cycle_colors"), "set_color_cycle_colors", "get_color_cycle_colors");

	ClassDB::bind_method(D_METHOD("set_color_cycling_enabled", "enabled"), &EnhancedSprite::set_color_cycling_enabled);
	ClassDB::bind_method(D_METHOD("get_color_cycling_enabled"), &EnhancedSprite::get_color_cycling_enabled);
	ClassDB::add_property("EnhancedSprite", PropertyInfo(Variant::BOOL, "color_cycling_enabled"), "set_color_cycling_enabled", "get_color_cycling_enabled");

	ClassDB::bind_method(D_METHOD("start_effects"), &EnhancedSprite::start_effects);
	ClassDB::bind_method(D_METHOD("stop_effects"), &EnhancedSprite::stop_effects);
	ClassDB::bind_method(D_METHOD("reset_position"), &EnhancedSprite::reset_position);

	// Bind signals
	ADD_SIGNAL(MethodInfo("boundary_hit", PropertyInfo(Variant::STRING, "side")));
	ADD_SIGNAL(MethodInfo("effects_started"));
	ADD_SIGNAL(MethodInfo("effects_stopped"));
}

EnhancedSprite::EnhancedSprite() {
	// default values
	bounce_speed = 200.0f;
	bounce_direction = Vector2(1.0f, 1.0f).normalized();
	bounce_area_size = Vector2(800.0f, 600.0f);
	bounce_enabled = true;
	
	cycle_time = 2.0f;
	color_cycling_enabled = true;
	current_cycle_time = 0.0f;
	current_color_index = 0;
	
	original_position = Vector2(0, 0);
	initialized = false;

	// color cycle values
	color_cycle_colors = TypedArray<Color>();
	color_cycle_colors.push_back(Color(1.0f, 0.0f, 0.0f, 1.0f));
	color_cycle_colors.push_back(Color(0.0f, 1.0f, 0.0f, 1.0f));
	color_cycle_colors.push_back(Color(0.0f, 0.0f, 1.0f, 1.0f)); 
	color_cycle_colors.push_back(Color(1.0f, 1.0f, 0.0f, 1.0f)); 
}

EnhancedSprite::~EnhancedSprite() {
}

void EnhancedSprite::_ready() {
	original_position = get_position();
	initialized = true;
	
	if (color_cycle_colors.size() > 0) {
		set_modulate(color_cycle_colors[0]);
	}
    
	// shout into the debug void that we're all set to go
	UtilityFunctions::print("EnhancedSprite is ready to go! Position: ", get_position());
}

void EnhancedSprite::_process(double delta) {
	if (!initialized) return;

	if (bounce_enabled) {
		update_bounce_movement(delta);
	}

	if (color_cycling_enabled && color_cycle_colors.size() > 1) {
		update_color_cycling(delta);
	}
}

// The magical bounce function ( handles the actual boundary collision and "physics" )
void EnhancedSprite::update_bounce_movement(double delta) {
	Vector2 current_pos = get_position();
	Vector2 new_pos = current_pos + (bounce_direction * bounce_speed * static_cast<float>(delta));
	
	bool hit_boundary = false;
	String boundary_side = "";
	
	if (new_pos.x <= 0 || new_pos.x >= bounce_area_size.x) {
		bounce_direction.x = -bounce_direction.x;
		new_pos.x = Math::clamp(new_pos.x, 0.0f, bounce_area_size.x);
		hit_boundary = true;
		boundary_side = (new_pos.x <= 0) ? "left" : "right";
	}
	
	if (new_pos.y <= 0 || new_pos.y >= bounce_area_size.y) {
		bounce_direction.y = -bounce_direction.y;
		new_pos.y = Math::clamp(new_pos.y, 0.0f, bounce_area_size.y);
		hit_boundary = true;
		if (boundary_side.is_empty()) {
			boundary_side = (new_pos.y <= 0) ? "top" : "bottom";
		} else {
			boundary_side += "_" + String((new_pos.y <= 0) ? "top" : "bottom");
		}
	}
	
	set_position(new_pos);
	
	if (hit_boundary) {
		emit_boundary_signal(boundary_side);
	}
}

// The magical color cycle function ( changes the modulation based on the cycle time thats selected )
void EnhancedSprite::update_color_cycling(double delta) {
	current_cycle_time += static_cast<float>(delta);
	
	if (current_cycle_time >= cycle_time) {
		current_cycle_time = 0.0f;
		current_color_index = (current_color_index + 1) % color_cycle_colors.size();
		set_modulate(color_cycle_colors[current_color_index]);
	}
}

void EnhancedSprite::emit_boundary_signal(const String &side) {
	emit_signal("boundary_hit", side);
	UtilityFunctions::print("Boundary Bonk: ", side);
}

void EnhancedSprite::set_bounce_speed(float p_speed) {
	bounce_speed = Math::max(0.0f, p_speed);
}

float EnhancedSprite::get_bounce_speed() const {
	return bounce_speed;
}

void EnhancedSprite::set_bounce_area_size(Vector2 p_size) {
	bounce_area_size = Vector2(Math::max(100.0f, p_size.x), Math::max(100.0f, p_size.y));
}

Vector2 EnhancedSprite::get_bounce_area_size() const {
	return bounce_area_size;
}

void EnhancedSprite::set_bounce_enabled(bool p_enabled) {
	bounce_enabled = p_enabled;
}

bool EnhancedSprite::get_bounce_enabled() const {
	return bounce_enabled;
}

void EnhancedSprite::set_cycle_time(float p_time) {
	cycle_time = Math::max(0.1f, p_time);
}

float EnhancedSprite::get_cycle_time() const {
	return cycle_time;
}

void EnhancedSprite::set_color_cycle_colors(const TypedArray<Color> &p_colors) {
	color_cycle_colors = p_colors;
	if (color_cycle_colors.size() > 0 && initialized) {
		current_color_index = 0;
		set_modulate(color_cycle_colors[0]);
	}
}

TypedArray<Color> EnhancedSprite::get_color_cycle_colors() const {
	return color_cycle_colors;
}

void EnhancedSprite::set_color_cycling_enabled(bool p_enabled) {
	color_cycling_enabled = p_enabled;
}

bool EnhancedSprite::get_color_cycling_enabled() const {
	return color_cycling_enabled;
}

void EnhancedSprite::start_effects() {
	bounce_enabled = true;
	color_cycling_enabled = true;
	emit_signal("effects_started");
	UtilityFunctions::print("Effects started :D");
}

void EnhancedSprite::stop_effects() {
	bounce_enabled = false;
	color_cycling_enabled = false;
	emit_signal("effects_stopped");
	UtilityFunctions::print("Effects stopped :C");
}

void EnhancedSprite::reset_position() {
	if (initialized) {
		set_position(original_position);
		current_cycle_time = 0.0f;
		current_color_index = 0;
		if (color_cycle_colors.size() > 0) {
			set_modulate(color_cycle_colors[0]);
		}
		UtilityFunctions::print("Position reset to: ", original_position);
	}
}
