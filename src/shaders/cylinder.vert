R"zzz(#version 330 core
const float kPi = 3.1415926535897932384626433832795;
uniform mat4 bone_transform;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform float height;
uniform float radius;
in vec4 vertex_position;

void main() {
    float angle = vertex_position[0] * kPi * 2;
    vec4 transformed_vertex = vec4(radius * cos(angle), 2 * height * vertex_position[1], radius * sin(angle), 1.0);

    mat4 mvp = model * projection * view;
    gl_Position = mvp * bone_transform * transformed_vertex;
}
)zzz"
