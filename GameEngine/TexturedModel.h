#ifndef __TEXTUREDMODEL_H__
#define __TEXTUREDMODEL_H__

#include "RawModel.h"
#include "ModelTexture.h"

class TexturedModel {

public:
	~TexturedModel() {};
	RawModel rawModel;
	ModelTexture texture;
	TexturedModel(RawModel model, ModelTexture texture_) : rawModel(model), texture(texture_) {};
	

};
#endif
