#version 330 core
out vec4 fragColor;

in vec4 vsColor;

uniform vec3 fColor;


void main() {
    //fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    //fragColor = vsColor;
    fragColor = vec4(fColor, 0.0);
    
}