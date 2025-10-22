using Godot;

/// <summary>
/// A VERY simple enemy that chases the player. The enemy will continuously move towards
/// the player's position and avoid walls ( kinda ) through the physics engine.
/// When the enemy collides with the player, the player dies and respawns.
/// </summary>
public partial class Enemy : CharacterBody2D
{
    [Export]
    public float Speed = 100f;

    /// <summary>
    /// Exposed NodePath to assign the target ( Player ) in the editor.
    /// </summary>
    [Export]
    public NodePath TargetPath;

    private Node2D _target;
    private GameManager _gameManager;

    public override void _Ready()
    {
        if (TargetPath != null)
        {
            _target = GetNode<Node2D>(TargetPath);
        }

        // Get ref to GameManager
        _gameManager = GetTree().Root.GetNode<GameManager>("Main/GameManager");
        if (_gameManager == null)
        {
            GD.PrintErr("Enemy: Could not find GameManager!");
        }
    }

    public override void _PhysicsProcess(double delta)
    {
        if (_target == null)
            return;

        // Don't chase if player is invisible ( dead/respawning )
        if (!_target.Visible)
        {
            Velocity = Vector2.Zero;
            return;
        }

        // Calculate direction from enemy to player
        Vector2 direction = (_target.GlobalPosition - GlobalPosition).Normalized();

        // Move towards the target
        Velocity = direction * Speed;
        var collision = MoveAndCollide(Velocity * (float)delta);

        // Check if collided with the player
        if (collision != null && collision.GetCollider() is Player player)
        {
            // Only kill if player is not invulnerable
            if (_gameManager != null && !_gameManager.IsPlayerInvulnerable)
            {
                GD.Print("Enemy killed player!");
                _gameManager.OnPlayerDied();
            }
        }
    }
}