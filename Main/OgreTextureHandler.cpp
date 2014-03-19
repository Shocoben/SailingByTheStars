#include "OgreTextureHandler.h"


OgreTextureHandler::OgreTextureHandler(Ogre::TexturePtr& texture, uint32 defaultColor) : _texture(texture), _data(NULL), _pb(PixelBox(0,0,0,Ogre::PixelFormat::PF_X8R8G8B8))
{
	/// Lock the buffer so we can write to it
	_texture->getBuffer()->lock(HardwareBuffer::HBL_NORMAL);
	_pb = _texture->getBuffer()->getCurrentLock();
		/// Update the contents of pb here
	/// Image data starts at pb.data and has format pb.format
	/// Here we assume data.format is PF_X8R8G8B8 so we can address pixels as uint32.
	_data = static_cast<uint32*>(_pb.data);
	
	_height = _pb.getHeight();
	_width = _pb.getWidth();
	_pitch = _pb.rowPitch; // Skip between rows of image
	drawRect(0, 0, _width, _height, defaultColor);
	_texture->getBuffer()->unlock();
}

void OgreTextureHandler::setPixel(size_t x, size_t y, uint32 hexColor)
{
	_data[_pitch*y + x] = hexColor;
}

void OgreTextureHandler::drawRect(size_t startX, size_t startY, size_t width, size_t height, uint32 hexColor)
{
	for(size_t y= startY; y<height; ++y)
	{
		for(size_t x= startX; x<width; ++x)
		{
			// 0xRRGGBB
			setPixel(x, y, hexColor);
		}
	}
}

void OgreTextureHandler::updateLock(bool discard) //At the moment, when playing with locks, the data is stil accessible, but you may need this if you have somme issues.
{
	_texture->getBuffer()->lock(HardwareBuffer::HBL_NORMAL);
	_pb = _texture->getBuffer()->getCurrentLock();
	_data = static_cast<uint32*>(_pb.data);
}

void OgreTextureHandler::updateTexture()
{
	_texture->getBuffer()->lock(HardwareBuffer::HBL_NORMAL);
	_texture->getBuffer()->unlock();
}

OgreTextureHandler::~OgreTextureHandler(void)
{
}
