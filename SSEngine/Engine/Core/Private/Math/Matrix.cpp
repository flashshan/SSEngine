#include "Core\Math\Matrix.h"

#include <xmmintrin.h>


// SIMD optimization
#define SHUFFLE_PARAM(x, y, z, w)  ((x) | ((y) << 2) | ((z) << 4) | ((w) << 6))

#define _mm_replicate_x_ps(v) _mm_shuffle_ps((v), (v), SHUFFLE_PARAM(0, 0, 0, 0))
#define _mm_replicate_y_ps(v) _mm_shuffle_ps((v), (v), SHUFFLE_PARAM(1, 1, 1, 1))
#define _mm_replicate_z_ps(v) _mm_shuffle_ps((v), (v), SHUFFLE_PARAM(2, 2, 2, 2))
#define _mm_replicate_w_ps(v) _mm_shuffle_ps((v), (v), SHUFFLE_PARAM(3, 3, 3, 3))

static Matrix FastMatrix(1.0f, 0.0f, 0.0f, 0.0f,
						 0.0f, 1.0f, 0.0f, 0.0f,
						 0.0f, 0.0f, 1.0f, 0.0f,
						 0.0f, 0.0f, 0.0f, 1.0f);

Matrix& Matrix::Transpose()
{
	float temp;
	temp = M[0][1]; M[0][1] = M[1][0]; M[1][0] = temp;
	temp = M[0][2]; M[0][2] = M[2][0]; M[2][0] = temp;
	temp = M[0][3]; M[0][3] = M[3][0]; M[3][0] = temp;
	temp = M[1][2]; M[1][2] = M[2][1]; M[2][1] = temp;
	temp = M[1][3]; M[1][3] = M[3][1]; M[3][1] = temp;
	temp = M[2][3]; M[2][3] = M[3][2]; M[3][2] = temp;
	return *this;
}
//fast inverse from Unreal
static inline void Inverse4x4(float *o_dst, const float *i_src)
{
	const double s0 = static_cast<double>(i_src[0]);   const double s1 = static_cast<double>(i_src[1]);   const double s2 = static_cast<double>(i_src[2]);   const double s3 = static_cast<double>(i_src[3]);
	const double s4 = static_cast<double>(i_src[4]);   const double s5 = static_cast<double>(i_src[5]);   const double s6 = static_cast<double>(i_src[6]);   const double s7 = static_cast<double>(i_src[7]);
	const double s8 = static_cast<double>(i_src[8]);   const double s9 = static_cast<double>(i_src[9]);   const double s10 = static_cast<double>(i_src[10]); const double s11 = static_cast<double>(i_src[11]);
	const double s12 = static_cast<double>(i_src[12]); const double s13 = static_cast<double>(i_src[13]); const double s14 = static_cast<double>(i_src[14]); const double s15 = static_cast<double>(i_src[15]);

	double inv[16];
	inv[0] = s5 * s10 * s15 - s5 * s11 * s14 - s9 * s6 * s15 + s9 * s7 * s14 + s13 * s6 * s11 - s13 * s7 * s10;
	inv[1] = -s1 * s10 * s15 + s1 * s11 * s14 + s9 * s2 * s15 - s9 * s3 * s14 - s13 * s2 * s11 + s13 * s3 * s10;
	inv[2] = s1 * s6  * s15 - s1 * s7  * s14 - s5 * s2 * s15 + s5 * s3 * s14 + s13 * s2 * s7 - s13 * s3 * s6;
	inv[3] = -s1 * s6  * s11 + s1 * s7  * s10 + s5 * s2 * s11 - s5 * s3 * s10 - s9  * s2 * s7 + s9  * s3 * s6;
	inv[4] = -s4 * s10 * s15 + s4 * s11 * s14 + s8 * s6 * s15 - s8 * s7 * s14 - s12 * s6 * s11 + s12 * s7 * s10;
	inv[5] = s0 * s10 * s15 - s0 * s11 * s14 - s8 * s2 * s15 + s8 * s3 * s14 + s12 * s2 * s11 - s12 * s3 * s10;
	inv[6] = -s0 * s6  * s15 + s0 * s7  * s14 + s4 * s2 * s15 - s4 * s3 * s14 - s12 * s2 * s7 + s12 * s3 * s6;
	inv[7] = s0 * s6  * s11 - s0 * s7  * s10 - s4 * s2 * s11 + s4 * s3 * s10 + s8  * s2 * s7 - s8  * s3 * s6;
	inv[8] = s4 * s9  * s15 - s4 * s11 * s13 - s8 * s5 * s15 + s8 * s7 * s13 + s12 * s5 * s11 - s12 * s7 * s9;
	inv[9] = -s0 * s9  * s15 + s0 * s11 * s13 + s8 * s1 * s15 - s8 * s3 * s13 - s12 * s1 * s11 + s12 * s3 * s9;
	inv[10] = s0 * s5  * s15 - s0 * s7  * s13 - s4 * s1 * s15 + s4 * s3 * s13 + s12 * s1 * s7 - s12 * s3 * s5;
	inv[11] = -s0 * s5  * s11 + s0 * s7  * s9 + s4 * s1 * s11 - s4 * s3 * s9 - s8  * s1 * s7 + s8  * s3 * s5;
	inv[12] = -s4 * s9  * s14 + s4 * s10 * s13 + s8 * s5 * s14 - s8 * s6 * s13 - s12 * s5 * s10 + s12 * s6 * s9;
	inv[13] = s0 * s9  * s14 - s0 * s10 * s13 - s8 * s1 * s14 + s8 * s2 * s13 + s12 * s1 * s10 - s12 * s2 * s9;
	inv[14] = -s0 * s5  * s14 + s0 * s6  * s13 + s4 * s1 * s14 - s4 * s2 * s13 - s12 * s1 * s6 + s12 * s2 * s5;
	inv[15] = s0 * s5  * s10 - s0 * s6  * s9 - s4 * s1 * s10 + s4 * s2 * s9 + s8  * s1 * s6 - s8  * s2 * s5;

	double det = s0 * inv[0] + s1 * inv[4] + s2 * inv[8] + s3 * inv[12];
	if (det != 0.0)
	{
		det = 1.0 / det;
	}
	for (int i = 0; i < 16; ++i)
	{
		o_dst[i] = static_cast<float>(inv[i] * det);
	}
}

Matrix& Matrix::Inverse()
{
	if (IsSingular())
	{
		DEBUG_PRINT("Inverse Singular Matrix!\n");
		for (int i = 0;i < 16; ++i)
		{
			if (i / 4 == i % 4) M[i / 4][i % 4] = 1;
			else M[i / 4][i % 4] = 0;
		}
	}
	else
	{
		float res[16];
		Inverse4x4(res, M[0]);
		for (int i = 0;i < 16;++i)
		{
			M[i / 4][i % 4] = res[i];
		}
	}
	return *this;
}


Matrix Matrix::GetInverse() const
{
	if (IsSingular())
	{
		DEBUG_PRINT("Inverse Singular Matrix!\n");
		return Matrix::CreateIdentity();
	}

	float res[16];
	Inverse4x4(res, M[0]);
	return Matrix(res);
}

Matrix Matrix::operator*(const float i_float) const
{
	return Matrix(M[0][0] * i_float, M[0][1] * i_float, M[0][2] * i_float, M[0][3] * i_float,
				  M[1][0] * i_float, M[1][1] * i_float, M[1][2] * i_float, M[1][3] * i_float,
				  M[2][0] * i_float, M[2][1] * i_float, M[2][2] * i_float, M[2][3] * i_float,
				  M[3][0] * i_float, M[3][1] * i_float, M[3][2] * i_float, M[3][3] * i_float);
}

Matrix& Matrix::operator*=(const float i_float)
{
	for (int i = 0;i < 16;++i)
	{
		M[i / 4][i % 4] *= i_float;
	}
	return *this;
}

Matrix Matrix::operator/(const float i_float) const
{
	return Matrix(M[0][0] / i_float, M[0][1] / i_float, M[0][2] / i_float, M[0][3] / i_float,
		M[1][0] / i_float, M[1][1] / i_float, M[1][2] / i_float, M[1][3] / i_float,
		M[2][0] / i_float, M[2][1] / i_float, M[2][2] / i_float, M[2][3] / i_float,
		M[3][0] / i_float, M[3][1] / i_float, M[3][2] / i_float, M[3][3] / i_float);
}
Matrix& Matrix::operator/=(const float i_float)
{
	for (int i = 0;i < 16;++i)
	{
		M[i / 4][i % 4] /= i_float;
	}
	return *this;
}

Matrix Matrix::operator*(const Matrix &i_other) const
{
	__m128 r11 = _mm_load_ps(&M[0][0]), r12 = _mm_load_ps(&M[1][0]), r13 = _mm_load_ps(&M[2][0]), r14 = _mm_load_ps(&M[3][0]);
	__m128 r21 = _mm_load_ps(&i_other.M[0][0]), r22 = _mm_load_ps(&i_other.M[1][0]), r23 = _mm_load_ps(&i_other.M[2][0]), r24 = _mm_load_ps(&i_other.M[3][0]);

	ALIGN(16) float row1[4] = { 0.0f, 0.0f ,0.0f ,0.0f };
	ALIGN(16) float row2[4] = { 0.0f, 0.0f ,0.0f ,0.0f };
	ALIGN(16) float row3[4] = { 0.0f, 0.0f ,0.0f ,0.0f };
	ALIGN(16) float row4[4] = { 0.0f, 0.0f ,0.0f ,0.0f };

	__m128 result;
	result = _mm_mul_ps(_mm_replicate_x_ps(r11), r21);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_y_ps(r11), r22), result);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_z_ps(r11), r23), result);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_w_ps(r11), r24), result);
	_mm_store_ps(&row1[0], result);

	result = _mm_mul_ps(_mm_replicate_x_ps(r12), r21);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_y_ps(r12), r22), result);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_z_ps(r12), r23), result);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_w_ps(r12), r24), result);
	_mm_store_ps(&row2[0], result);

	result = _mm_mul_ps(_mm_replicate_x_ps(r13), r21);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_y_ps(r13), r22), result);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_z_ps(r13), r23), result);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_w_ps(r13), r24), result);
	_mm_store_ps(&row3[0], result);

	result = _mm_mul_ps(_mm_replicate_x_ps(r14), r21);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_y_ps(r14), r22), result);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_z_ps(r14), r23), result);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_w_ps(r14), r24), result);
	_mm_store_ps(&row4[0], result);
	
	return Matrix(row1[0], row1[1], row1[2], row1[3],
				  row2[0], row2[1], row2[2], row2[3],
				  row3[0], row3[1], row3[2], row3[3],
				  row4[0], row4[1], row4[2], row4[3]);
}

Matrix& Matrix::operator*=(const Matrix &i_other)
{
	__m128 r11 = _mm_load_ps(&M[0][0]), r12 = _mm_load_ps(&M[1][0]), r13 = _mm_load_ps(&M[2][0]), r14 = _mm_load_ps(&M[3][0]);
	__m128 r21 = _mm_load_ps(&i_other.M[0][0]), r22 = _mm_load_ps(&i_other.M[1][0]), r23 = _mm_load_ps(&i_other.M[2][0]), r24 = _mm_load_ps(&i_other.M[3][0]);

	__m128 result;
	result = _mm_mul_ps(_mm_replicate_x_ps(r11), r21);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_y_ps(r11), r22), result);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_z_ps(r11), r23), result);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_w_ps(r11), r24), result);
	_mm_store_ps(&M[0][0], result);

	result = _mm_mul_ps(_mm_replicate_x_ps(r12), r21);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_y_ps(r12), r22), result);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_z_ps(r12), r23), result);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_w_ps(r12), r24), result);
	_mm_store_ps(&M[1][0], result);

	result = _mm_mul_ps(_mm_replicate_x_ps(r13), r21);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_y_ps(r13), r22), result);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_z_ps(r13), r23), result);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_w_ps(r13), r24), result);
	_mm_store_ps(&M[2][0], result);

	result = _mm_mul_ps(_mm_replicate_x_ps(r14), r21);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_y_ps(r14), r22), result);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_z_ps(r14), r23), result);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_w_ps(r14), r24), result);
	_mm_store_ps(&M[3][0], result);
	return *this;
}

// normally used, Matrix is on the left
Vector4 Matrix::MultiplyLeft(const Vector4 &i_vector) const
{	
	__m128 v = _mm_load_ps(&i_vector.X);
	__m128 r1 = _mm_load_ps(&M[0][0]), r2 = _mm_load_ps(&M[1][0]), r3 = _mm_load_ps(&M[2][0]), r4 = _mm_load_ps(&M[3][0]);

	__m128 result;
	result = _mm_mul_ps(_mm_replicate_x_ps(v), r1);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_y_ps(v), r2), result);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_z_ps(v), r3), result);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_w_ps(v), r4), result);
	
	ALIGN(16) float resultVector[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	_mm_store_ps(&resultVector[0], result);
	return Vector4(resultVector[0], resultVector[1], resultVector[2], resultVector[3]);
}

// slower, matrix is on the right
Vector4 Matrix::MultiplyRight(const Vector4 &i_vector) const
{
	Matrix transpose = GetTranspose();

	__m128 v = _mm_load_ps(&i_vector.X);
	__m128 r1 = _mm_load_ps(&transpose.M[0][0]), r2 = _mm_load_ps(&transpose.M[1][0]), r3 = _mm_load_ps(&transpose.M[2][0]), r4 = _mm_load_ps(&transpose.M[3][0]);

	__m128 result;
	result = _mm_mul_ps(_mm_replicate_x_ps(v), r1);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_y_ps(v), r2), result);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_z_ps(v), r3), result);
	result = _mm_add_ps(_mm_mul_ps(_mm_replicate_w_ps(v), r4), result);

	ALIGN(16) float resultVector[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	_mm_store_ps(&resultVector[0], result);
	return Vector4(resultVector[0], resultVector[1], resultVector[2], resultVector[3]);
}

