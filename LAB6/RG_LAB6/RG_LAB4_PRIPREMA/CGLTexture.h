#pragma once
class CGLTexture
{
protected:
	UINT m_ID;
public:
	CGLTexture();
	~CGLTexture();
	UINT GetTextureID();
	void SelectTexture();
	UINT LoadTextureFromFile(char* fileName, bool bRepeatTex = false);
	void DeleteTexture();
	static void PrepareTexturing(bool isLightnihEnabled = false);
};

