#include "OBJLoader.h"

vector<float> vertices_ = {
		-0.5f,0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f,0.5f,-0.5f,

		-0.5f,0.5f,0.5f,
		-0.5f,-0.5f,0.5f,
		0.5f,-0.5f,0.5f,
		0.5f,0.5f,0.5f,

		0.5f,0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,0.5f,
		0.5f,0.5f,0.5f,

		-0.5f,0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		-0.5f,0.5f,0.5f,

		-0.5f,0.5f,0.5f,
		-0.5f,0.5f,-0.5f,
		0.5f,0.5f,-0.5f,
		0.5f,0.5f,0.5f,

		-0.5f,-0.5f,0.5f,
		-0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,0.5f

};
vector<float> normals_ = {
	-1.0000, -1.0000,  1.0000,
	 1.0000, -1.0000,  1.0000,
	-1.0000,  1.0000,  1.0000,
	-1.0000,  1.0000,  1.0000,
	 1.0000, -1.0000,  1.0000,
	 1.0000,  1.0000,  1.0000,
	 -1.0000, -1.0000,  1.0000,
	 1.0000, -1.0000,  1.0000,
	-1.0000,  1.0000,  1.0000,
	-1.0000,  1.0000,  1.0000,
	 1.0000, -1.0000,  1.0000,
	 1.0000,  1.0000,  1.0000
};
vector<float> textures_ = {

		0,0,
		0,1,
		1,1,
		1,0,
		0,0,
		0,1,
		1,1,
		1,0,
		0,0,
		0,1,
		1,1,
		1,0,
		0,0,
		0,1,
		1,1,
		1,0,
		0,0,
		0,1,
		1,1,
		1,0,
		0,0,
		0,1,
		1,1,
		1,0


};
vector<unsigned int> indices_ = {
		0,1,3,
		3,1,2,
		4,5,7,
		7,5,6,
		8,9,11,
		11,9,10,
		12,13,15,
		15,13,14,
		16,17,19,
		19,17,18,
		20,21,23,
		23,21,22

};

RawModel* OBJLoader::loadObjModel(const string& fileName, Loader loader, bool useNormalMap) {

	vector<glm::vec3> vertices;
	vector<glm::vec2> textures;
	vector<glm::vec3> normals;
	vector<glm::vec3> tangents(3000, glm::vec3(0.0));
	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	vector<unsigned int> indicesArray;
	vector<float> verticesArray;
	vector<float> texturesArray;
	vector<float> normalsArray;
	vector<float> tangentsArray;

	const GLchar* fileName_ = (const GLchar*)fileName.c_str();
	FILE* file = fopen(fileName_, "r");
	if (file == NULL) {
		std::cout << "Could not find obj file" << std::endl;
		return loader.loadToVAO(vertices_, textures_, normals_,indices_);
	}
	while (1) {

		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y =1 -uv.y; 
			textures.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
		
			normals.push_back(normal);
		}

		else if (strcmp(lineHeader, "f") == 0) {
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				std::cout << "File can't be read by this parser" << std::endl;
				std::fclose(file);
				return loader.loadToVAO(vertices_, textures_, normals_, indices_);
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
			if (useNormalMap) {
				glm::vec3 tangent1;

				glm::vec3 edge1 = vertices[vertexIndex[1] - 1] - vertices[vertexIndex[0] - 1];
				glm::vec3 edge2 = vertices[vertexIndex[2] - 1] - vertices[vertexIndex[0] - 1];
				glm::vec2 deltaUV1 = textures[uvIndex[1] - 1] - textures[uvIndex[0] - 1];
				glm::vec2 deltaUV2 = textures[uvIndex[2] - 1] - textures[uvIndex[0] - 1];

				float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

				tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
				tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
				tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
				tangents[vertexIndex[0] - 1] = tangents[vertexIndex[0] - 1]+tangent1;
				tangents[vertexIndex[1] - 1]= tangents[vertexIndex[1] - 1]+tangent1;
				tangents[vertexIndex[2] - 1] = tangents[vertexIndex[2] - 1]+ tangent1;
			}
		}
		else {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}if(useNormalMap){
		for (glm::vec3 &tangent : tangents) {
			tangent = glm::normalize(tangent);
		}
	}
	
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = vertices[vertexIndex - 1];
		glm::vec2 uv = textures[uvIndex - 1];
		glm::vec3 normal = normals[normalIndex - 1];
	
		// Put the attributes in buffers
		//std::cout << vertexIndex - 1 << std::endl;
		indicesArray.push_back(i);
		verticesArray.push_back(vertex.x);
		verticesArray.push_back(vertex.y);
		verticesArray.push_back(vertex.z);
		texturesArray.push_back(uv.x);
		texturesArray.push_back(uv.y);
		normalsArray.push_back(normal.x);
		normalsArray.push_back(normal.y);
		normalsArray.push_back(normal.z);
		if (useNormalMap) {
			glm::vec3 tangent = tangents[vertexIndex-1];
			tangentsArray.push_back(tangent.x);
			tangentsArray.push_back(tangent.y);
			tangentsArray.push_back(tangent.z);
		}
	}
	std::fclose(file);
	if (useNormalMap) {
		return loader.loadToVAO(verticesArray, texturesArray, normalsArray,tangentsArray, indicesArray);
	}
	return loader.loadToVAO(verticesArray,texturesArray,normalsArray,indicesArray);
};

