#ifndef __TTERRAINPACK_H__
#define __TTERRAINPACK_H__
#include "TerrainTexture.h"

class TerrainTexturePack {

private:
	const TerrainTexture backgroundTexture;
	const TerrainTexture rTexture;
	const TerrainTexture gTexture;
	const TerrainTexture bTexture;

public:
	TerrainTexturePack(TerrainTexture bg, TerrainTexture r, TerrainTexture g, TerrainTexture b) :
		backgroundTexture(bg), rTexture(r), gTexture(g), bTexture(b) {};
	
	inline TerrainTexture getBackgroundTexture() const { return backgroundTexture; };
	inline TerrainTexture getrTexture() const { return rTexture; };
	inline TerrainTexture getgTexture() const { return gTexture; };
	inline TerrainTexture getbTexture() const { return bTexture; };


};





#endif