#ifndef BOID_H
#define BOID_H

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/array.hpp>

using namespace godot;

class Boid : public Sprite2D {
	GDCLASS(Boid, Sprite2D);

private:
	Vector2 velocity;
	Vector2 acceleration;

	// Flocking parameters
	float max_speed;
	float max_force;
	float perception_radius;

	// Behavior weights
	float separation_weight;
	float alignment_weight;
	float cohesion_weight;
	float target_weight;

	Vector2 boundary_min;
	Vector2 boundary_max;

	bool initialized;

protected:
	static void _bind_methods();

public:
	Boid();
	~Boid();

	void _ready() override;
	void _process(double delta) override;

	// Flocking behaviors (Reynolds' Boids rules)
	Vector2 separation(const Array &boids);
	Vector2 alignment(const Array &boids);
	Vector2 cohesion(const Array &boids);
	Vector2 seek_target(Vector2 target);

	// Movement
	void apply_force(Vector2 force);
	void update_movement(double delta);
	void wrap_around_edges();

	// Getters and setters
	void set_max_speed(float p_speed);
	float get_max_speed() const;

	void set_max_force(float p_force);
	float get_max_force() const;

	void set_perception_radius(float p_radius);
	float get_perception_radius() const;

	void set_separation_weight(float p_weight);
	float get_separation_weight() const;

	void set_alignment_weight(float p_weight);
	float get_alignment_weight() const;

	void set_cohesion_weight(float p_weight);
	float get_cohesion_weight() const;

	void set_target_weight(float p_weight);
	float get_target_weight() const;

	void set_boundary_min(Vector2 p_min);
	Vector2 get_boundary_min() const;

	void set_boundary_max(Vector2 p_max);
	Vector2 get_boundary_max() const;

	Vector2 get_velocity() const;
	void set_velocity(Vector2 p_velocity);
};

#endif
