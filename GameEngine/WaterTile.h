#ifndef __WATERTILE_H__
#define __WATERTILE_H__


class WaterTile {

private:	
	 float height;
	 float x, z;


public:
    static const int TILE_SIZE = 70;
	WaterTile(float centerX, float centerZ, float height_): x(centerX), z(centerZ), height(height_) {}
	
	inline float getHeight()const {return height;}

	inline float getX() const{return x;}

    inline float getZ() const{return z;}
};



#endif