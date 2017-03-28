#pragma once

#include "Matrix.h"
#include "Vector3.h"

struct Box2D {
public:
	FORCEINLINE Box2D();
	FORCEINLINE Box2D(const Vector2 &i_center, const Vector2 &i_extend);
	FORCEINLINE Box2D(const Box2D &i_other);

	FORCEINLINE Box2D& operator=(const Box2D &i_other);

	inline Box2D& Transform(const Matrix &i_matrix);
	inline Box2D GetTransform(const Matrix &i_matrix);

private:
	Vector2 center_, extend_;
};




// implement forceinline

FORCEINLINE Box2D::Box2D()
	: center_(0.0f, 0.0f), extend_(0.0f, 0.0f)
{
}

FORCEINLINE Box2D::Box2D(const Vector2 &i_center, const Vector2 &i_extend)
	: center_(i_center), extend_(i_extend)
{
}

FORCEINLINE Box2D::Box2D(const Box2D &i_other)
	: center_(i_other.center_), extend_(i_other.extend_)
{
}

FORCEINLINE Box2D &Box2D::operator=(const Box2D &i_other)
{
	center_ = i_other.center_;
	extend_ = i_other.extend_;
	return *this;
}

inline Box2D& Box2D::Transform(const Matrix &i_matrix)
{
	center_ = Vector2(Vector4(center_).ToPoint().Mul(i_matrix));
	extend_ = Vector2(Vector4(extend_).ToPoint().Mul(i_matrix));
	return *this;
}

inline Box2D Box2D::GetTransform(const Matrix &i_matrix)
{
	return Box2D(Vector2(Vector4(center_).ToPoint().Mul(i_matrix)), Vector2(Vector4(extend_).ToPoint().Mul(i_matrix)));
}
