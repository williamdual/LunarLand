shader_type sky;

uniform sampler2D sampler: source_color, filter_linear;

void sky() {
	
	// Reducing the amount of calculations the sky shader is going to have to do
	if (AT_CUBEMAP_PASS) {
		COLOR = vec3(0.2, 0.6, 1.0);
	} else {
		vec4 tex = vec4(texture(sampler, SKY_COORDS).rgb, 1.0);
		COLOR = tex.rgb;
	}
}
