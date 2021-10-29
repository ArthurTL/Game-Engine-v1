
#ifndef __OBJLOADER_H__
#define __OBJLOADER_H__


#include "Loader.h"



#include <fstream>
#include <stdlib.h>
#include <glm.hpp>


class OBJLoader {
public:
	static RawModel* loadObjModel(const string& fileName, Loader loader, bool useNormalMap = false);
	
};
#endif