using Godot;

public partial class Player : CharacterBody2D
{
	[Export] public float Speed = 200f;

	public override void _PhysicsProcess(double delta)
	{
		// Get input and calculate velocity ( chacha slide )
		Vector2 velocity = Velocity;
		Vector2 inputDirection = Input.GetVector("ui_left", "ui_right", "ui_up", "ui_down");

		if (inputDirection != Vector2.Zero)
		{
			velocity = inputDirection * Speed;
		}
		else
		{
			velocity = velocity.Lerp(Vector2.Zero, 0.1f);
		}

		Velocity = velocity;
		MoveAndSlide();
	}
}
