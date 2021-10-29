#version 400 core

in vec3 position;
out vec3 textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main(void){
	gl_ClipDistance[0]=1;
	vec4 pos= projectionMatrix *viewMatrix* vec4(position, 1.0); 
	gl_Position =pos.xyww;
	textureCoords = position;
	
}