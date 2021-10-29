#version 400 core

in vec2 ftextureCoords;
in vec3 surfaceNormal;
in vec3 toLightVector[5];
in vec3 toCameraVector;
in vec4 shadowCoords;
in float visibility;

out vec4 out_Color;

uniform sampler2D backgroundTexture;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;
uniform sampler2D blendMap;
uniform sampler2D shadowMap;

uniform vec3 lightColor[5];
uniform vec3 attenuation[5];
uniform float shineDamper;
uniform float reflectivity;
uniform float blendFactor;
uniform vec3 skyColor;

const int pcfCount=2;
const float totalTexels = (pcfCount*2.0+1)*(pcfCount*2.0+1);

void main(void){
	
	float mapSize = 4096.0;
	float texelSize = 1.0/mapSize;
	float total = 0.0;

	for (int x=-pcfCount; x<=pcfCount;x++){
		for (int y=-pcfCount; y<=pcfCount;y++){
			float objectNearestLight = texture(shadowMap, shadowCoords.xy + vec2(x,y)*texelSize).r;
			if(shadowCoords.z>objectNearestLight){
				total+=1.0;
			}
		}
	}
	total /= totalTexels;

	float lightFactor =1.0- 0.6*(total*shadowCoords.w);
	


	vec4 blendMapColor = texture(blendMap,ftextureCoords);

	float backTextureAmount = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
	vec2 tiledCoords =  ftextureCoords * 40.0;
	vec4 backgroundTextureColor = texture(backgroundTexture,tiledCoords) * backTextureAmount;

	vec4 rTextureColor = texture(rTexture,tiledCoords) * blendMapColor.r;
	vec4 gTextureColor = texture(gTexture,tiledCoords) * blendMapColor.g;
	vec4 bTextureColor = texture(bTexture,tiledCoords) * blendMapColor.b;
	
	vec4 totalColor = backgroundTextureColor + rTextureColor + gTextureColor + bTextureColor;
	

	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitVectorToCamera = normalize(toCameraVector);

	vec3 totalDiffuse = vec3(0.0);
	vec3 totalSpecular = vec3(0.0);

	for(int i =0; i<5;i++){
		//diffuse lighting
		vec3 unitLightVec = normalize(toLightVector[i]);
		float distance = length(toLightVector[i]);
		float attFactor = attenuation[i].x+(attenuation[i].y*distance)+(attenuation[i].z*distance*distance);
		float diffuseFactor = dot(unitNormal,unitLightVec);
		float brightness = max(diffuseFactor,0.0);
	
		//specular lighting
		vec3 lightDirection = -unitLightVec;
		vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
		float specularFactor =dot(reflectedLightDirection,unitVectorToCamera);
		specularFactor = max(specularFactor,0.0);
		float dampedFactor = pow(specularFactor, shineDamper);
	
		if(i==0){
		totalDiffuse = totalDiffuse + (brightness*lightFactor * lightColor[i])/attFactor;
		totalSpecular = totalSpecular + (dampedFactor *lightFactor* reflectivity* lightColor[i])/attFactor;
		}else{
		totalDiffuse = totalDiffuse + (brightness * lightColor[i])/attFactor;
		totalSpecular = totalSpecular + (dampedFactor * reflectivity* lightColor[i])/attFactor;
		}
	}
	totalDiffuse = max(totalDiffuse, 0.1);
	

	out_Color = vec4(totalDiffuse,1.0)*totalColor +vec4(totalSpecular,1.0);
	vec3 fogColor =mix(vec3(0.0,0.04,0.06),skyColor,blendFactor);
	out_Color = mix(vec4(fogColor,1.0),out_Color, visibility);

}