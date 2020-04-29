R"zzz(
#version 330 core
in vec4 face_normal;
in vec4 vertex_normal;
in vec4 light_direction;
in vec4 camera_direction;
in vec2 uv_coords;
//in vec4 light_coords;
in vec4 world_position;

uniform mat4 light_view[];
uniform mat4 light_proj;
uniform vec4 light_position;
uniform vec4 diffuse;
uniform vec4 ambient;
uniform vec4 specular;
uniform float shininess;
uniform float alpha;
uniform sampler2D textureSampler;
uniform sampler2D sampler0;
uniform sampler2D sampler1;
uniform sampler2D sampler2;
uniform sampler2D sampler3;
uniform sampler2D sampler4;
uniform sampler2D sampler5;
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
		if (position.x > light_position.y) {
			return 4;
		}
	
		return 5;
	}

	return 0;

}

float depth(int number, vec4 pos) {
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

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
void main() {
	vec3 texcolor = texture(textureSampler, uv_coords).xyz;
	if (length(texcolor) == 0.0) {
		//vec3 color = vec3(0.0, 1.0, 0.0);
		//vec3 color = vec3(diffuse);
		vec3 color = vec3(diffuse);
		//vec2 randuv = vec2(rand(light_direction.xy), rand(light_direction.zw));
		//vec3 color = vec3(diffuse) + texture(textureSampler, randuv).xyz;
		//vec3 color = texture(textureSampler, randuv).xyz;
		//vec3 color = vec3(diffuse) + vec3(randuv.x, randuv.y, 1.0);
		float dot_nl = dot(normalize(light_direction), normalize(vertex_normal));
		dot_nl = clamp(dot_nl, 0.0, 1.0);
		vec4 spec = specular * pow(max(0.0, dot(reflect(-light_direction, vertex_normal), camera_direction)), shininess);
		color = clamp(dot_nl * color + vec3(ambient) + vec3(spec), 0.0, 1.0);
		fragment_color = vec4(color, alpha);
	} else {
		fragment_color = vec4(texcolor.rgb, alpha);
	}
	
	/*
		//Calculates distance from current position to light
	vec3 light_pos = vec3(light_position[0], light_position[1], light_position[2]);
	vec3 world_pos = vec3(world_position[0], world_position[1], world_position[2]);
	float dist = distance(light_pos, world_pos);
	

	int number = cubeMapNum(world_position);
	vec4 light_coords = light_proj * light_view[number] * world_position;
	float dep = depth(number, light_coords);
	if (dep > 2) {
		fragment_color = vec4(dep / 20.0, 0.0, dep / 20.0, 1.0);
	}
	//fragment_color = vec4(0.2, 0.2, 0.2, 0.5);
	*/
}
)zzz"
