#version 330 core

in vec3 normal;
in vec3 position;
in vec2 texCoord;
out vec4 fragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;        // 광원의 색상
uniform vec3 objectColor;       
uniform float ambientStrength;  // 주변광(물체의 기본 색상)의 전체 밝기를 나타냄
// uniform sampler2D tex;
// uniform sampler2D tex2;

void main() {
    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDir = normalize(lightPos - position);

    // openGL program -> vertex shader -> 에서 넘어온 normal을 다시 normalize 하는 이유
    // vertex shader에서 계산된 normal은 rasterization과정에서 선형보간이 진행됨
    // 단위벡터간의 선형보간 결과가 단위벡터임을 보장하지 못하기때문에 다시 normalize과정을 fragment shader과정에 진행
    vec3 pixelNorm = normalize(normal);


    vec3 diffuse = max(dot(pixelNorm, lightDir), 0.0) * lightColor; // ignore minus val of dot product


    vec3 result = (ambient + diffuse) * objectColor;

    fragColor = vec4(result, 1.0);
    //fragColor = texture(tex, texCoord)*0.8 + texture(tex2, texCoord) * 0.2;
}