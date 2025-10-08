# Flocking Boids + DVD Logo CollabGDExtension Demo

This project demonstrates **flocking behavior** based on Craig Reynolds' Boids algorithm, implemented as a Godot GDExtension in C++. The flocking system is integrated with the classic bouncing DVD logo screensaver to create an interactive chase mechanic.

## Features

The DVD logo bounces around the screen while a flock of 30 boids (autonomous agents) chase after it using flocking algorithms. The boids exhibit emergent group behavior:

Note: to see the rules in full effect they may need to be set to extreme values

1. **Separation** - Avoid crowding neighbors
2. **Alignment** - Steer towards the average heading of neighbors
3. **Cohesion** - Steer towards the average position of neighbors
4. **Target Seeking** - Chase the DVD logo

This creates a dynamic, organic-feeling swarm that pursues the player (DVD logo) while maintaining realistic group movement patterns.

## Flocking Algorithm Implementation

### Core Classes

#### 1. **Boid** (src/boid.h, src/boid.cpp)
A custom Sprite2D that represents an individual agent in the flock.

**Flocking Properties:**
- `velocity` - Current movement direction and speed
- `acceleration` - Accumulated forces from flocking behaviors
- `max_speed` Maximum velocity magnitude
- `max_force` Maximum steering force
- `perception_radius` Range for detecting neighbors

**Behavior Weights:**
- `separation_weight` Avoidance force strength
- `alignment_weight` Group alignment strength
- `cohesion_weight` Attraction to group center
- `target_weight` Chase behavior strength

#### 2. **FlockManager**
A Node2D that manages the entire flock and applies flocking rules.

**Properties:**
- `boid_count` (default: 30) - Number of boids in the flock
- `target_position` - Current position of the DVD logo to chase
- `follow_target_enabled` (default: true) - Enable/disable chase behavior
- `spawn_area_min/max` - Boundaries for boid spawning and movement

**Functionality:**
- Spawns boids with randomized positions and velocities
- Updates each boid by calculating and applying all flocking forces
- Continuously updates target position to track the DVD logo

#### 3. **EnhancedSprite**
The bouncing DVD logo that serves as the target for the flock.

**Features:**
- Bounces within defined boundaries with configurable speed
- Changes color over time for visual appeal
- Emits signals on boundary collisions

### How Flocking Improves the Game

**Game Design Integration:**
The flocking algorithm transforms a simple screensaver into an engaging visual experience:

1. **Visual Interest** - The organic, fluid movement of the flock creates mesmerizing patterns that are more engaging than the single bouncing logo alone.

2. **Emergent Behavior** - Each boid follows simple rules, but the flock exhibits complex, lifelike group behavior that never repeats exactly the same way. Like a weird digital lava lamp.

3. **Dynamic Difficulty** - By adjusting weights and perception radius, the flock can be made more aggressive (higher target_weight) or more independent (lower target_weight).

4. **Performance** - Demonstrates real-time simulation of 30 autonomous agents with neighbor detection and multi-force physics calculations.

**Feel & Experience:**
- The swarm creates a sense of pursuit and energy
- The boids appear "alive" rather than mechanically following a path
- The interaction between individual boid behaviors and group dynamics is visually satisfying
- Color-coded boids make it easy to track individual agents within the flock

## Controls

- **Start Button** - Begins the DVD logo animation and boid chase
- **Stop Button** - Pauses all movement
- **Reset Button** - Returns DVD logo to starting position
