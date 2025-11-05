shader_type canvas_item;

// Uniforms for animation and visual effects
uniform float wave_intensity : hint_range(0.0, 1.0) = 0.1;
uniform vec4 color_start : source_color = vec4(1.0, 0.5, 0.0, 1.0);
uniform vec4 color_end : source_color = vec4(1.0, 0.0, 0.5, 1.0);
uniform float animation_speed : hint_range(0.5, 3.0) = 1.0;

void fragment()
{
	// Get UV coordinates
	vec2 uv = UV;

	// Apply wave distortion using time and UV position
	uv.x += sin(uv.y * 10.0 + TIME * animation_speed) * wave_intensity;

	// Create color gradient based on UV.y
	vec4 gradient_color = mix(color_start, color_end, uv.y);

	// Create a radial gradient for the particle shape (circle)
	float distance = length(uv - vec2(0.5, 0.5));
	float alpha = smoothstep(0.5, 0.0, distance);

	// Apply the gradient color with calculated alpha
	COLOR = vec4(gradient_color.rgb, gradient_color.a * alpha);
}
