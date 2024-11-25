shader_type particles;

// Attribution: Burning torus particle effect adapted from the provided code in the OpenGL version of COMP3501.

uniform float num_particles;

const float loop_radius = 2.0;
const float circle_radius = 1.0;
const float maxspray = 0.5;

const vec3 base_color = vec3(0.8, 0.15, 0.01);

#include "res://shaders/helpers.gdshaderinc"

// TRANSFORM[3].xyz is starting position/orientation in this function
// VELOCITY
// COLOR
void start(){
	// calculate color; check in spatial shader to see how the alpha is used!
	float u, v, w;
	u = pseudo_random_from_inputs(float(INDEX), 0.23678);
	v = pseudo_random_from_inputs(float(INDEX), 0.6321);
	w = pseudo_random_from_inputs(float(INDEX), 0.7654);
	//COLOR = vec4(u, v, w, 1.0) * 0.1; 
	COLOR = vec4(base_color, 0.4);
	
	// calculate starting position
	u = pseudo_random_from_inputs(float(INDEX), 0.4532);
	v = pseudo_random_from_inputs(float(INDEX), 1.2942);
	// 
	float theta, phi;
	theta = u * 2.0*PI; // random angle in 360 degree range
	phi = v * 2.0*PI; // random angle in 360 degree range
	
    vec3 normal = vec3(cos(theta)*cos(phi), sin(theta)*cos(phi), sin(phi));
    vec3 center = vec3(loop_radius*cos(theta), loop_radius*sin(theta), 0.0);
	float pos_offset = pseudo_random_from_inputs(float(INDEX), 0.1);
    vec3 position = vec3(loop_radius*pos_offset*cos(theta), 0.0, loop_radius*pos_offset*sin(theta));
	COLOR = vec4(float(INDEX)/num_particles, 0.0, 1.0 - (float(INDEX)/num_particles), 0.3); // The red channel of the color stores the particle id
		
	u = pseudo_random_from_inputs(float(INDEX), 0.3183);
	v = pseudo_random_from_inputs(float(INDEX), 0.1420);
    // Use u to define the angle theta along one direction of the sphere
    theta = u * 2.0*PI;
    // Use v to define the angle phi along the other direction of the sphere
    phi = acos(2.0*v - 1.0);
    // Use w to define how much we can deviate from the surface of the sphere (change of radius)
    float spray = maxspray*pow(w, (1.0/3.0)); // Cubic root of w

    // Define the normal and point based on theta, phi and the spray
    vec3 wander = vec3(spray*cos(theta)*sin(phi) * 0.5, spray*sin(theta)*sin(phi) * 1.5, spray*cos(phi) * 0.5);

    // Assign the wander direction to the normal
    normal = wander;
	
	VELOCITY = vec3(0.0, 0.0, 0.0); // We are going to simulate the motion in the vertex shader!
	
	// calculated above to position the particles at their 'birth' location!
	TRANSFORM[3].xyz = position;
	
	// EMISSION_TRANSFORM is the transformation for the GPUParticles3D Node
	TRANSFORM = EMISSION_TRANSFORM * TRANSFORM; // if you want the transform to be in local space rather than working in world space
	
	// We will use the custom x for indeX, and the other 3 components of the vector for the wander
	CUSTOM.xyz = wander;
	CUSTOM.w = LIFETIME * 0.5;
}

void process(){
}