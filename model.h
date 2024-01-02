#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"
#include "tgaimage.h"

class Model
{
public:
	Model(const char *filename);
	~Model();

	int nverts();
	int nfaces();
	Vec3f normal(int iface, int nthvert);
	Vec3f normal(Vec2f uv);
	Vec3f vert(int i);
	Vec3f vert(int iface, int nthvert);
	Vec2f uv(int iface,int nthvert);
	TGAColor diffuse(Vec2f uv);
	float specular(Vec2f uv);
	std::vector<int> face(int idx);
private:
	std::vector<Vec3f> verts_;
	std::vector<std::vector<Vec3i>> faces_;
	std::vector<Vec3f> norms_;
	std::vector<Vec2f> uv_;
	TGAImage diffusemap_;
	TGAImage normalmap_;
	TGAImage specularmap_;
	void load_texture(std::string filename, const char* suffix, TGAImage& img);
};

#endif // !__MODEL_H__
