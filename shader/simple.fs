#version 330 core
out vec4 fragColor;
in vec4 vsColor;
in vec2 texCoord;


uniform sampler2D tex;

void main() {
    //fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    //fragColor = vertexColor;
    fragColor = texture(tex, texCoord);
}