#version 400 core
in vec4 clipSpace;
in vec2 textureCoords;
in vec3 toCameraVector;
in vec3 fromLightVector[5];

out vec4 out_Color;
uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform vec3 lightColor[5];
uniform vec3 attenuation[5];
uniform float moveFactor;

const float shineDamper=  40.;
const float reflectivity = 0.7;
const float waveStrength = 0.04;
void main(void) {
	
	vec2 refractTexCoords = (clipSpace.xy/clipSpace.w)/2.0 +0.5;
	vec2 reflectTexCoords = vec2(refractTexCoords.x,-refractTexCoords.y);
	
	float depth = texture(depthMap,refractTexCoords).r;
	float near = 0.1;
	float far = 1000.0;
	float floorDistance = 2.0*near*far/(far+near- (2.0*depth-1.0)*(far-near));

	depth = gl_FragCoord.z;
	float waterDistance = 2.0*near*far/(far+near- (2.0*depth-1.0)*(far-near));
	float waterDepth =floorDistance - waterDistance;

	vec2 distortedTexCoord = texture(dudvMap, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg*0.1;
	distortedTexCoord = textureCoords + vec2(distortedTexCoord.x, distortedTexCoord.y + moveFactor);
	vec2 totalDistortion =(texture(dudvMap, distortedTexCoord).rg *2.0 -1.0)*waveStrength*clamp(waterDepth/20.0,0.0,1.0);

	refractTexCoords+=totalDistortion;
	refractTexCoords = clamp(refractTexCoords,0.001,0.999);
	
	reflectTexCoords+=totalDistortion;
	reflectTexCoords.x =clamp(reflectTexCoords.x,0.001,0.999);
	reflectTexCoords.y =clamp(reflectTexCoords.y,-0.999,-0.001);
	
	vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
	vec4 refractColor = texture(refractionTexture,refractTexCoords);
	
	vec4 normalMapColor = texture(normalMap, distortedTexCoord);
	vec3 normal = vec3(normalMapColor.r*2.0-1.0, normalMapColor.b*3.0,normalMapColor.g*2.0-1.0);
	normal = normalize(normal);
	
	vec3 viewVector = normalize(toCameraVector);
	float refractiveFactor = dot(viewVector, normal);
	refractiveFactor = pow(refractiveFactor,1.);

	vec3 totalSpecular = vec3(0.0);
	for(int i=0;i<5;i++){
		float distance = length(fromLightVector[i]);
		float attFactor = attenuation[i].x+(attenuation[i].y*distance)+(attenuation[i].z*distance*distance);
		vec3 unitLightVec = normalize(fromLightVector[i]);
		vec3 reflectedLight = reflect(unitLightVec, normal);
		float specularFactor =dot(reflectedLight,viewVector);
		specularFactor = max(specularFactor,0.0);
		specularFactor = pow(specularFactor, shineDamper);
		totalSpecular = totalSpecular + (specularFactor * reflectivity* lightColor[i])/attFactor;
	}
	totalSpecular = totalSpecular*clamp(waterDepth/5.0,0.0,1.0);
	out_Color = mix(reflectColor,refractColor,refractiveFactor) + vec4(totalSpecular,0.0);
	out_Color.a = clamp(waterDepth/5.0,0.0,1.0);
}