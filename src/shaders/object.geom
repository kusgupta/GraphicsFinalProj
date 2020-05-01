R"zzz(#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec4 color[];
out vec4 diffuse_color;

void main() {
	int n = 0;
	for (n = 0; n < 3; n++) {
		gl_Position = gl_in[n].gl_Position;
		diffuse_color = color[n];
		EmitVertex();
	}

	EndPrimitive();
}
)zzz"
