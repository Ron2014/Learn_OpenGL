///////////////////////////////////////////////////////////////////////////////
//
// a simple vector3d class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <cmath>

class Vector3D;

inline float vectorMag(const Vector3D &v);
inline Vector3D crossProduct(const Vector3D &a, const Vector3D &b);
inline Vector3D operator *(float k, const Vector3D &v);
inline float distance(const Vector3D &a, const Vector3D &b);

class Vector3D {
public:
  float x, y, z;
  Vector3D ():x(0.0f),y(0.0f),z(0.0f) {}
  Vector3D(const Vector3D &o):x(o.x), y(o.y), z(o.z) {}
  Vector3D(float nx, float ny, float nz):x(nx), y(ny), z(nz) {}

  Vector3D &operator =(const Vector3D &o) {
    x=o.x; y=o.y; z=o.z;
    return *this;
  }

  bool operator ==(const Vector3D &o) {
    return x==o.x && y==o.y && z==o.z;
  }

  bool operator !=(const Vector3D &o) {
    return x!=o.x || y!=o.y || z!=o.z;
  }

  void zero() { x = y = z = 0.0f; }

  Vector3D operator -() const {
    return Vector3D(-x, -y, -z);
  }

  Vector3D operator +(const Vector3D &o) const {
    return Vector3D(x+o.x, y+o.y, z+o.z);
  }

  Vector3D &operator +=(const Vector3D &o) {
    x+=o.x; y+=o.y; z+=o.z;
    return *this;
  }

  Vector3D operator -(const Vector3D &o) const {
    return Vector3D(x-o.x, y-o.y, z-o.z);
  }

  Vector3D &operator -=(const Vector3D &o) {
    x-=o.x; y-=o.y; z-=o.z;
    return *this;
  }

  float operator *(const Vector3D &o) const {
    return x*o.x + y*o.y + z*o.z;
  }

  Vector3D operator *(float v) const {
    return Vector3D(x*v, y*v, z*v);
  }

  Vector3D &operator *=(float v) {
    x*=v; y*=v; z*=v;
    return *this;
  }

  Vector3D operator /(float v) const {
    float oneOverA = 1/v;
    return Vector3D(x*oneOverA, y*oneOverA, z*oneOverA);
  }

  Vector3D &operator /=(float v) {
    float oneOverA = 1/v;
    x*=oneOverA; y*=oneOverA; z*=oneOverA;
    return *this;
  }

  float magnitude() {
    return vectorMag(*this);
  }

  void normalize() {
    float mag = magnitude();
    if (mag > 0.0f) {
      float oneOverA = 1/mag;
      x*=oneOverA;
      y*=oneOverA;
      z*=oneOverA;
    }
  }
};

inline float vectorMag(const Vector3D &v) {
  return sqrt(v.x*v.x, v.y*v.y, v.z*v.z);
}

inline Vector3D crossProduct(const Vector3D &a, const Vector3D &b) {
  return Vector3D(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

inline Vector3D operator *(float k, const Vector3D &v) {
  return Vector3D(v.x*k, v.y*k, v.z*k);
}

inline float distance(const Vector3D &a, const Vector3D &b) {
  Vector3D direction = b-a;
  return direction.magnitude();
}

extern const Vector3D kZeroVector;