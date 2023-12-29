#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <iostream>
#include <cmath>

template <class T>
struct Vec2
{
	union
	{
		struct { T u, v; };
		struct { T x, y; };
		T raw[2];
	};
	Vec2() :u(0), v(0) {}
	Vec2(T _u, T _v) :u(_u), v(_v) {}
	inline Vec2<T> operator+(const Vec2<T>& vec2) const { return Vec2<T>(u + vec2.u, v + vec2.v); }
	inline Vec2<T> operator-(const Vec2<T>& vec2) const { return Vec2<T>(u - vec2.u, v - vec2.v); }
	inline Vec2<T> operator*(float f)			  const { return Vec2<T>(u * f, v * f); }
	template <class> friend std::ostream& operator<<(std::ostream& s, Vec2<T>& v);
};

template <class T>
struct Vec3
{
	union
	{
		struct { T x, y, z; };
		struct { T ivert, iuv, inorm; };
		T raw[3];
	};
	Vec3() :x(0), y(0), z(0) {}
	Vec3(T _x, T _y, T _z) :x(_x), y(_y), z(_z) {}
	inline Vec3<T> operator^(const Vec3<T>& vec3) const { return Vec3<T>(y * vec3.z - z * vec3.y, z * vec3.x - x * vec3.z, x * vec3.y - y * vec3.x); }
	inline Vec3<T> operator+(const Vec3<T>& vec3) const { return Vec3<T>(x + vec3.x, y + vec3.v, z + vec3.z); }
	inline Vec3<T> operator-(const Vec3<T>& vec3) const { return Vec3<T>(x - vec3.x, y - vec3.y, z - vec3.z); }
	inline Vec3<T> operator*(float f)			  const { return Vec3<T>(x * f, y * f, z * f); }
	inline T	   operator*(const Vec3<T>& vec3) const { return x * vec3.x + y * vec3.y + z * vec3.z; }
	float norm() const { return std::sqrt(x * x + y * y + z * z); }
	Vec3<T> normalize(T l = 1) { *this = (*this) * (l / norm()); return *this; }
	template <class> friend std::ostream& operator<<(std::ostream& s, Vec3<T>& v);
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>	Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>	Vec3i;

template <class T>
std::ostream& operator<<(std::ostream& s, Vec2<T>& v)
{
	s << "(" << v.x << "," << v.y << ")\n";
	return s;
}

template <class T>
std::ostream& operator<<(std::ostream& s, Vec3<T>& v)
{
	s << "(" << v.x << "," << v.y << ")\n";
	return s;
}

#endif // !__GEOMETRY_H__
