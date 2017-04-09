#include "Engine.h"

Material ::Material(char *name, char *path): Resource< Material >(name, path)
{
	D3DXIMAGE_INFO info;
	Script *script = new Script(name, path);
	wchar_t strTexture[4096];
	
	wsprintf( strTexture, L"%s", script->GetStringData("texture"));
	std::wstring strTexture();

	if(script->GetColorData("transparency")->a == 0.0f)
	{
		D3DXCreateTextureFromFileEx(g_engine->GetDevice(),strTexture , D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE, D3DX_FILTER_TRIANGLE , 0, &info, NULL, &m_texture);

	}
	else
	{
		D3DCOLORVALUE *colour = script->GetColourData( "transparency" );
		D3DCOLOR transparency = D3DCOLOR_COLORVALUE( colour->r, colour->g, colour->b, colour->a );
		D3DXCreateTextureFromFileEx( g_engine->GetDevice(), strTexture, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE, D3DX_FILTER_TRIANGLE, transparency, &info, NULL, &m_texture );
	}

	m_width = info.Width;
	m_height = info.Height;

	m_lighting.Diffuse = *script->GetColourData( "diffuse" );
	m_lighting.Ambient = *script->GetColourData( "ambient" );
	m_lighting.Specular = *script->GetColourData( "specular" );
	m_lighting.Emissive = *script->GetColourData( "emissive" );
	m_lighting.Power = *script->GetFloatData( "power" );
		
	m_ignoreFace = *script->GetBoolData( "ignore_face" );
		
	m_ignoreFog = *script->GetBoolData( "ignore_fog" );
		
	m_ignoreRay = *script->GetBoolData( "ignore_ray" );
		
	SAFE_DELETE( script );
}

Material::~Material()
{
	SAFE_RELEASE(m_texture);
}

IDirect3DTexture9 *Material::GetTexture()
{
	return m_texture;
}

D3DMATERIAL9 *Material::GetLighting()
{
	return &m_lighting;
}

unsigned long Material::GetWidth()
{
	return m_width;
}

unsigned long Material::GetHeight()
{
	return m_height;
}

bool Material::GetIgnoreFace()
{
	return m_ignoreFace;
}

bool Material::GetIgnoreFog()
{
	return m_ignoreFog;
}

bool Material::GetIgnorRay()
{
	return m_ignoreRay;
}
