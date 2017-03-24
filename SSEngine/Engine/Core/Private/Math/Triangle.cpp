#include "Core\Math\Triangle.h"


void Triangle::Transform(const Matrix &i_Matrix)
{
	V_[0] = i_Matrix.MultiplyLeft(originV_[0]);
	V_[1] = i_Matrix.MultiplyLeft(originV_[1]);
	V_[2] = i_Matrix.MultiplyLeft(originV_[2]);
}