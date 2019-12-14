#ifndef Vector_h__
#define Vector_h__

#include <vector>

struct Vector
{
	Vector() : x(0.f), y(0.f) {}
	Vector(float x, float y) : x(x), y(y) {}
  float x;
  float y;
};

typedef std::vector<Vector> VectorList;

#endif // Vector_h__
