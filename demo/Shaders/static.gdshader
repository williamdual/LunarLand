shader_type spatial;

render_mode unshaded; // without this line, the world will attempt to apply lighting to the quad, which will make *everything* look darker (or lighter, depending)
render_mode skip_vertex_transform;
render_mode fog_disabled; // we aren't going to use fog, but putting this here anyways so that you know it would be necessary

uniform sampler2D screen_texture : source_color, hint_screen_texture, filter_nearest_mipmap;

uniform float glitch_size = 30;
uniform bool static;

float pseudo_random_from_inputs(float seed, float factor1){
	//return (mod((seed*factor1+factor2),factor3)/factor3);
	return fract(sin(dot(vec2((seed)*68.379 / 0.236472, factor1), // IGNORE THE ERROR ON THIS LINE
	 vec2(12.9898,78.233)))*48.5453123);
}

void vertex() {
	// Called for every vertex the material is visible on.
	POSITION = vec4(VERTEX, 1.0f); // which space is this? (clip space)
	//POSITION = vec4(VERTEX * 0.75, 1.0f); // what does this do?
}

void fragment() {
	//float range = sin(TIME);
    vec4 pixel = textureLod(screen_texture, SCREEN_UV, 0.0); // the third argument has to do with the mip-map level

	// Checking to see if the static effect should be applied
	if (static) {
		// Storing the initial UV coords before changing them
		vec2 offset_uv = SCREEN_UV;
		
		// Getting a random offsets for the glitch effect in blocks based on the groups of pixels to be shifted
		float offset_x = 0.0;
		if (pseudo_random_from_inputs(TIME, 1.0) > 0.99) {
			float glitch_height = floor(SCREEN_UV.y * glitch_size) / glitch_size;
			offset_x = pseudo_random_from_inputs(glitch_height * TIME, 1.0) * 0.05;
		}
		offset_uv.x += offset_x;
		pixel = textureLod(screen_texture, offset_uv, 0.0 );
		
		// Creating random fuzz colour modifier
		float rand_col = pseudo_random_from_inputs(TIME * SCREEN_UV.x * SCREEN_UV.y, 1.0);
		vec3 col = vec3(1.0, 1.0, 1.0) * rand_col;
		pixel = pixel * vec4(col, 1.0);
		
		// Converting to greyscale
		float grey_scale = pixel.r * 0.3 + pixel.g * 0.6 + pixel.b * 0.1;
		pixel = vec4(grey_scale, grey_scale, grey_scale, 1.0);
	}

	//pixel = textureLod(screen_texture, SCREEN_UV, 0.0);
    ALBEDO = pixel.rgb;
}
