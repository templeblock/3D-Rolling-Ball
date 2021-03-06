#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec2 textCoords;
layout(location=2) in vec3 normal;

out vec3 Normal;
out vec3 FragPos;
out vec2 TextCoords;

out float frag;

out vec2 worldxz;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 lightSpaceMatrix;
out vec4 FragPosLightSpace;

uniform vec4 clipPlane;

void main()
{
	vec4 worldPosition = model * vec4(position, 1.0f);

	gl_Position = projection * view * worldPosition;

	gl_ClipDistance[0] = dot(model * vec4(position, 1), clipPlane);

	FragPos = vec3(model * vec4(position, 1.0f));
	Normal = mat3(transpose(inverse(model))) * normal;

	TextCoords = textCoords;

	frag = (position.y + 50) / 100.0;
	// frag2 = 1 - frag1;
	worldxz = worldPosition.xz;

    FragPosLightSpace = lightSpaceMatrix * worldPosition;
}
