#ifndef ENHANCED_SPRITE_H
#define ENHANCED_SPRITE_H

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/color.hpp>

using namespace godot;

class EnhancedSprite : public Sprite2D {
	GDCLASS(EnhancedSprite, Sprite2D);

private:
	// how fast we bonkin around
	float bounce_speed;
	Vector2 bounce_direction;
	Vector2 bounce_area_size;

	// are the bonks allowed?
	bool bounce_enabled;
	
	float cycle_time;
	PackedFloat32Array color_cycle_times;
	TypedArray<Color> color_cycle_colors;
	
	bool color_cycling_enabled;
	float current_cycle_time;
	int current_color_index;

	Vector2 original_position;
	bool initialized;

protected:
	static void _bind_methods();

public:
	EnhancedSprite();
	~EnhancedSprite();

	void _ready() override;
	void _process(double delta) override;

	void set_bounce_speed(float p_speed);
	float get_bounce_speed() const;
	
	void set_bounce_area_size(Vector2 p_size);
	Vector2 get_bounce_area_size() const;
	
	void set_bounce_enabled(bool p_enabled);
	bool get_bounce_enabled() const;

	void set_cycle_time(float p_time);
	float get_cycle_time() const;
	
	void set_color_cycle_colors(const TypedArray<Color> &p_colors);
	TypedArray<Color> get_color_cycle_colors() const;
	
	void set_color_cycling_enabled(bool p_enabled);
	bool get_color_cycling_enabled() const;

	void start_effects();
	void stop_effects();
	void reset_position();

	void _on_boundary_hit(const String &boundary_side);

private:
	void update_bounce_movement(double delta);
	void update_color_cycling(double delta);
	void emit_boundary_signal(const String &side);
	void initialize_defaults();
};

#endif
