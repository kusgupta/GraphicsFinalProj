R"zzz(
#version 330 core
in vec4 vertex_position;
in vec4 diffuse;
uniform mat4 view;
uniform mat4 projection;

out vec4 color;
void main() {
	gl_Position = projection * view * vertex_position;
	color = diffuse;
}
)zzz"
