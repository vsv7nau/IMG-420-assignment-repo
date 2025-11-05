using Godot;

public partial class ParticleController : GpuParticles2D
{
	private ShaderMaterial _shaderMaterial;
	private float _timeAccumulator = 0f;

	public override void _Ready()
	{
		// Load and apply the custom shader
		var shader = GD.Load<Shader>("res://custom_particle.gdshader");
		_shaderMaterial = new ShaderMaterial();
		_shaderMaterial.Shader = shader;

		// Configure particle properties
		Amount = 100;
		Lifetime = 3.0f;
		OneShot = false;
		Emitting = true;

		// Configure process material for particle physics
		var processMaterial = new ParticleProcessMaterial();
		processMaterial.Direction = new Vector3(0, -1, 0);
		processMaterial.Spread = 45f;
		processMaterial.InitialVelocity = new Vector2(0, -100);
		processMaterial.Scale = new Vector2(1.0f, 1.0f);

		// Set the process material to handle particle physics
		ProcessMaterial = processMaterial;

		// Apply the custom shader material for rendering
		Material = _shaderMaterial;
	}

	public override void _Process(double delta)
	{
		// Update shader parameters over time for animation
		if (_shaderMaterial != null)
		{
			_timeAccumulator += (float)delta;
			_shaderMaterial.SetShaderParameter("wave_intensity", Mathf.Sin(_timeAccumulator * 0.5f) * 0.2f + 0.1f);
		}
	}
}
