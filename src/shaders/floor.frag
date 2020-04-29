R"zzz(
#version 330 core

uniform sampler2D sampler0;
uniform sampler2D sampler1;
uniform sampler2D sampler2;
uniform sampler2D sampler3;
uniform sampler2D sampler4;
uniform sampler2D sampler5;
uniform mat4 light_view[];
uniform mat4 light_proj;
uniform vec4 light_position;


in vec4 face_normal;
in vec4 vertex_normal;
in vec4 light_direction;
in vec4 world_position;
out vec4 fragment_color;

int cubeMapNum(vec4 position) {
	float yDiff = abs(position.y - light_position.y);
	float xDiff = abs(position.x - light_position.x);
	float zDiff = abs(position.z - light_position.z);

	if (zDiff >= xDiff && zDiff >= yDiff) {
		if (position.z > light_position.z) {
			return 0;
		}

		return 1;
	}
	

	if (yDiff >= xDiff && yDiff >= zDiff) {
		if (position.y > light_position.y) {
			return 2;
		}

		return 3;
	}
	
	if (xDiff >= yDiff && xDiff >= zDiff) {
		if (position.x > light_position.x) {
			return 4;
		}
	
		return 5;
	}

	return 0;

}


float deep(int number, vec4 pos) {

	vec2 texCoord;
	texCoord[0] = (pos.x + 1.0) / 2.0;
	texCoord[1] = (pos.y + 1.0) / 2.0;

	if (number == 0) {
		return texture(sampler0, texCoord).x; 
	}

	if (number == 1) {
		return texture(sampler1, texCoord).x;
	}

	if (number == 2) {
		return texture(sampler2, texCoord).x;
	}

	if (number == 3) {
		return texture(sampler3, texCoord).x;
	}

	if (number == 4) {
		return texture(sampler4, texCoord).x;
	}

	if (number == 5) {
		return texture(sampler5, texCoord).x;
	}


	return -3.0;

}


void main() {
	vec4 pos = world_position;
	float check_width = 5.0;
	float i = floor(pos.x / check_width);
	float j  = floor(pos.z / check_width);
	vec3 color = mod(i + j, 2) * vec3(1.0, 1.0, 1.0);
	float dot_nl = dot(normalize(light_direction), normalize(face_normal));
	dot_nl = clamp(dot_nl, 0.0, 1.0);
	color = clamp(dot_nl * color, 0.0, 1.0);
	fragment_color = vec4(color, 1.0);

	/*

	//Calculates distance from current position to light
	vec3 light_pos = vec3(light_position[0], light_position[1], light_position[2]);
	vec3 world_pos = vec3(world_position[0], world_position[1], world_position[2]);
	float dist = distance(light_pos, world_pos);
	
	//Calculates distance from front coordinate to light
	int number = cubeMapNum(world_position);
	vec4 light_coords = light_proj * light_view[number] * world_position;
	float dep = deep(number, light_coords);

	
	
	//If front distance < current distance then
	if (light_coords.z > -1.0 && light_coords.z < 1.0 && dist - 0.1 > dep) {
		fragment_color = vec4(abs(dep) / 2.0, 0, 0, 1.0);
	}

	*/

	
	
	
	


}
)zzz"
