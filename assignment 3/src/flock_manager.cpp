#include "flock_manager.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/image.hpp>

using namespace godot;

void FlockManager::_bind_methods() {
	// Bind flock management methods
	ClassDB::bind_method(D_METHOD("spawn_boids"), &FlockManager::spawn_boids);
	ClassDB::bind_method(D_METHOD("clear_boids"), &FlockManager::clear_boids);
	ClassDB::bind_method(D_METHOD("get_boids"), &FlockManager::get_boids);

	// Bind basic properties
	ClassDB::bind_method(D_METHOD("set_boid_count", "count"), &FlockManager::set_boid_count);
	ClassDB::bind_method(D_METHOD("get_boid_count"), &FlockManager::get_boid_count);
	ClassDB::add_property("FlockManager", PropertyInfo(Variant::INT, "boid_count", PROPERTY_HINT_RANGE, "1,200,1"), "set_boid_count", "get_boid_count");

	ClassDB::bind_method(D_METHOD("set_target_position", "position"), &FlockManager::set_target_position);
	ClassDB::bind_method(D_METHOD("get_target_position"), &FlockManager::get_target_position);
	ClassDB::add_property("FlockManager", PropertyInfo(Variant::VECTOR2, "target_position"), "set_target_position", "get_target_position");

	ClassDB::bind_method(D_METHOD("set_follow_target_enabled", "enabled"), &FlockManager::set_follow_target_enabled);
	ClassDB::bind_method(D_METHOD("get_follow_target_enabled"), &FlockManager::get_follow_target_enabled);
	ClassDB::add_property("FlockManager", PropertyInfo(Variant::BOOL, "follow_target_enabled"), "set_follow_target_enabled", "get_follow_target_enabled");

	// Bind spawn area
	ClassDB::bind_method(D_METHOD("set_spawn_area_min", "min"), &FlockManager::set_spawn_area_min);
	ClassDB::bind_method(D_METHOD("get_spawn_area_min"), &FlockManager::get_spawn_area_min);
	ClassDB::add_property("FlockManager", PropertyInfo(Variant::VECTOR2, "spawn_area_min"), "set_spawn_area_min", "get_spawn_area_min");

	ClassDB::bind_method(D_METHOD("set_spawn_area_max", "max"), &FlockManager::set_spawn_area_max);
	ClassDB::bind_method(D_METHOD("get_spawn_area_max"), &FlockManager::get_spawn_area_max);
	ClassDB::add_property("FlockManager", PropertyInfo(Variant::VECTOR2, "spawn_area_max"), "set_spawn_area_max", "get_spawn_area_max");

	// Bind boid template properties
	ClassDB::bind_method(D_METHOD("set_boid_max_speed", "speed"), &FlockManager::set_boid_max_speed);
	ClassDB::bind_method(D_METHOD("get_boid_max_speed"), &FlockManager::get_boid_max_speed);
	ClassDB::add_property("FlockManager", PropertyInfo(Variant::FLOAT, "boid_max_speed", PROPERTY_HINT_RANGE, "0,500,10"), "set_boid_max_speed", "get_boid_max_speed");

	ClassDB::bind_method(D_METHOD("set_boid_max_force", "force"), &FlockManager::set_boid_max_force);
	ClassDB::bind_method(D_METHOD("get_boid_max_force"), &FlockManager::get_boid_max_force);
	ClassDB::add_property("FlockManager", PropertyInfo(Variant::FLOAT, "boid_max_force", PROPERTY_HINT_RANGE, "0,10,0.1"), "set_boid_max_force", "get_boid_max_force");

	ClassDB::bind_method(D_METHOD("set_boid_perception_radius", "radius"), &FlockManager::set_boid_perception_radius);
	ClassDB::bind_method(D_METHOD("get_boid_perception_radius"), &FlockManager::get_boid_perception_radius);
	ClassDB::add_property("FlockManager", PropertyInfo(Variant::FLOAT, "boid_perception_radius", PROPERTY_HINT_RANGE, "0,200,10"), "set_boid_perception_radius", "get_boid_perception_radius");

	ClassDB::bind_method(D_METHOD("set_boid_separation_weight", "weight"), &FlockManager::set_boid_separation_weight);
	ClassDB::bind_method(D_METHOD("get_boid_separation_weight"), &FlockManager::get_boid_separation_weight);
	ClassDB::add_property("FlockManager", PropertyInfo(Variant::FLOAT, "boid_separation_weight", PROPERTY_HINT_RANGE, "0,5,0.1"), "set_boid_separation_weight", "get_boid_separation_weight");

	ClassDB::bind_method(D_METHOD("set_boid_alignment_weight", "weight"), &FlockManager::set_boid_alignment_weight);
	ClassDB::bind_method(D_METHOD("get_boid_alignment_weight"), &FlockManager::get_boid_alignment_weight);
	ClassDB::add_property("FlockManager", PropertyInfo(Variant::FLOAT, "boid_alignment_weight", PROPERTY_HINT_RANGE, "0,5,0.1"), "set_boid_alignment_weight", "get_boid_alignment_weight");

	ClassDB::bind_method(D_METHOD("set_boid_cohesion_weight", "weight"), &FlockManager::set_boid_cohesion_weight);
	ClassDB::bind_method(D_METHOD("get_boid_cohesion_weight"), &FlockManager::get_boid_cohesion_weight);
	ClassDB::add_property("FlockManager", PropertyInfo(Variant::FLOAT, "boid_cohesion_weight", PROPERTY_HINT_RANGE, "0,5,0.1"), "set_boid_cohesion_weight", "get_boid_cohesion_weight");

	ClassDB::bind_method(D_METHOD("set_boid_target_weight", "weight"), &FlockManager::set_boid_target_weight);
	ClassDB::bind_method(D_METHOD("get_boid_target_weight"), &FlockManager::get_boid_target_weight);
	ClassDB::add_property("FlockManager", PropertyInfo(Variant::FLOAT, "boid_target_weight", PROPERTY_HINT_RANGE, "0,5,0.1"), "set_boid_target_weight", "get_boid_target_weight");
}

FlockManager::FlockManager() {
	boid_count = 30;
	target_position = Vector2(400, 300);
	follow_target_enabled = true;

	spawn_area_min = Vector2(0, 0);
	spawn_area_max = Vector2(800, 600);

	// Default boid template settings
	boid_max_speed = 150.0f;
	boid_max_force = 3.0f;
	boid_perception_radius = 50.0f;
	boid_separation_weight = 1.5f;
	boid_alignment_weight = 1.0f;
	boid_cohesion_weight = 1.0f;
	boid_target_weight = 0.5f;

	initialized = false;
}

FlockManager::~FlockManager() {
}

void FlockManager::_ready() {
	initialized = true;
	spawn_boids();
	UtilityFunctions::print("FlockManager ready! Spawned ", boid_count, " boids.");
}

void FlockManager::_process(double delta) {
	if (!initialized) return;
	update_flock(delta);
}

void FlockManager::spawn_boids() {
	clear_boids();

	// Create a simple texture for boids
	Ref<Image> img = Image::create(8, 8, false, Image::FORMAT_RGBA8);

	// Fill with white color
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			img->set_pixel(x, y, Color(1, 1, 1, 1));
		}
	}

	Ref<ImageTexture> texture = ImageTexture::create_from_image(img);

	for (int i = 0; i < boid_count; i++) {
		Boid* boid = memnew(Boid);

		// Random spawn position
		float x = UtilityFunctions::randf() * (spawn_area_max.x - spawn_area_min.x) + spawn_area_min.x;
		float y = UtilityFunctions::randf() * (spawn_area_max.y - spawn_area_min.y) + spawn_area_min.y;
		boid->set_position(Vector2(x, y));

		// Set boid properties from template
		boid->set_max_speed(boid_max_speed);
		boid->set_max_force(boid_max_force);
		boid->set_perception_radius(boid_perception_radius);
		boid->set_separation_weight(boid_separation_weight);
		boid->set_alignment_weight(boid_alignment_weight);
		boid->set_cohesion_weight(boid_cohesion_weight);
		boid->set_target_weight(boid_target_weight);
		boid->set_boundary_min(spawn_area_min);
		boid->set_boundary_max(spawn_area_max);

		// Set texture and appearance
		boid->set_texture(texture);
		boid->set_scale(Vector2(1.5, 1.5));

		// Set random color for visual distinction
		Color color = Color(UtilityFunctions::randf() * 0.5 + 0.5, UtilityFunctions::randf() * 0.5 + 0.5, 1.0);
		boid->set_modulate(color);

		add_child(boid);
		boids.push_back(boid);
	}
}

void FlockManager::clear_boids() {
	for (int i = 0; i < boids.size(); i++) {
		Object* obj = boids[i];
		Boid* boid = Object::cast_to<Boid>(obj);
		if (boid != nullptr) {
			boid->queue_free();
		}
	}
	boids.clear();
}

void FlockManager::update_flock(double delta) {
	// Update each boid with flocking behaviors
	for (int i = 0; i < boids.size(); i++) {
		Object* obj = boids[i];
		Boid* boid = Object::cast_to<Boid>(obj);

		if (boid != nullptr) {
			// Calculate flocking forces
			Vector2 separation = boid->separation(boids) * boid->get_separation_weight();
			Vector2 alignment = boid->alignment(boids) * boid->get_alignment_weight();
			Vector2 cohesion = boid->cohesion(boids) * boid->get_cohesion_weight();

			// Apply flocking forces
			boid->apply_force(separation);
			boid->apply_force(alignment);
			boid->apply_force(cohesion);

			// If target following is enabled, add seek behavior
			if (follow_target_enabled) {
				Vector2 seek = boid->seek_target(target_position) * boid->get_target_weight();
				boid->apply_force(seek);
			}
		}
	}
}

// Getters and Setters
void FlockManager::set_boid_count(int p_count) {
	boid_count = Math::max(1, p_count);
}

int FlockManager::get_boid_count() const {
	return boid_count;
}

void FlockManager::set_target_position(Vector2 p_position) {
	target_position = p_position;
}

Vector2 FlockManager::get_target_position() const {
	return target_position;
}

void FlockManager::set_follow_target_enabled(bool p_enabled) {
	follow_target_enabled = p_enabled;
}

bool FlockManager::get_follow_target_enabled() const {
	return follow_target_enabled;
}

void FlockManager::set_spawn_area_min(Vector2 p_min) {
	spawn_area_min = p_min;
}

Vector2 FlockManager::get_spawn_area_min() const {
	return spawn_area_min;
}

void FlockManager::set_spawn_area_max(Vector2 p_max) {
	spawn_area_max = p_max;
}

Vector2 FlockManager::get_spawn_area_max() const {
	return spawn_area_max;
}

void FlockManager::set_boid_max_speed(float p_speed) {
	boid_max_speed = Math::max(0.0f, p_speed);
}

float FlockManager::get_boid_max_speed() const {
	return boid_max_speed;
}

void FlockManager::set_boid_max_force(float p_force) {
	boid_max_force = Math::max(0.0f, p_force);
}

float FlockManager::get_boid_max_force() const {
	return boid_max_force;
}

void FlockManager::set_boid_perception_radius(float p_radius) {
	boid_perception_radius = Math::max(0.0f, p_radius);
}

float FlockManager::get_boid_perception_radius() const {
	return boid_perception_radius;
}

void FlockManager::set_boid_separation_weight(float p_weight) {
	boid_separation_weight = Math::max(0.0f, p_weight);
}

float FlockManager::get_boid_separation_weight() const {
	return boid_separation_weight;
}

void FlockManager::set_boid_alignment_weight(float p_weight) {
	boid_alignment_weight = Math::max(0.0f, p_weight);
}

float FlockManager::get_boid_alignment_weight() const {
	return boid_alignment_weight;
}

void FlockManager::set_boid_cohesion_weight(float p_weight) {
	boid_cohesion_weight = Math::max(0.0f, p_weight);
}

float FlockManager::get_boid_cohesion_weight() const {
	return boid_cohesion_weight;
}

void FlockManager::set_boid_target_weight(float p_weight) {
	boid_target_weight = Math::max(0.0f, p_weight);
}

float FlockManager::get_boid_target_weight() const {
	return boid_target_weight;
}

Array FlockManager::get_boids() const {
	return boids;
}
