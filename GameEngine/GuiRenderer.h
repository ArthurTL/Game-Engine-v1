#ifndef __GUIRENDERER_H__
#define __GUIRENDERER_H_
#include "Loader.h"
#include "GuiTexture.h"
#include "GuiShader.h"
class GuiRenderer {

private:
	const RawModel* quad;
	GuiShader* shader;
public:

	GuiRenderer(Loader loader);

	void render(std::vector<GuiTexture> guis);

	void cleanUp();
};




#endif