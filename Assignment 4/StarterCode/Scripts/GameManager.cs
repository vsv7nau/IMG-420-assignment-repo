using Godot;

/// <summary>
/// Manages game state including player respawning when killed by enemy.
/// </summary>
public partial class GameManager : Node
{
    private Player _player;
    private Vector2 _playerSpawnPosition;
    private float _respawnTimer = 0f;
    private const float RESPAWN_DELAY = 5f;
    private bool _isRespawning = false;
    private float _invulnerabilityTimer = 0f;
    private const float INVULNERABILITY_DURATION = 3f;
    public bool IsPlayerInvulnerable { get; private set; } = false;
    private Label _respawnLabel;

    public override void _Ready()
    {
        // Find player in the scene tree
        _player = GetTree().Root.GetNode<Player>("Main/Level/Player");
        if (_player != null)
        {
            _playerSpawnPosition = _player.GlobalPosition;
            GD.Print("GameManager: Found player at " + _playerSpawnPosition);
        }
        else
        {
            GD.PrintErr("GameManager: Could not find player!");
        }

        // Find the respawn label
        _respawnLabel = GetTree().Root.GetNode<Label>("Main/Level/UI/RespawnLabel");
        if (_respawnLabel != null)
        {
            _respawnLabel.Text = "";
        }
    }

    public override void _PhysicsProcess(double delta)
    {
        // Hrespawn timer
        if (_isRespawning)
        {
            _respawnTimer -= (float)delta;

            // Update label with a countdown
            if (_respawnLabel != null)
            {
                int countdownSeconds = (int)Mathf.Ceil(_respawnTimer);
                _respawnLabel.Text = "Respawning in: " + countdownSeconds + "s";
                _respawnLabel.AddThemeColorOverride("font_color", Colors.Red);
            }

            if (_respawnTimer <= 0)
            {
                RespawnPlayer();
            }
        }
        else if (_respawnLabel != null)
        {
            _respawnLabel.Text = "";
        }

        // invulnerability timer
        if (IsPlayerInvulnerable)
        {
            _invulnerabilityTimer -= (float)delta;
            if (_invulnerabilityTimer <= 0)
            {
                IsPlayerInvulnerable = false;
                GD.Print("Invulnerability ended");
            }
        }
    }

    /// <summary>
    /// Called when the player dies ( touched the enemy )
    /// </summary>
    public void OnPlayerDied()
    {
        if (_player != null)
        {
            GD.Print("Player died! Respawning in 5 seconds...");
            _player.Visible = false;
            _player.SetPhysicsProcess(false);
            _player.ProcessMode = ProcessModeEnum.Disabled;
            _isRespawning = true;
            _respawnTimer = RESPAWN_DELAY;
        }
    }

    /// <summary>
    /// Respawns the player at the set spawn position
    /// </summary>
    private void RespawnPlayer()
    {
        if (_player != null)
        {
            GD.Print("Respawning player...");
            _player.GlobalPosition = _playerSpawnPosition;
            _player.Visible = true;
            _player.SetPhysicsProcess(true);
            _player.ProcessMode = ProcessModeEnum.Inherit;
            _player.Velocity = Vector2.Zero;
            _isRespawning = false;

            // Grant invulnerability for 3 seconds
            IsPlayerInvulnerable = true;
            _invulnerabilityTimer = INVULNERABILITY_DURATION;
            GD.Print("Player respawned at " + _playerSpawnPosition + " with 3 seconds invulnerability");
        }
    }
}
