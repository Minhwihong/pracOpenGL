#version 330 core

in vec3 normal;
in vec3 position;
in vec2 texCoord;
out vec4 fragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientStrength;
// uniform sampler2D tex;
// uniform sampler2D tex2;

void main() {
    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDir = normalize(lightPos - position);
    vec3 pixelNorm = normalize(normal);
    vec3 diffuse = max(dot(pixelNorm, lightDir), 0.0) * lightColor; // ignore minus val of dot product

    vec3 result = (ambient + diffuse) * objectColor;

    fragColor = vec4(result, 1.0);
    //fragColor = texture(tex, texCoord)*0.8 + texture(tex2, texCoord) * 0.2;
}