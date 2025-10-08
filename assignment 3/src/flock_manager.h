#ifndef FLOCK_MANAGER_H
#define FLOCK_MANAGER_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include "boid.h"

using namespace godot;

class FlockManager : public Node2D {
	GDCLASS(FlockManager, Node2D);

private:
	Array boids;
	Vector2 target_position;
	int boid_count;

	// Spawn settings
	Vector2 spawn_area_min;
	Vector2 spawn_area_max;

	// Boid template settings
	float boid_max_speed;
	float boid_max_force;
	float boid_perception_radius;
	float boid_separation_weight;
	float boid_alignment_weight;
	float boid_cohesion_weight;
	float boid_target_weight;

	bool initialized;
	bool follow_target_enabled;

protected:
	static void _bind_methods();

public:
	FlockManager();
	~FlockManager();

	void _ready() override;
	void _process(double delta) override;

	// Flock management
	void spawn_boids();
	void clear_boids();
	void update_flock(double delta);

	// Getters and setters
	void set_boid_count(int p_count);
	int get_boid_count() const;

	void set_target_position(Vector2 p_position);
	Vector2 get_target_position() const;

	void set_follow_target_enabled(bool p_enabled);
	bool get_follow_target_enabled() const;

	void set_spawn_area_min(Vector2 p_min);
	Vector2 get_spawn_area_min() const;

	void set_spawn_area_max(Vector2 p_max);
	Vector2 get_spawn_area_max() const;

	// Boid template settings
	void set_boid_max_speed(float p_speed);
	float get_boid_max_speed() const;

	void set_boid_max_force(float p_force);
	float get_boid_max_force() const;

	void set_boid_perception_radius(float p_radius);
	float get_boid_perception_radius() const;

	void set_boid_separation_weight(float p_weight);
	float get_boid_separation_weight() const;

	void set_boid_alignment_weight(float p_weight);
	float get_boid_alignment_weight() const;

	void set_boid_cohesion_weight(float p_weight);
	float get_boid_cohesion_weight() const;

	void set_boid_target_weight(float p_weight);
	float get_boid_target_weight() const;

	Array get_boids() const;
};

#endif
