#include "Terrain.h"
const float Terrain::SIZE= 800.0;
const float Terrain::MAX_HEIGHT = 50.0;
const float Terrain::MAX_PIXEL_COLOR = 256.*256.*256.;

Terrain::Terrain(int gridX, int gridZ, Loader loader, TerrainTexturePack* texturePack_, TerrainTexture* blendMap_, const string& heightMap) {
	texturePack = texturePack_;
	blendMap = blendMap_;
	x = gridX * Terrain::SIZE;
	z = gridZ * Terrain::SIZE;
	model = generateTerrain(loader,heightMap);
	
}


RawModel* Terrain::generateTerrain(Loader loader, const string& heightMap_){
		
		int width, height, nrChannels;
		
		const GLchar* file = (const GLchar*)heightMap_.c_str();
		unsigned char* heightMap = stbi_load(file, &width, &height, &nrChannels, 0);
		
		const int VERTEX_COUNT = height;
		heights.reserve(VERTEX_COUNT);
		const size_t count = (float)VERTEX_COUNT * VERTEX_COUNT;
		vector<float> vertices(count * 3);
		vector<float> normals(count * 3);
		vector<float> textureCoords(count * 3);
		vector<GLuint> indices(6 * ((float)VERTEX_COUNT - 1) * (VERTEX_COUNT - 1));
		int vertexPointer = 0;
		for(int i=0;i<VERTEX_COUNT;i++){
			vector<float> hz;
			for(int j=0;j<VERTEX_COUNT;j++){
				vertices[vertexPointer*3] = (float)j/((float)VERTEX_COUNT - 1.) * SIZE;
				float vHeight = getHeight(j, i, heightMap, height, nrChannels);
				hz.push_back(vHeight);
				vertices[vertexPointer*3+1] = vHeight;
				vertices[vertexPointer*3+2] = (float)i/((float)VERTEX_COUNT - 1.) * SIZE;
				glm::vec3 normal = calculateNormal(j, i, heightMap, height, nrChannels);
				normals[vertexPointer*3] = normal.x;
				normals[vertexPointer*3+1] = normal.y;
				normals[vertexPointer*3+2] = normal.z;
				textureCoords[vertexPointer*2] = (float)j/((float)VERTEX_COUNT - 1.);
				textureCoords[vertexPointer*2+1] = (float)i/((float)VERTEX_COUNT - 1.);
				vertexPointer++;
			}
			heights.push_back(hz);
		}
		int pointer = 0;
		for(int gz=0;gz<VERTEX_COUNT-1;gz++){
			for(int gx=0;gx<VERTEX_COUNT-1;gx++){
				int topLeft = (gz*VERTEX_COUNT)+gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz+1)*VERTEX_COUNT)+gx;
				int bottomRight = bottomLeft + 1;
				indices[pointer++] = topLeft;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = topRight;
				indices[pointer++] = topRight;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = bottomRight;
			}
		}
		for (float v : vertices) {

		}
		
		RawModel* model = loader.loadToVAO(vertices, textureCoords, normals, indices);
		stbi_image_free(heightMap);
		
		return model;
	}

glm::vec3 Terrain::calculateNormal(int x, int y, unsigned char* heightMap, int height, int channelCount) {
	float heightL = getHeight(x - 1, y, heightMap, height, channelCount);
	float heightR = getHeight(x + 1, y, heightMap, height, channelCount);
	float heightD = getHeight(x, y - 1, heightMap, height, channelCount);
	float heightU = getHeight(x, y + 1, heightMap, height, channelCount);
	glm::vec3 normal = glm::vec3(heightL - heightR, SIZE/(float)(height-1), heightD - heightU);
	normal = glm::normalize(normal);
	return normal;
}

float Terrain::getHeight(int x, int y, unsigned char* heightMap,int height, int channelCount) {
	if (x < 0 || x >= height || y < 0 || y >= height) {
		return 0;
	}else{
		unsigned char* pixelOffset = heightMap + (x + height * y) * channelCount;
		float r = pixelOffset[0];
		float g = pixelOffset[1];
		float b = pixelOffset[2];
		float height = r * g * b;
		height -= MAX_PIXEL_COLOR / 8.0f;
		height /= MAX_PIXEL_COLOR / 2.0f;
		height *= MAX_HEIGHT;
		return height;
	}
}
float Terrain::getHeightOfTerrain(float worldX, float worldZ) {
	float terrainX = worldX - x;
	float terrainZ = worldZ - z;
	float gridSquareSize = SIZE / (float)(heights.size() - 1);
	int gridX = (int)floor(terrainX / gridSquareSize);
	int gridZ = (int)floor(terrainZ / gridSquareSize);
	if (gridX >= heights.size() - 1 || gridZ >= heights.size() - 1 || gridX < 0 || gridZ < 0) {
		return 0;
	}
	float xCoord = (fmod(terrainX,gridSquareSize))/gridSquareSize;
	float zCoord = (fmod(terrainZ,gridSquareSize))/gridSquareSize;
	float h;
	if (xCoord <= (1 - zCoord)) {
		h = Maths::barryCentric(glm::vec3(0, heights[gridZ][gridX], 0),
			glm::vec3(1, heights[gridZ][gridX+1], 0),
			glm::vec3(0, heights[gridZ+1][gridX], 1),
			glm::vec2(xCoord, zCoord));
	}
	else {
		h = Maths::barryCentric(glm::vec3(1, heights[gridZ][gridX+1], 0),
			glm::vec3(1, heights[gridZ + 1][gridX+1], 1),
			glm::vec3(0, heights[gridZ+1][gridX], 1),
			glm::vec2(xCoord, zCoord));
	}
	return h;
}

int Terrain::determineTerrain(float x, float z) {
	if (z<0.) {
		if (x > 0.)
			return 0;
		else return 1;
	}
	else {
		if (x > 0.)
			return 2;
		else return 3;
	}
}