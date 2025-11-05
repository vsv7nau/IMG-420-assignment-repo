# Assignment 5: Rendering and Physics, now with more particles, chains, and lasers! Woo!

This assignment demonstrates advanced Godot engine features including custom shaders, rigid body physics with joints, and raycasting for collision detection.

## Part 1: Custom Canvas Item Shader with Particles (3 points)

### How the Shader Works

The custom particle shader (`custom_particle.gdshader`) implements:

1. **Wave Distortion**: Uses a sine wave function to create undulating motion
   ```glsl
   uv.x += sin(uv.y * 10.0 + TIME * animation_speed) * wave_intensity;
   ```
   - Frequency of 10.0 creates tightly spaced waves
   - TIME variable ensures continuous animation
   - `animation_speed` uniform controls animation rate

2. **Color Gradient**: Creates a smooth transition between two colors based on vertical position
   ```glsl
   vec4 gradient_color = mix(color_start, color_end, uv.y);
   ```
   - `color_start`: Orange (1.0, 0.5, 0.0, 1.0)
   - `color_end`: Pink (1.0, 0.0, 0.5, 1.0)
   - Gradient flows from top to bottom

3. **Alpha Blending**: Uses a radial gradient to create smooth particle edges
   ```glsl
   float distance = length(uv - vec2(0.5, 0.5));
   float alpha = smoothstep(0.5, 0.0, distance);
   ```
   - Creates circular particle shapes with soft edges
   
### Implementation Details (ParticleController.cs)

- Configures GpuParticles2D with 100 particles
- Lifetime: 3 seconds
- ParticleProcessMaterial handles physics simulation:
  - Direction: Upward (-Y)
  - Spread: 45 degrees
  - Initial Velocity: 100 units/sec
  - Gravity: 0.5x strength
  - Damping: 0.1 (energy loss over time)

---

## Part 2: Advanced Rigid Body Physics with Joints

### Physics Properties Chosen and Why

The physics chain system uses `PinJoint2D` to connect segments. This choice was made because:

1. **PinJoint2D Benefits**:
   - Maintains distance between connected bodies
   - Realistic chain/rope behavior
   - Stable and predictable physics
   - Lower computational cost than spring-based constraints

2. **Configuration Parameters**:
   - `SegmentDistance = 30f`: Distance between each chain link (affects chain length)
   - `ChainSegments = 5`: Number of rigid bodies in the chain
   - `Softness = 0.1f`: Allows slight stretching for realistic flex
   - `Mass = 1.0f`: Each segment has equal weight for uniform dynamics

3. **Anchor Point**:
   - Uses StaticBody2D at the chain's start
   - Prevents the top from moving while rest of chain hangs naturally
   - Can be modified to use a RigidBody2D with constraints for moving anchors

### Force Application

The `ApplyForceToSegment()` and `ApplyImpulseToSegment()` methods allow:
- Player interaction with chain segments
- Realistic wave propagation through the chain
- Dynamic response to external forces

---

## Part 3: Raycasting Laser with Player Detection

### How the Raycast Detection Works

The laser detection system uses Godot's RayCast2D node for collision detection:

```
_PhysicsProcess()
  ↓
Force RayCast update
  ↓
Check if colliding?
  ├─ YES: Get collision point → Update visual to collision point
  │         Check if collider is player → Trigger alarm
  └─ NO: Update visual to full laser length
  ↓
Update alarm state
  ↓
Decay alarm (reset after timeout)
```

#### Alarm System:

- **Triggers**: When raycast detects player collision
- **Duration**: 0.5 seconds (configurable)
- **Feedback**:
  - Visual: Laser changes to red
  - Console: Prints "ALARM! Player detected!"
  - Could be extended with particles, sound effects, or camera shake