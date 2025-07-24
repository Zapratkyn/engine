#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec4 backgroundColor;

void main() {
    vec4 texColor = texture(texture1, TexCoord);
    if (texColor.a < 0.0) {
        texColor = vec4(backgroundColor.rgb, 1.0);
    }
    FragColor = texColor;
}
