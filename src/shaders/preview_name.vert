R"zzz(#version 330 core
uniform mat4 ortho;
in vec4 screen_coords;
in vec4 tex_coords;


out vec4 TexCoords;

void main() {
    gl_Position = ortho * screen_coords;
    TexCoords = tex_coords;
}
)zzz"
