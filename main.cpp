#include <vector>
#include <cmath>

#include "tgaimage.h"
#include "geometry.h"
#include "model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
const int width = 800;
const int height = 800;

Model* model = NULL;

void line(Vec2i p0, Vec2i p1, TGAImage& image, TGAColor color)
{
	bool steep = false;
	if (std::abs(p0.x - p1.x) < std::abs(p0.y - p1.y))
	{
		std::swap(p0.x, p0.y);
		std::swap(p1.x, p1.y);
		steep = true;
	}
	if (p0.x > p1.x)
		std::swap(p0, p1);

	int dx = p1.x - p0.x;
	int dy = p1.y - p0.y;
	int derror = std::abs(dy) * 2;
	float error = 0;
	int y = p0.y;
	for (float x = p0.x; x <= p1.x; x++)
	{
		if (steep)
			image.set(y, x, color);
		else
			image.set(x, y, color);

		error += derror;
		if (error > dx)
		{
			y += (p1.y > p0.y ? 1 : -1);
			error -= dx * 2;
		}
	}
}

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	for (int x = x0; x <= x1; x++) {
		float t = (x - x0) / (float)(x1 - x0);
		int y = y0 * (1. - t) + y1 * t;
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
	}
}

Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P)
{
	Vec3f s[2];
	for (int i = 2; i--;)
	{
		s[i][0] = C[i] - A[i];
		s[i][1] = B[i] - A[i];
		s[i][2] = A[i] - P[i];
	}
	Vec3f u = cross(s[0], s[1]);
	if (std::abs(u[2]) > 1e-2)
		return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	return Vec3f(-1, 1, 1);
}

void triangle(Vec3f* pts, float* zbuffer, TGAImage& image, TGAColor color)
{
	Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	Vec2f clamp(image.get_width() - 1, image.get_height() - 1);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 2; j++)
		{
			bboxmin[j] = std::max(0.f, std::min(bboxmin[j], pts[i][j]));
			bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
		}

	Vec3f P;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
		{
			Vec3f bc = barycentric(pts[0], pts[1], pts[2], P);
			if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;
			P.z = 0;
			for (int i = 0; i < 3; i++)
				P.z += pts[i][2] * bc[i];
			if (zbuffer[int(P.x + P.y * width)] < P.z)
			{
				zbuffer[int(P.x + P.y * width)] = P.z;
				image.set(P.x, P.y, color);
			}
		}
}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color) {
	if (t0.y == t1.y && t0.y == t2.y) return;

	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);

	int total_height = t2.y - t0.y;
	for (int i = 0; i < total_height; i++)
	{
		bool second_half = i > t1.y - t0.y || t1.y == t0.y;
		int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;

		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height;

		Vec2i A = t0 + (t2 - t0) * alpha;
		Vec2i B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;

		if (A.x > B.x) std::swap(A, B);
		for (int j = A.x; j <= B.x; j++)
			image.set(j, t0.y + i, color);
	}
}

Vec3f world2screen(Vec3f v)
{
	return Vec3f(int((v.x + 1.) * width / 2. + .5), int((v.y + 1.) * height / 2. + .5), v.z);
}

int main(int argc, char** argv)
{
	if (2 == argc) {
		model = new Model(argv[1]);
	}
	else {
		model = new Model("african_head.obj");
	}

	Vec3f light_dir(0,0,-1);
	float* zbuffer = new float[width * height];
	for (int i = width * height; i--; zbuffer[i] = -std::numeric_limits<float>::max());

	TGAImage image(width, height, TGAImage::RGB);
	for (int i = 0; i < model->nfaces(); i++)
	{
		std::vector<int> face = model->face(i);

		Vec3f pts[3];
		for (int i = 0; i < 3; i++) pts[i] = world2screen(model->vert(face[i]));

		Vec3f wc[3];
		for (int i = 0; i < 3; i++) wc[i] = model->vert(face[i]);

		Vec3f n = cross(wc[2]-wc[0], wc[1] - wc[0]);
		n.normalize();

		float intensity = n * light_dir;

		triangle(pts, zbuffer, image, TGAColor(255 * intensity, 255 * intensity, 255 * intensity, 255));
		//triangle(pts, zbuffer, image, TGAColor(rand() % 255 * intensity, rand() % 255 * intensity, rand() % 255 * intensity, 255));
	}

	image.flip_vertically();
	image.write_tga_file("output.tga");

	delete model;

	return 0;
}