#version 330 core

in vec3 normal;
in vec3 position;
in vec2 texCoord;
out vec4 fragColor;


uniform vec3 viewPos;           // 보고 있는 눈(카메라)의 위치


struct Light {
    // directional light model
    vec3 direction;
    // spot light with attenuation model
    vec3 position;
    vec3 attenuation;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;


struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;




void main() {
    /* ****************** 주변광(기본 광원) 계산 ****************** */

    vec3 texColor = texture(material.diffuse, texCoord).xyz;
    vec3 ambient = texColor * light.ambient;
    /* ******************************************************* */


    /* ************** 광원이 물체에 반사되는 광량 계산 *************** */

    // Point 조명 (감쇄적용)
    float dist = length(light.position - position);
    vec3 distPoly = vec3(1.0, dist, dist*dist);
    float attenuation = 1.0 / dot(distPoly, light.attenuation);
    vec3 lightDir = (light.position - position) / dist;

    // Directional Light
    //vec3 lightDir = normalize(-light.direction);

    // openGL program -> vertex shader -> 에서 넘어온 normal을 다시 normalize 하는 이유
    // vertex shader에서 계산된 normal은 rasterization과정에서 선형보간이 진행됨
    // 단위벡터간의 선형보간 결과가 단위벡터임을 보장하지 못하기때문에 다시 normalize과정을 fragment shader과정에 진행
    vec3 pixelNorm = normalize(normal);

    float diff = max(dot(pixelNorm, lightDir), 0.0);

    vec3 diffuse = diff * texColor * light.diffuse;
    /* ******************************************************* */


    /* ********* 광원이 물체에 반사되어 카메라에 비치는 광량 계산 ******** */
    vec3 specColor = texture(material.specular, texCoord).xyz;  

    vec3 viewDir = normalize(viewPos - position);

    vec3 reflectDir = reflect(-lightDir, pixelNorm);

    // 물체에 반사광이 표시되는 영역의 크기를 조절 -> specularShininess
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess );

    //vec3 specular = spec * material.specular * light.specular;
    vec3 specular = spec * specColor * light.specular;
    /* ******************************************************* */

    // directional light model
    //vec3 result = ambient + diffuse + specular;

    // Point light model with attenuation
    vec3 result = (ambient + diffuse + specular) * attenuation;

    fragColor = vec4(result, 1.0);
}