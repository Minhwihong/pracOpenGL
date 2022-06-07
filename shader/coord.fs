#version 330 core
out vec4 fragColor;

in vec4 vsColor;


void main() {

    fragColor = vsColor;
}