#pragma once
#include <d3d11.h>
#include <DDSTextureLoader.h>
#include "../../D3D.h"

class Texture
{
public:
	Texture(D3D& d3d, WCHAR* filename);
	~Texture(void);

	// Returns a pointer to the ShaderResourceView of the texture.
	ID3D11ShaderResourceView* GetTexture() { return m_texture; }

private:
	ID3D11ShaderResourceView* m_texture;
};

