using Godot;

public partial class LaserDetector : Node2D
{
	[Export] public float LaserLength = 500f;
	[Export] public Color LaserColorNormal = Colors.Green;
	[Export] public Color LaserColorAlert = Colors.Red;
	[Export] public NodePath PlayerPath;

	private RayCast2D _rayCast;
	private Line2D _laserBeam;
	private Node2D _player;
	private bool _isAlarmActive = false;
	private Timer _alarmTimer;
	private float _alarmDuration = 0.5f;
	private float _alarmTimeRemaining = 0.0f;

	public override void _Ready()
	{
		SetupRaycast();
		SetupVisuals();

		// Get the player reference
		if (PlayerPath != null)
		{
			_player = GetNode<Node2D>(PlayerPath);
		}

		// Setup alarm timer
		_alarmTimer = new Timer();
		AddChild(_alarmTimer);
		_alarmTimer.Connect(Timer.SignalName.Timeout, Callable.From(ResetAlarm));
	}

	private void SetupRaycast()
	{
		// Create and configure RayyCast2D
		_rayCast = new RayCast2D();
		_rayCast.TargetPosition = new Vector2(LaserLength, 0);
		_rayCast.CollisionMask = 1; // Adjust based on your collision layers
		AddChild(_rayCast);
	}

	private void SetupVisuals()
	{
		// Create Line2D for laser visuals
		_laserBeam = new Line2D();
		_laserBeam.Width = 2f;
		_laserBeam.DefaultColor = LaserColorNormal;
		AddChild(_laserBeam);

		// Add initial points
		_laserBeam.AddPoint(Vector2.Zero);
		_laserBeam.AddPoint(new Vector2(LaserLength, 0));
	}

	public override void _PhysicsProcess(double delta)
	{
		// Force the raycast to update
		_rayCast.ForceRaycastUpdate();

		// Check if raycast is colliding with player
		if (_rayCast.IsColliding())
		{
			// Get collision point in global coordinates
			Vector2 globalCollisionPoint = _rayCast.GetCollisionPoint();
			// Convert to local coordinates
			Vector2 localCollisionPoint = globalCollisionPoint - GlobalPosition;

			// Only take the X component ( laser is horizontal )
			float laserEndDistance = localCollisionPoint.X;

			if (laserEndDistance > LaserLength) laserEndDistance = LaserLength;
			if (laserEndDistance < 0) laserEndDistance = 0;

			UpdateLaserBeam(new Vector2(laserEndDistance, 0));

			// Check if hit object is the player
			GodotObject colliderObject = _rayCast.GetCollider();
			if (colliderObject is Node collider)
			{
				GD.Print($"Laser hit: {collider.Name}, Player: {_player?.Name}");
				if (collider == _player || (collider is Node2D node && node.IsAncestorOf(_player)))
				{
					TriggerAlarm();
				}
			}
		}
		else
		{
			// No collision, show full laser lenth
			UpdateLaserBeam(new Vector2(LaserLength, 0));
		}

		// Update alarm state
		if (_isAlarmActive)
		{
			_alarmTimeRemaining -= (float)delta;
			if (_alarmTimeRemaining <= 0)
			{
				ResetAlarm();
			}
		}
	}

	private void UpdateLaserBeam(Vector2 endPoint)
	{
		_laserBeam.ClearPoints();
		_laserBeam.AddPoint(Vector2.Zero);
		_laserBeam.AddPoint(endPoint);
	}

	private void TriggerAlarm()
	{
		if (!_isAlarmActive)
		{
			_isAlarmActive = true;
			_alarmTimeRemaining = _alarmDuration;

			// Change laser color to alert REDDDD
			_laserBeam.DefaultColor = LaserColorAlert;

			// Print alarm message
			GD.Print("ALARM! Player detected!");

			// Add visual feedback - make laser flash
			// Could add modulation for flashing effect
		}
	}

	private void ResetAlarm()
	{
		_isAlarmActive = false;
		_laserBeam.DefaultColor = LaserColorNormal;
	}
}
