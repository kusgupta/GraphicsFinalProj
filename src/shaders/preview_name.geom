R"zzz(#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
in vec4 TexCoords[];

out vec4 texCoords;
void main() {
    

    for (int i = 0; i < gl_in.length(); i++) {
        texCoords = TexCoords[i];
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
)zzz"
