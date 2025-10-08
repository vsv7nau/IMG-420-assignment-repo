#include "boid.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void Boid::_bind_methods() {
	// Bind movement properties
	ClassDB::bind_method(D_METHOD("set_max_speed", "speed"), &Boid::set_max_speed);
	ClassDB::bind_method(D_METHOD("get_max_speed"), &Boid::get_max_speed);
	ClassDB::add_property("Boid", PropertyInfo(Variant::FLOAT, "max_speed", PROPERTY_HINT_RANGE, "0,500,10"), "set_max_speed", "get_max_speed");

	ClassDB::bind_method(D_METHOD("set_max_force", "force"), &Boid::set_max_force);
	ClassDB::bind_method(D_METHOD("get_max_force"), &Boid::get_max_force);
	ClassDB::add_property("Boid", PropertyInfo(Variant::FLOAT, "max_force", PROPERTY_HINT_RANGE, "0,10,0.1"), "set_max_force", "get_max_force");

	ClassDB::bind_method(D_METHOD("set_perception_radius", "radius"), &Boid::set_perception_radius);
	ClassDB::bind_method(D_METHOD("get_perception_radius"), &Boid::get_perception_radius);
	ClassDB::add_property("Boid", PropertyInfo(Variant::FLOAT, "perception_radius", PROPERTY_HINT_RANGE, "0,200,10"), "set_perception_radius", "get_perception_radius");

	// Bind behavior weights
	ClassDB::bind_method(D_METHOD("set_separation_weight", "weight"), &Boid::set_separation_weight);
	ClassDB::bind_method(D_METHOD("get_separation_weight"), &Boid::get_separation_weight);
	ClassDB::add_property("Boid", PropertyInfo(Variant::FLOAT, "separation_weight", PROPERTY_HINT_RANGE, "0,5,0.1"), "set_separation_weight", "get_separation_weight");

	ClassDB::bind_method(D_METHOD("set_alignment_weight", "weight"), &Boid::set_alignment_weight);
	ClassDB::bind_method(D_METHOD("get_alignment_weight"), &Boid::get_alignment_weight);
	ClassDB::add_property("Boid", PropertyInfo(Variant::FLOAT, "alignment_weight", PROPERTY_HINT_RANGE, "0,5,0.1"), "set_alignment_weight", "get_alignment_weight");

	ClassDB::bind_method(D_METHOD("set_cohesion_weight", "weight"), &Boid::set_cohesion_weight);
	ClassDB::bind_method(D_METHOD("get_cohesion_weight"), &Boid::get_cohesion_weight);
	ClassDB::add_property("Boid", PropertyInfo(Variant::FLOAT, "cohesion_weight", PROPERTY_HINT_RANGE, "0,5,0.1"), "set_cohesion_weight", "get_cohesion_weight");

	ClassDB::bind_method(D_METHOD("set_target_weight", "weight"), &Boid::set_target_weight);
	ClassDB::bind_method(D_METHOD("get_target_weight"), &Boid::get_target_weight);
	ClassDB::add_property("Boid", PropertyInfo(Variant::FLOAT, "target_weight", PROPERTY_HINT_RANGE, "0,5,0.1"), "set_target_weight", "get_target_weight");

	// Bind boundary
	ClassDB::bind_method(D_METHOD("set_boundary_min", "min"), &Boid::set_boundary_min);
	ClassDB::bind_method(D_METHOD("get_boundary_min"), &Boid::get_boundary_min);
	ClassDB::add_property("Boid", PropertyInfo(Variant::VECTOR2, "boundary_min"), "set_boundary_min", "get_boundary_min");

	ClassDB::bind_method(D_METHOD("set_boundary_max", "max"), &Boid::set_boundary_max);
	ClassDB::bind_method(D_METHOD("get_boundary_max"), &Boid::get_boundary_max);
	ClassDB::add_property("Boid", PropertyInfo(Variant::VECTOR2, "boundary_max"), "set_boundary_max", "get_boundary_max");

	// Bind methods
	ClassDB::bind_method(D_METHOD("apply_force", "force"), &Boid::apply_force);
	ClassDB::bind_method(D_METHOD("get_velocity"), &Boid::get_velocity);
	ClassDB::bind_method(D_METHOD("set_velocity", "velocity"), &Boid::set_velocity);
}

Boid::Boid() {
	// Default flocking parameters based on Reynolds' original values
	max_speed = 150.0f;
	max_force = 3.0f;
	perception_radius = 50.0f;

	// Behavior weights
	separation_weight = 1.5f;
	alignment_weight = 1.0f;
	cohesion_weight = 1.0f;
	target_weight = 0.5f;

	// Initialize velocity with random direction
	float angle = UtilityFunctions::randf() * Math_TAU;
	velocity = Vector2(Math::cos(angle), Math::sin(angle)) * max_speed * 0.5f;
	acceleration = Vector2(0, 0);

	boundary_min = Vector2(0, 0);
	boundary_max = Vector2(800, 600);

	initialized = false;
}

Boid::~Boid() {
}

void Boid::_ready() {
	initialized = true;
}

void Boid::_process(double delta) {
	if (!initialized) return;
	update_movement(delta);
}

Vector2 Boid::separation(const Array &boids) {
	Vector2 steering = Vector2(0, 0);
	int total = 0;

	for (int i = 0; i < boids.size(); i++) {
		Object* obj = boids[i];
		Boid* other = Object::cast_to<Boid>(obj);

		if (other != nullptr && other != this) {
			Vector2 other_pos = other->get_position();
			float distance = get_position().distance_to(other_pos);

			if (distance > 0 && distance < perception_radius) {
				Vector2 diff = get_position() - other_pos;
				diff = diff / (distance * distance); // Weight by distance
				steering += diff;
				total++;
			}
		}
	}

	if (total > 0) {
		steering = steering / total;
		steering = steering.normalized() * max_speed;
		steering = steering - velocity;

		// Limit steering force
		if (steering.length() > max_force) {
			steering = steering.normalized() * max_force;
		}
	}

	return steering;
}

Vector2 Boid::alignment(const Array &boids) {
	Vector2 steering = Vector2(0, 0);
	int total = 0;

	for (int i = 0; i < boids.size(); i++) {
		Object* obj = boids[i];
		Boid* other = Object::cast_to<Boid>(obj);

		if (other != nullptr && other != this) {
			float distance = get_position().distance_to(other->get_position());

			if (distance > 0 && distance < perception_radius) {
				steering += other->get_velocity();
				total++;
			}
		}
	}

	if (total > 0) {
		steering = steering / total;
		steering = steering.normalized() * max_speed;
		steering = steering - velocity;

		// Limit steering force
		if (steering.length() > max_force) {
			steering = steering.normalized() * max_force;
		}
	}

	return steering;
}

Vector2 Boid::cohesion(const Array &boids) {
	Vector2 steering = Vector2(0, 0);
	int total = 0;

	for (int i = 0; i < boids.size(); i++) {
		Object* obj = boids[i];
		Boid* other = Object::cast_to<Boid>(obj);

		if (other != nullptr && other != this) {
			float distance = get_position().distance_to(other->get_position());

			if (distance > 0 && distance < perception_radius) {
				steering += other->get_position();
				total++;
			}
		}
	}

	if (total > 0) {
		steering = steering / total;
		steering = steering - get_position();
		steering = steering.normalized() * max_speed;
		steering = steering - velocity;

		// Limit steering force
		if (steering.length() > max_force) {
			steering = steering.normalized() * max_force;
		}
	}

	return steering;
}

Vector2 Boid::seek_target(Vector2 target) {
	Vector2 desired = target - get_position();
	desired = desired.normalized() * max_speed;

	Vector2 steering = desired - velocity;

	// Limit steering force
	if (steering.length() > max_force) {
		steering = steering.normalized() * max_force;
	}

	return steering;
}

void Boid::apply_force(Vector2 force) {
	acceleration += force;
}

void Boid::update_movement(double delta) {
	// Update velocity
	velocity += acceleration * static_cast<float>(delta);

	// Limit speed
	if (velocity.length() > max_speed) {
		velocity = velocity.normalized() * max_speed;
	}

	// Update position
	set_position(get_position() + velocity * static_cast<float>(delta));

	// Reset acceleration
	acceleration = Vector2(0, 0);

	// Wrap around edges
	wrap_around_edges();

	// Rotate sprite to face direction of movement
	if (velocity.length() > 0) {
		set_rotation(velocity.angle());
	}
}

void Boid::wrap_around_edges() {
	Vector2 pos = get_position();

	if (pos.x < boundary_min.x) pos.x = boundary_max.x;
	if (pos.x > boundary_max.x) pos.x = boundary_min.x;
	if (pos.y < boundary_min.y) pos.y = boundary_max.y;
	if (pos.y > boundary_max.y) pos.y = boundary_min.y;

	set_position(pos);
}

// Getters and Setters
void Boid::set_max_speed(float p_speed) {
	max_speed = Math::max(0.0f, p_speed);
}

float Boid::get_max_speed() const {
	return max_speed;
}

void Boid::set_max_force(float p_force) {
	max_force = Math::max(0.0f, p_force);
}

float Boid::get_max_force() const {
	return max_force;
}

void Boid::set_perception_radius(float p_radius) {
	perception_radius = Math::max(0.0f, p_radius);
}

float Boid::get_perception_radius() const {
	return perception_radius;
}

void Boid::set_separation_weight(float p_weight) {
	separation_weight = Math::max(0.0f, p_weight);
}

float Boid::get_separation_weight() const {
	return separation_weight;
}

void Boid::set_alignment_weight(float p_weight) {
	alignment_weight = Math::max(0.0f, p_weight);
}

float Boid::get_alignment_weight() const {
	return alignment_weight;
}

void Boid::set_cohesion_weight(float p_weight) {
	cohesion_weight = Math::max(0.0f, p_weight);
}

float Boid::get_cohesion_weight() const {
	return cohesion_weight;
}

void Boid::set_target_weight(float p_weight) {
	target_weight = Math::max(0.0f, p_weight);
}

float Boid::get_target_weight() const {
	return target_weight;
}

void Boid::set_boundary_min(Vector2 p_min) {
	boundary_min = p_min;
}

Vector2 Boid::get_boundary_min() const {
	return boundary_min;
}

void Boid::set_boundary_max(Vector2 p_max) {
	boundary_max = p_max;
}

Vector2 Boid::get_boundary_max() const {
	return boundary_max;
}

Vector2 Boid::get_velocity() const {
	return velocity;
}

void Boid::set_velocity(Vector2 p_velocity) {
	velocity = p_velocity;
}
