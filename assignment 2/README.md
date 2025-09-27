# Bouncing DVD logo GDExtension Enhanced Sprite2D Demo

This project demonstrates the extension Godot's **Sprite2D** via GDExtension in C++ to add
new interactive features in the form of my favorite thing to stare at for HOURS, the magical bouncing DVD logo.
( I decided not to upgrade the features of my assignments 1's 2D sprite because that code was MESSY and I wasnt sure at all what features I could add that would make sense )

## EnhancedSprite Features

### 1. Bouncing Movement

-   Moves automatically within a defined rectangular space which can be changed
-   Properties:
    -   `bounce_speed` (float, default 200)
    -   `bounce_area_size` (Vector2, default 800x600)
    -   `bounce_enabled` (bool, default true)
-   Behavior: reverses direction at collision with boundaries, emits
    `boundary_hit(side)`

### 2. Color Cycling( extra credit feature )

-   Cycles through the sprites colors over time similar to how the DVD logos would
-   Properties:
    -   `cycle_time` (float, default 2.0s)
    -   `color_cycle_colors` (Array\[Color\], defaults to RGBA set)
    -   `color_cycling_enabled` (bool, default true)
-   Behavior: Choppy transitions, loops through the pre-set palette ( just like the DVD logo! )

## Signals & Methods

**Signals**\
- `boundary_hit(side)` -- emitted on boundary collision
- `effects_started()` -- both features activated
- `effects_stopped()` -- both features deactivated

**Methods**\
- `start_effects()` -- enable bounce + cycling
- `stop_effects()` -- disable both
- `reset_position()` -- reset position & color




