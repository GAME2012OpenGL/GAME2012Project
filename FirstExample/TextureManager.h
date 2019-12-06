#ifndef __TEXTUREMANAGER__
#define __TEXTUREMANAGER__

#include <map>
#include "GL/glew.h"

class TextureManager
{
private :
	static std::map<const char*, GLuint> m_mapTextures;
public :
	TextureManager() {}
	~TextureManager() {}

	static void CreateTexture(const char* filePath, const char* textureKey, int force_channels);
	static GLuint GetTexture(const char* textureKey);

	static void DestroyTexture();
};

#endif