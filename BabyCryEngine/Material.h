#ifndef MATERIAL_H
#define MATERIAL_H


class Material : public Resource< Material >
{
public:

  Material( char *name, char *path = "./");
  virtual ~Material();

  IDirect3DTexture9 *GetTexture();
  D3DMATERIAL9 *GetLighting();
  unsigned long GetWidth();
  unsigned long GetHeight();
  bool GetIgnoreFace();
  bool GetIgnoreFog();
  bool GetIgnorRay();

private:

	IDirect3DTexture9 *m_texture;
	D3DMATERIAL9 m_lighting;
	unsigned long m_width;
	unsigned long m_height;
	bool m_ignoreFace; // Indicates if face with this material should be ignored.
	bool m_ignoreFog;  // Indicates this material should be ignored when rendering fog.
	bool m_ignoreRay;  // Indicates ray intersection test should be ignored for this ray.
};

#endif