R"zzz(#version 330 core
in vec4 texCoords;

uniform sampler2D screenTexture;
out vec4 fragment_color;
void main() {
    fragment_color = vec4(1.0, 0.0, 0.0, 1.0);//vec4(texture(screenTexture, vec2(texCoords.x, texCoords.y)));
}
)zzz"
