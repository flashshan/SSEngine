#pragma once

#include "Vector4.h"

struct Matrix {
public:
	FORCEINLINE Matrix() {}
	FORCEINLINE Matrix(float i_11, float i_12, float i_13, float i_14,
		float i_21, float i_22, float i_23, float i_24,
		float i_31, float i_32, float i_33, float i_34,
		float i_41, float i_42, float i_43, float i_44);
	FORCEINLINE explicit Matrix(float *i_val);
	FORCEINLINE Matrix(const Vector4 &i_v1, const Vector4 &i_v2, const Vector4 &i_v3, const Vector4 &i_v4);
	FORCEINLINE Matrix(const Matrix &i_other);

	FORCEINLINE void operator =(const Matrix &i_other);

	FORCEINLINE static Matrix CreateZero();
	FORCEINLINE static Matrix CreateIdentity();

	FORCEINLINE static Matrix CreateTranslate(const Vector3 i_Vector);
	FORCEINLINE static Matrix CreateXRotation(const float i_XRadian);
	FORCEINLINE static Matrix CreateYRotation(const float i_YRadian);
	FORCEINLINE static Matrix CreateZRotation(const float i_ZRadian);
	FORCEINLINE static Matrix CreateScale(const float i_xScale, const float i_yScale, const float i_zScale);

	FORCEINLINE bool IsSingular() const;

	FORCEINLINE void Transpose();
	FORCEINLINE Matrix GetTranspose() const;

	void Inverse();
	Matrix GetInverse() const;

	Matrix operator*(const Matrix &i_other) const;
	void operator*=(const Matrix &i_other);
	FORCEINLINE float operator [](const int i_index) const;

	Vector4 MultiplyLeft(const Vector4 &i_vector) const;
	Vector4 MultiplyRight(const Vector4 &i_vector) const;

public:
	static Matrix FastMatrix;

public:
	ALIGN(16) float M[4][4];
};





// implement forceinline

FORCEINLINE Matrix::Matrix(float i_11, float i_12, float i_13, float i_14,
	float i_21, float i_22, float i_23, float i_24,
	float i_31, float i_32, float i_33, float i_34,
	float i_41, float i_42, float i_43, float i_44)
	:M{ i_11, i_12, i_13, i_14, i_21, i_22, i_23, i_24, i_31, i_32, i_33, i_34, i_41, i_42, i_43, i_44 }
{
}

FORCEINLINE Matrix::Matrix(float *i_val)
{
	for (int i = 0;i < 16;++i)
		M[i / 4][i % 4] = i_val[i];
}

FORCEINLINE Matrix::Matrix(const Vector4 &i_v1, const Vector4 &i_v2, const Vector4 &i_v3, const Vector4 &i_v4)
	: M{ i_v1.X, i_v1.Y, i_v1.Z, i_v1.W,
		 i_v2.X, i_v2.Y, i_v2.Z, i_v2.W,
		 i_v3.X, i_v3.Y, i_v3.Z, i_v3.W,
		 i_v4.X, i_v4.Y, i_v4.Z, i_v4.W }
{
}

FORCEINLINE Matrix::Matrix(const Matrix &i_other)
	: M{ i_other.M[0][0], i_other.M[0][1], i_other.M[0][2], i_other.M[0][3],
		 i_other.M[1][0], i_other.M[1][1], i_other.M[1][2], i_other.M[1][3],
		 i_other.M[2][0], i_other.M[2][1], i_other.M[2][2], i_other.M[2][3],
		 i_other.M[3][0], i_other.M[3][1], i_other.M[3][2], i_other.M[3][3] }
{
}

FORCEINLINE void Matrix::operator =(const Matrix &i_other)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			M[i][j] = i_other.M[i][j];
}

FORCEINLINE Matrix Matrix::CreateZero()
{
	return Matrix(0.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 0.0f, 0.0f);
}

FORCEINLINE Matrix Matrix::CreateIdentity()
{
	return Matrix(1.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 1.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 1.0f, 0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f);
}

FORCEINLINE Matrix Matrix::CreateTranslate(const Vector3 i_Vector)
{
	return Matrix(1.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 1.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 1.0f, 0.0f,
				  i_Vector.X, i_Vector.Y, i_Vector.Z, 1.0f);
}

FORCEINLINE Matrix Matrix::CreateXRotation(const float i_XRadian)
{
	float Cos = cos(i_XRadian), Sin = sin(i_XRadian);
	return Matrix(1.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, Cos, Sin, 0.0f,
				  0.0f, -Sin, Cos, 0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f);
}

FORCEINLINE Matrix Matrix::CreateYRotation(const float i_YRadian)
{
	float Cos = cos(i_YRadian), Sin = sin(i_YRadian);
	return Matrix(Cos, 0.0f, Sin, 0.0f,
				  0.0f, 1.0f, 0.0f, 0.0f,
				  -Sin, 0.0f, Cos, 0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f);
}

FORCEINLINE Matrix Matrix::CreateZRotation(const float i_ZRadian)
{
	float Cos = cos(i_ZRadian), Sin = sin(i_ZRadian);
	return Matrix(Cos, Sin, 0.0f, 0.0f,
				  -Sin, Cos, 0.0f, 0.0f,
				  0.0f, 0.0f, 1.0f, 0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f);
}

FORCEINLINE Matrix Matrix::CreateScale(const float i_xScale, const float i_yScale, const float i_zScale)
{
	return Matrix(i_xScale, 0.0f, 0.0f, 0.0f,
				  0.0f, i_yScale, 0.0f, 0.0f,
				  0.0f, 0.0f, i_zScale, 0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f);
}

FORCEINLINE bool Matrix::IsSingular() const
{
	return (Float::IsZero((M[0][0] * M[0][0] + M[0][1] * M[0][1] + M[0][2] * M[0][2] + M[0][3] * M[0][3])) ||
			Float::IsZero((M[1][0] * M[1][0] + M[1][1] * M[1][1] + M[1][2] * M[1][2] + M[1][3] * M[1][3])) ||
			Float::IsZero((M[2][0] * M[2][0] + M[2][1] * M[2][1] + M[2][2] * M[2][2] + M[2][3] * M[2][3])) ||
			Float::IsZero((M[3][0] * M[3][0] + M[3][1] * M[3][1] + M[3][2] * M[3][2] + M[3][3] * M[3][3])));
}

FORCEINLINE void Matrix::Transpose()
{
	float temp;
	temp = M[0][1]; M[0][1] = M[1][0]; M[1][0] = temp;
	temp = M[0][2]; M[0][2] = M[2][0]; M[2][0] = temp;
	temp = M[0][3]; M[0][3] = M[3][0]; M[3][0] = temp;
	temp = M[1][2]; M[1][2] = M[2][1]; M[2][1] = temp;
	temp = M[1][3]; M[1][3] = M[3][1]; M[3][1] = temp;
	temp = M[2][3]; M[2][3] = M[3][2]; M[3][2] = temp;
}

FORCEINLINE Matrix Matrix::GetTranspose() const
{
	return Matrix(M[0][0], M[1][0], M[2][0], M[3][0],
				  M[0][1], M[1][1], M[2][1], M[3][1],
				  M[0][2], M[1][2], M[2][2], M[3][2],
				  M[0][3], M[1][3], M[2][3], M[3][3]);
}

FORCEINLINE float Matrix::operator [](const int i_index) const
{
	ASSERT(i_index < 16);
	return M[i_index / 4][i_index % 4];
}



// for Vector4
FORCEINLINE Vector4 Vector4::Transform(const Matrix &i_matrix) const
{
	return i_matrix.MultiplyLeft(*this);
}
