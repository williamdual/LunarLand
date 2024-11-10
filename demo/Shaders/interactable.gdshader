shader_type spatial;

render_mode skip_vertex_transform; // self-explanitory
render_mode unshaded; // turns off environment lighting

varying smooth vec3 position_interp;
uniform bool in_range;
uniform bool glows;
uniform sampler2D sampler;

void vertex() {
	// Called for every vertex the material is visible on.
	POSITION = (PROJECTION_MATRIX * VIEW_MATRIX * (MODEL_MATRIX * vec4(VERTEX, 1.0)));
    
	// VIEW SPACE FOR LIGHTING CALCULATIONS !
    position_interp = (VIEW_MATRIX * (MODEL_MATRIX * vec4(VERTEX, 1.0))).xyz;
	
   	NORMAL = vec3(MODELVIEW_NORMAL_MATRIX * NORMAL);
}

void fragment() {
	// Glowing colour
	vec3 glow_col = vec3(0.0196, 0.8353, 0.9804);
	
	// Setting the colour based on the player distance to the interactable
	vec4 tex = texture(sampler, UV);
	if (in_range && glows) {
		ALBEDO = tex.rgb * glow_col;
	} else if (in_range) {
		ALBEDO = glow_col;
	} else {
		ALBEDO = tex.rgb;
	}
}
