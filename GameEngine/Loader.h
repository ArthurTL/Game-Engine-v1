#ifndef __LOADER_H__
#define __LOADER_H__


#include "RawModel.h"
#include <vector>
#include <iostream>
#include <string>


using namespace std;

class Loader {

private:
	vector<GLuint> vaos;
	vector<GLuint> vbos;
	vector<GLuint> textures;

	GLuint createVAO();
	void storeDataInAttributeList(GLuint, GLuint, vector<float>);
	void bindIndicesBuffer(vector<GLuint>);
	void unbindVAO();
	
	

public:
	RawModel* loadToVAO(vector<float>,vector<float>,vector<float>, vector<unsigned int>);
	RawModel* loadToVAO(vector<float>, vector<float>, vector<float>, vector<float>, vector<unsigned int>);
	RawModel* loadToVAO(vector<float> positions, int dimensions);
	GLuint loadTexture(const string& fileName);
	GLuint loadCubeMap(vector<string> textureFiles);
	void cleanUp();


};
#endif

