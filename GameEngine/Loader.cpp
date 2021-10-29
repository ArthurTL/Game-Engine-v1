#include "Loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

RawModel* Loader::loadToVAO(vector<float> positions, vector<float> textureCoords, vector<float> normals,vector<unsigned int> indices) {
	GLuint vaoID = Loader::createVAO();
	storeDataInAttributeList(0,3,positions);
	storeDataInAttributeList(1,2,textureCoords);
	storeDataInAttributeList(2, 3, normals);
	bindIndicesBuffer(indices);
	unbindVAO();
	return new RawModel(vaoID, indices.size());
}

RawModel* Loader::loadToVAO(vector<float> positions, vector<float> textureCoords, vector<float> normals, vector<float> tangents, vector<unsigned int> indices) {
	GLuint vaoID = Loader::createVAO();
	storeDataInAttributeList(0, 3, positions);
	storeDataInAttributeList(1, 2, textureCoords);
	storeDataInAttributeList(2, 3, normals);
	storeDataInAttributeList(3, 3, tangents);
	bindIndicesBuffer(indices);
	unbindVAO();
	return new RawModel(vaoID, indices.size());
}

RawModel* Loader::loadToVAO(vector<float> positions, int dimensions) {
	GLuint vaoID = createVAO();
	storeDataInAttributeList(0, dimensions, positions);
	unbindVAO();
	return new RawModel(vaoID, positions.size() /(size_t)dimensions);
}

GLuint Loader::createVAO() {
	GLuint vaoID = 0;
	glGenVertexArrays(1, &vaoID);
	vaos.push_back(vaoID);
	glBindVertexArray(vaoID);
	return vaoID;
}

void Loader::storeDataInAttributeList(GLuint attributeNumber, GLuint coordinateSize, vector<float> data) {
	GLuint vboID = 0;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	vbos.push_back(vboID);
	size_t vertexBufferSize = sizeof(float) * data.size();
	glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}	

void Loader::unbindVAO() {
	glBindVertexArray(0);
}

void Loader::bindIndicesBuffer(vector<unsigned int> indices) {
	GLuint vboID = 0;
	glGenBuffers(1, &vboID);
	vbos.push_back(vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	size_t indexBufferSize = sizeof(unsigned int) * indices.size();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indexBufferSize, indices.data(),GL_STATIC_DRAW);
	
}

GLuint Loader::loadTexture(const string& fileName) {
	GLuint texID;
	int width, height, nrChannels;
	const GLchar* file = (const GLchar*)fileName.c_str();
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "failed to load texture" << std::endl;
	}


	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	textures.push_back(texID);
	return texID;
}
GLuint Loader::loadCubeMap(vector<string> textureFiles)
{
	GLuint texID;
    glGenTextures(1,&texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
	glActiveTexture(GL_TEXTURE0);
	int width, height, nrChannels;
	for (int i = 0; i < textureFiles.size(); i++) {
		const GLchar* file = (const GLchar*)textureFiles[i].c_str();
		unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
			stbi_image_free(data);
		}
		else {
			std::cout << "failed to load texture" << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	textures.push_back(texID);
	return texID;
}
void Loader::cleanUp() {
	for (GLuint vao : vaos) {
		glDeleteVertexArrays(1, &vao);
	}
	for (GLuint vbo : vbos) {
		glDeleteBuffers(1, &vbo);
	}
	for (GLuint texture : textures) {
		glDeleteTextures(1,&texture);
	}
}
