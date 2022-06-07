#version 330 core

in vec3 normal;
in vec3 position;
in vec2 texCoord;
out vec4 fragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;        // 광원의 색상
uniform vec3 objectColor;       
uniform float ambientStrength;  // 주변광(물체의 기본 색상)의 전체 밝기를 나타냄

uniform float specularStrength;
uniform float specularShininess;
uniform vec3 viewPos;           // 보고 있는 눈(카메라)의 위치


// uniform sampler2D tex;
// uniform sampler2D tex2;


void main() {
    /* ****************** 주변광(기본 광원) 계산 ****************** */
    vec3 ambient = ambientStrength * lightColor;
    /* ******************************************************* */


    /* ************** 광원이 물체에 반사되는 광량 계산 *************** */
    vec3 lightDir = normalize(lightPos - position);

    // openGL program -> vertex shader -> 에서 넘어온 normal을 다시 normalize 하는 이유
    // vertex shader에서 계산된 normal은 rasterization과정에서 선형보간이 진행됨
    // 단위벡터간의 선형보간 결과가 단위벡터임을 보장하지 못하기때문에 다시 normalize과정을 fragment shader과정에 진행
    vec3 pixelNorm = normalize(normal);

    vec3 diffuse = max(dot(pixelNorm, lightDir), 0.0) * lightColor; // ignore minus val of dot product
    /* ******************************************************* */


    /* ********* 광원이 물체에 반사되어 카메라에 비치는 광량 계산 ******** */
    vec3 viewDir = normalize(viewPos - position);

    vec3 reflectDir = reflect(-lightDir, pixelNorm);

    // 물체에 반사광이 표시되는 영역의 크기를 조절 -> specularShininess
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularShininess );

    vec3 specular = specularStrength * spec * lightColor;
    /* ******************************************************* */


    vec3 result = (ambient + diffuse + specular) * objectColor;

    fragColor = vec4(result, 1.0);
    //fragColor = texture(tex, texCoord)*0.8 + texture(tex2, texCoord) * 0.2;
}