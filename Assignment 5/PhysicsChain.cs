using Godot;
using System.Collections.Generic;

public partial class PhysicsChain : Node2D
{
	[Export] public int ChainSegments = 5;
	[Export] public float SegmentDistance = 30f;
	[Export] public Color SegmentColor = Colors.White;
	[Export] public float JointSoftness = 0.95f;  // Higher = looser chain
	[Export] public float SegmentMass = 0.2f;     // Lower = easier to move
	[Export] public float PushForce = 300f;       // Force multiplier for pushing

	private List<RigidBody2D> _segments = new List<RigidBody2D>();
	private List<Joint2D> _joints = new List<Joint2D>();

	public override void _Ready()
	{
		CreateChain();

		foreach (var segment in _segments)
		{
			segment.LinearVelocity = new Vector2(0, 50f);  // Start falling
		}
	}

	public override void _PhysicsProcess(double delta)
	{
		// Apply outward force to segments to help them spread and move more freely
		for (int i = 0; i < _segments.Count; i++)
		{
			// Small random perturbations to encourage movement
			var randomForce = new Vector2(
				(float)(GD.Randf() - 0.5f) * PushForce * 0.1f,
				0
			);
			_segments[i].ApplyForce(randomForce);
		}
	}

	private void CreateChain()
	{
		Vector2 startPosition = Vector2.Zero;  // Local position within this node
		Node2D previousSegment = null;

		// Create the anchor point using StaticBody2D (immovable)
		var anchor = new StaticBody2D();
		anchor.Position = startPosition;
		AddChild(anchor);

		var anchorShape = new CollisionShape2D();
		anchorShape.Shape = new RectangleShape2D() { Size = new Vector2(20, 20) };
		anchor.AddChild(anchorShape);

		previousSegment = anchor;

		// Create chain segments
		for (int i = 0; i < ChainSegments; i++)
		{
			// Create RigidBody2D segment
			var segment = new RigidBody2D();
			segment.Position = startPosition + new Vector2(0, (i + 1) * SegmentDistance);
			segment.GravityScale = 1.0f;
			segment.LockRotation = false;
			segment.Mass = SegmentMass;
			AddChild(segment);

			// Add collision shape
			var collisionShape = new CollisionShape2D();
			collisionShape.Shape = new RectangleShape2D() { Size = new Vector2(10, 20) };
			segment.AddChild(collisionShape);

			// Add visual representation
			var colorRect = new ColorRect();
			colorRect.Color = SegmentColor;
			colorRect.Size = new Vector2(10, 20);
			colorRect.Position = new Vector2(-5, -10);
			segment.AddChild(colorRect);

			_segments.Add(segment);

			// Create joint between previous and current segment
			var joint = new PinJoint2D();
			joint.NodeA = previousSegment.GetPath();
			joint.NodeB = segment.GetPath();
			joint.Softness = JointSoftness;
			// Position joint at the midpoint between the two segments
			joint.Position = (previousSegment.Position + segment.Position) / 2;
			AddChild(joint);

			_joints.Add(joint);
			previousSegment = segment;
		}
	}

	public void ApplyForceToSegment(int segmentIndex, Vector2 force)
	{
		if (segmentIndex >= 0 && segmentIndex < _segments.Count)
		{
			_segments[segmentIndex].ApplyForce(force);
		}
	}

	public void ApplyImpulseToSegment(int segmentIndex, Vector2 impulse)
	{
		if (segmentIndex >= 0 && segmentIndex < _segments.Count)
		{
			_segments[segmentIndex].ApplyImpulse(impulse);
		}
	}
}
