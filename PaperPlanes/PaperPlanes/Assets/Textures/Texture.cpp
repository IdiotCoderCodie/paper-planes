#include "Texture.h"
#include "../../d3d_safe_release.h"

Texture::Texture(D3D& d3d, WCHAR* filename)
{
	DirectX::CreateDDSTextureFromFile(&d3d.GetDevice(), filename, nullptr, &m_texture);
}


Texture::~Texture(void)
{
	d3d_safe_release(m_texture);
}
