R"zzz(#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
uniform mat4 light_view;
uniform mat4 light_proj;
out vec4 world_position;
void main() {
	int n = 0;
	for (n = 0; n < gl_in.length(); n++) {
		world_position = gl_in[n].gl_Position;
		gl_Position = light_proj * light_view * gl_in[n].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}
)zzz"
