#version 330 core
out vec4 fragColor;
in vec4 vsColor;

//uniform vec4 ourColor;

void main() {
    //fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    //fragColor = vertexColor;
    fragColor = vsColor;
}