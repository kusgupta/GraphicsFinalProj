R"zzz(
#version 330 core
uniform vec4 light_position;
in vec4 world_position;
out vec4 fragment_color;
void main() {

	vec3 light_pos = vec3(light_position.x, light_position.y, light_position.z);
	vec3 world_pos = vec3(world_position.x, world_position.y, world_position.z);
	float dist = distance(light_pos, world_pos);

	
	if (gl_FragDepth <= 1.1) {
		gl_FragDepth = 1000.0;
	}

	if (dist < gl_FragDepth) {
		gl_FragDepth = dist;
	}
	

	fragment_color = vec4(dist / 100.0, 0.0, 0.0, 1.0);

})zzz"
