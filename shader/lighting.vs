#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 transform;

// local coordi 를 world coordi 로 변환하는 행렬
// 조명은 world coordi 어딘가에서 입사됨
uniform mat4 modelTransform;

out vec3 normal;
out vec2 texCoord;
out vec3 position;

void main() {
    
    gl_Position = transform * vec4(aPos, 1.0);

    //normal = aNormal;
    normal = (transpose(inverse(modelTransform)) * vec4(aNormal, 0.0)).xyz;

    texCoord    = aTexCoord;

    // diffuse value를 계산하려면 world space상에서의 좌표값이 필요
    position = (modelTransform * vec4(aPos, 1.0)).xyz;
}