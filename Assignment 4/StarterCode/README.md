# Assignment 4 - 2D Platformer Game

A uhhh- working 2D platformer game built in **Godot** using **C#**. This game features player movement with physics, an enemy AI that chases the player, collectible items with STUNNING particle effects, and a respawn system with invulnerability periods.

### Setup Instructions
1. Open Godot
2. Select "Import" and navigate to the `StarterCode` folder
3. Open the project
4. Wait for Godot to auto-import all assets and resources
5. Go to **Project > Tools > C# > Build Project** to compile the C# scripts
6. Press **F5** to run the game

## Game Controls
Move Left | Left Arrow or A
Move Right | Right Arrow or D
Jump | Spacebar

## Gameplay

### Player
- Use arrow keys to move left and right
- Press space to jump
- Gravity automatically pulls you down
- Player respawns 5 seconds after being hit by the enemy
- 3-second invulnerability period after respawn ( so you dont get stuck in a death loop )

### Enemy
- Chases the player continuously across the level
- Avoids walls through physics collision
- Touching the enemy eliminates the player
- Stops chasing when the player its dead ( invisible )

### Collectibles
- 3 gold coins on 3 platformss
- Collecting a coin triggers a particle burst effect
- Coin disappears after collection

### Level
- Tile-based platformer level with 4 platforms
- Floor platforms at different heights
- Player spawns on the left, enemy spawns on the right

## Features

**Player Movement** - Smooth character movement with arrow keys
**Physics & Gravity** - Realistic gravity and jumping mechanics
**Enemy AI** - Enemy that chases player and detects collisions
**Particle Effects** - Visual effects when collecting coins
**Collectibles** - Items that can be collected for visual feedback
**Respawn System** - Player respawns 5 seconds after death
**Invulnerability** - 3-second protection period after respawn
**Level Design** - Multi-platform level with strategic positioning
**UI Feedback** - Respawn countdown timer displayed on screen
**Tile-Based World** - TileSet and TileMapLayer for level construction

## Technical Details

### C# Scripts ( the more detailed stuff )

**Player.cs**
- Extends `CharacterBody2D`
- Handles input ( ARROW keys and SPACE )
- Applies gravity and jump physics
- Manages animation switching between idle/walk ( this I didnt get working in time but its there )
- Exported parameters: Speed ( 200 ), JumpVelocity ( -500 ), Gravity ( 1000 )

**Enemy.cs**
- Extends `CharacterBody2D`
- Calculates direction to player and moves toward it
- Detects collision with player via `MoveAndCollide()`
- Communicates with GameManager on player contact
- Respects invulnerability status
- Exported parameters: Speed ( 100 ), TargetPath ( ../Player )

**Pickup.cs**
- Extends `Area2D` for collision detection
- Detects when player enters the trigger
- Spawns particle effect at collection point
- Removes itself from scene after collection
- Exported parameter: ParticlesScene (PickupParticles.tscn)

**GameManager.cs**
- Manages game state ( respawnin and invulnerability )
- Tracks respawn timer ( 5 seconds )
- Grants 3-second invulnerability after respawn
- Updates respawn countdown label
- Handles player visibility and physics during respawn
