using Godot;

public partial class MainScene : Node2D
{
	public override void _Ready()
	{
		GD.Print("Assignment 5: Rendering and Physics loaded successfully!");
	}

	public override void _PhysicsProcess(double delta)
	{
	}
}
