shader_type spatial;

render_mode skip_vertex_transform; // self-explanitory
render_mode unshaded; // turns off environment lighting

varying smooth vec3 position_interp;
uniform sampler2D sampler;

uniform vec3 init_light_positions[32];
uniform vec3 light_colours[32];
uniform vec3 view_pos;
uniform float spec_power[32];
varying smooth vec3 light_positions[32];
uniform int num_lights;
uniform float darkening_val;

void vertex() {
	// Called for every vertex the material is visible on.
	POSITION = (PROJECTION_MATRIX * VIEW_MATRIX * (MODEL_MATRIX * vec4(VERTEX, 1.0)));
    
	// VIEW SPACE FOR LIGHTING CALCULATIONS !
    position_interp = (VIEW_MATRIX * (MODEL_MATRIX * vec4(VERTEX, 1.0))).xyz;
	for (int i = 0; i < num_lights; i++) {
		light_positions[i] = (VIEW_MATRIX * (vec4(init_light_positions[i], 1.0))).xyz;
	}
	
   	NORMAL = vec3(MODELVIEW_NORMAL_MATRIX * NORMAL);
}

void fragment() {
	
	// Calculating the lighting per light position
	vec3 final_light = vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < num_lights; i++) {
		
		// Lighting attenuation calculation
		float dist = distance(position_interp, light_positions[i]);
		float attenuation = 1.0 / (1.0 + 0.0075 * dist + 0.0075 * dist * dist);
		// The two 0.0075 constants are arbitrary since they appeared to create reasonable light attenuation
		// Replace later with uniforms if needed
		
		vec3 vv = normalize(view_pos - position_interp); // view direction
		vec3 lv = normalize(light_positions[i] - position_interp); // light direction, object position as origin
		vec3 h = normalize((vv+lv)); // halfway vector -- note /2 pointless, just there for clarity
		vec3 n = normalize(NORMAL); // must normalize interpolated normal
		vec3 r = normalize(-lv + 2.0 * (dot(lv, NORMAL) * NORMAL));

		float diffuse = dot(n,lv); // N.L -- anything wrong with this line?
		float spec = max(0.0,dot(n,h)); // cannot be negative - BLINN-PHONG
		//float spec = max(0.0,dot(vv,r));

		spec = pow(spec, spec_power[i]); // specular power -- instead of hardcoded number, should have uniform here too
		vec4 ambcol = vec4(1,0,0,1); // ambient light -- in this case red, could be anything; should make uniform

	    float amb = 0.4; // ambient coefficient
		
		final_light = final_light + attenuation * (vec4(light_colours[i], 1.0)*diffuse + vec4(light_colours[i], 1.0)*vec4(1,1,1,1)*spec + vec4(light_colours[i], 1.0)*amb).xyz;
	}
	
	// Applying the final light colour
	ALBEDO = ALBEDO * final_light.xyz;
	
	// Setting the colour based on the player distance to the interactable
	vec4 tex = texture(sampler, UV);
	ALBEDO = ALBEDO * tex.rgb * darkening_val;
}
