#pragma once
#include <Ogre.h>

using namespace Ogre;
class OgreTextureHandler
{
private :
	TexturePtr& _texture;
	uint32 *_data;
	size_t _height;
	size_t _width;
	size_t _pitch;
	PixelBox &_pb;

public:
	void setPixel(size_t x, size_t y, uint32 color);
	void drawRect(size_t x, size_t y, size_t width, size_t height, uint32 color);
	void updateTexture();
	void updateLock(bool discard = false);
	OgreTextureHandler(Ogre::TexturePtr& texture, uint32 defaultColor = 0x000000);
	~OgreTextureHandler(void);
};

