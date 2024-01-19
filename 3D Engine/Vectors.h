#ifndef VECTORS_H
#define VECTORS_H

template<typename vectorType>
struct Vector2
{
	vectorType x;
	vectorType y;

	Vector2() : x(vectorType()), y(vectorType()) {}

	Vector2(vectorType _x, vectorType _y) : x(_x), y(_y) {}
};

template<typename T>
Vector2(T, T) -> Vector2<T>;

template<typename vectorType>
struct Vector3
{
	vectorType x;
	vectorType y;
	vectorType z;

	Vector3() : x(vectorType()), y(vectorType()), z(vectorType()) {}

	Vector3(vectorType _x, vectorType _y, vectorType _z) : x(_x), y(_y), z(_z) {}
};

template<typename T>
Vector3(T, T, T) -> Vector3<T>;


template<typename vectorType>
struct Vector4
{
	vectorType x;
	vectorType y;
	vectorType z;
	vectorType w;

	Vector4() : x(vectorType()), y(vectorType()), z(vectorType()), w(vectorType()) {}

	Vector4(vectorType _x, vectorType _y, vectorType _z, vectorType _w) : x(_x), y(_y), z(_z), w(_w) {}
};

template<typename T>
Vector4(T, T, T, T) -> Vector4<T>;

#endif // !VECTORS_H