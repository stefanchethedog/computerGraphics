#include "pch.h"
#include "CGLTexture.h"

#include "DImage.h"

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")

#include <GL\gl.h>
#include <GL\glu.h>

CGLTexture::CGLTexture()
{
	m_ID = 0;
}

CGLTexture::~CGLTexture()
{}

UINT CGLTexture::GetTextureID()
{
	return m_ID;
}

void CGLTexture::SelectTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void CGLTexture::DeleteTexture()
{
	glDeleteTextures(1, &m_ID);
	m_ID = 0;
}

void CGLTexture::PrepareTexturing(bool isLightnihEnabled)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	if (isLightnihEnabled)
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	else
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

UINT CGLTexture::LoadTextureFromFile(char* fileName, bool bRepeatTex)
{
	//UINT texID;
	DImage img;
	img.Load(CString(fileName));
	PrepareTexturing();

	//glGenTextures(1, &texID);
	//glBindTexture(GL_TEXTURE_2D, texID);
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	if (bRepeatTex)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(),
		GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());
	
	/*glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.Width(), img.Height(), 0,
	GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());*/

	//return texID;
	return m_ID;
}
