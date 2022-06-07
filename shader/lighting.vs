#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 transform;

// local coordi 를 world coordi 로 변환하는 행렬
// 조명은 world coordi 어딘가에서 입사됨
// diffuse value 를 계산할려면 world space상의 좌표값이 필요
// 법선벡터도 같은이유로 model transform만 적용 
uniform mat4 modelTransform;
out vec3 normal;

out vec2 texCoord;
out vec3 position;

void main() {
    
    gl_Position = transform * vec4(aPos, 1.0);

    // normal = aNormal;
    // 보통의 경우 inverse Transform model 변환을 glm등으로 계산하여 쉐이더에 적용함
    // 아래의 코드에서 (transpose(inverse(modelTransform)) 에 해당함
    normal = (transpose(inverse(modelTransform)) * vec4(aNormal, 0.0)).xyz;

    texCoord    = aTexCoord;

    // diffuse value를 계산하려면 world space상에서의 좌표값이 필요
    position = (modelTransform * vec4(aPos, 1.0)).xyz;
}