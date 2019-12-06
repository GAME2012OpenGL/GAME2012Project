#include "TextureManager.h"
#include "SoilLib/SOIL.h"

std::map<const char*, GLuint> TextureManager::m_mapTextures;

void TextureManager::CreateTexture(const char* filePath, const char* textureKey, int force_channels)
{
	GLint width, height;
	unsigned char* image = SOIL_load_image(filePath, &width, &height, 0, force_channels);
	if (image == nullptr)
	{
		printf("Error: image not found\n");
	}
	//glActiveTexture(GL_TEXTURE0);
	GLuint texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	if (force_channels == SOIL_LOAD_RGB)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else if (force_channels == SOIL_LOAD_RGBA)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);


	m_mapTextures.insert(std::make_pair(textureKey, texture));
}


GLuint TextureManager::GetTexture(const char* textureKey)
{
	std::map<const char*, GLuint>::iterator iter = m_mapTextures.find(textureKey); // 이더레이터를 씀

	if (iter != m_mapTextures.end()) 
	{
		return iter->second;
	}
	else
	{
		printf("Error: image not found\n");
	}

	return 0;
}