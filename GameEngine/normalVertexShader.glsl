#version 400 core

in vec3 position;
in vec2 textureCoordinates;
in vec3 normal;
in vec3 tangent;

out vec2 ftextureCoords;
out vec3 toLightVector[5];
out vec3 toCameraVector;
out float visibility;
out vec4 shadowCoords;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPositionEyeSpace[5];
uniform mat4 toShadowMapSpace;

uniform vec2 offset;

const float density = 0.0025;
const float gradient = 1.5;
const float shadowDistance = 170.0;
const float transitionDistance = 15.0;
uniform vec4 plane;
void main(void){

	vec4 worldPosition = transformationMatrix * vec4(position,1.0);
	shadowCoords = toShadowMapSpace * worldPosition;
	shadowCoords = shadowCoords*0.5+0.5;
	mat4 modelViewMatrix = viewMatrix * transformationMatrix;
	vec4 positionRelativeToCam = modelViewMatrix * vec4(position,1.0);
	gl_Position = projectionMatrix * positionRelativeToCam;
	gl_ClipDistance[0] = dot(worldPosition,plane);
	ftextureCoords = textureCoordinates + offset;
	
	vec3 surfaceNormal = (modelViewMatrix * vec4(normal,0.0)).xyz;
	
	vec3 norm = normalize(surfaceNormal);
	vec3 tang = normalize((modelViewMatrix * vec4(tangent, 0.0)).xyz);
	vec3 bitang = normalize(cross(norm, tang));
	
	mat3 toTangentSpace = mat3(
		tang.x, bitang.x, norm.x,
		tang.y, bitang.y, norm.y,
		tang.z, bitang.z, norm.z
	);
	
	for(int i=0;i<5;i++){
		toLightVector[i] = toTangentSpace * (lightPositionEyeSpace[i] - positionRelativeToCam.xyz);
	}
	toCameraVector = toTangentSpace * (-positionRelativeToCam.xyz);
	
	float dist = length(positionRelativeToCam.xyz);
	visibility = exp(-pow((dist*density),gradient));
	visibility = clamp(visibility,0.0,1.0);
	
	dist = dist - (shadowDistance - transitionDistance);
	dist = dist/transitionDistance;
	shadowCoords.w = clamp(1.0-dist,0.0,1.0);
}