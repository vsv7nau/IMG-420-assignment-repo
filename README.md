# GDExtension Enhanced Sprite Demo

This project extends Godot's **Sprite2D** via GDExtension in C++ to add
new interactive features.

## EnhancedSprite Features

### 1. Bouncing Movement

-   Moves automatically within a defined rectangular area\
-   Properties:
    -   `bounce_speed` (float, default 200)\
    -   `bounce_area_size` (Vector2, default 800x600)\
    -   `bounce_enabled` (bool, default true)\
-   Behavior: reverses direction at boundaries, emits
    `boundary_hit(side)`

### 2. Color Cycling

-   Cycles sprite colors over time\
-   Properties:
    -   `cycle_time` (float, default 2.0s)\
    -   `color_cycle_colors` (Array\[Color\], defaults to RGBA set)\
    -   `color_cycling_enabled` (bool, default true)\
-   Behavior: smooth transitions, loops palette

## Signals & Methods

**Signals**\
- `boundary_hit(side)` -- emitted on boundary collision\
- `effects_started()` -- both features activated\
- `effects_stopped()` -- both features deactivated

**Methods**\
- `start_effects()` -- enable bounce + cycling\
- `stop_effects()` -- disable both\
- `reset_position()` -- reset position & color

## Project Structure

    GDExtension_Demo/
    ├── src/                  # C++ code
    │   ├── enhanced_sprite.h/.cpp
    │   ├── register_types.h/.cpp
    ├── project/              # Godot project
    │   ├── enhanced_sprite.gdextension
    │   ├── main.tscn, main.gd
    │   └── project.godot
    └── SConstruct            # Build config

## Build Instructions

-   Requires Godot 4.1+, godot-cpp, SCons, and C++17 compiler

Example build (Windows Debug):

``` bash
scons platform=windows target=debug
```

Similar commands work for Linux/macOS with respective platforms.

## Usage

1.  Build extension\
2.  Open `project/project.godot` in Godot\
3.  Add **EnhancedSprite** node\
4.  Configure in Inspector (speed, area, colors)\
5.  Connect signals or call methods (`start_effects()`, etc.)

## Demo Scene

`main.tscn` shows:\
- UI buttons for start/stop/reset\
- Live feedback of collisions and state

------------------------------------------------------------------------

**Note:** `godot-cpp` is required but not included in this repo. Please
configure separately per Godot's GDExtension docs.
