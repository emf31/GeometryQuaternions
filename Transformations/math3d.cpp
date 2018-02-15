#include "math3d.h"
#include <math.h>

VECTOR3D Add(VECTOR3D a, VECTOR3D b) //Ya implementado como ejemplo.
{
	VECTOR3D ret;
	ret.x = a.x + b.x;
	ret.y = a.y + b.y;
	ret.z = a.z + b.z;
	return ret;
}

VECTOR3D Substract(VECTOR3D a, VECTOR3D b)
{
	VECTOR3D ret;
	ret.x = a.x - b.x;
	ret.y = a.y - b.y;
	ret.z = a.z - b.z;
	return ret;
}

VECTOR3D Multiply(VECTOR3D a, VECTOR3D b)
{
	VECTOR3D ret;
	ret.x = a.x * b.x;
	ret.y = a.y * b.y;
	ret.z = a.z * b.z;
	return ret;
}

VECTOR3D MultiplyWithScalar(float scalar, VECTOR3D a)
{
	VECTOR3D ret;
	ret.x = a.x * scalar;
	ret.y = a.y * scalar;
	ret.z = a.z * scalar;
	return ret;
}

double Magnitude(VECTOR3D a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

VECTOR3D Normalize(VECTOR3D a)
{
	VECTOR3D ret;
	double mag = Magnitude(a);
	ret.x = a.x / mag;
	ret.y = a.y / mag;
	ret.z = a.z / mag;
	return ret;
}

VECTOR3D CrossProduct(VECTOR3D a, VECTOR3D b)
{
	VECTOR3D ret;
	ret.x = a.y*b.z - a.z*b.y;
	ret.y = a.z*b.x - a.x*b.z;
	ret.z = a.x*b.y - a.y*b.x;
	return ret;
}

double DotProduct(VECTOR3D a, VECTOR3D b)
{
	return a.x*b.x+a.y*b.y+a.z*b.z;
}

MATRIX3 Transpose(MATRIX3 m)
{
	MATRIX3 ret;
	ret.column0.x = m.column0.x;
	ret.column0.y = m.column1.x;
	ret.column0.z = m.column2.x;

	ret.column1.x = m.column0.y;
	ret.column1.y = m.column1.y;
	ret.column1.z = m.column2.y;

	ret.column2.x = m.column0.z;
	ret.column2.y = m.column1.z;
	ret.column2.z = m.column2.z;

	return ret;
}

VECTOR3D Transform(MATRIX3 m, VECTOR3D a)
{
	VECTOR3D ret;
	ret.x = m.column0.x * a.x + m.column0.y * a.y + m.column0.z * a.z;
	ret.y = m.column1.x * a.x + m.column1.y * a.y + m.column1.z * a.z;
	ret.z = m.column2.x * a.x + m.column2.y * a.y + m.column2.z * a.z;
	return ret;
}

MATRIX4 InverseOrthogonalMatrix(MATRIX3 A, VECTOR3D t)
{
	 MATRIX4 ret;
	 MATRIX3 At = Transpose(A);
	 
	 ret.m[0] = At.column0.x;
	 ret.m[1] = At.column0.y;
	 ret.m[2] = At.column0.z;
	 ret.m[3] = 0;

	 ret.m[4] = At.column1.x;
	 ret.m[5] = At.column1.y;
	 ret.m[6] = At.column1.z;
	 ret.m[7] = 0;

	 ret.m[8] = At.column2.x;
	 ret.m[9] = At.column2.y;
	 ret.m[10] = At.column2.z;
	 ret.m[11] = 0;

	 ret.m[12] = -DotProduct(t, A.column0);
	 ret.m[13] = -DotProduct(t, A.column1);
	 ret.m[14] = -DotProduct(t, A.column2);
	 ret.m[15] = 1;

	 return ret;

}

QUATERNION QuaternionFromAngleAxis(float angle, VECTOR3D axis)
{
	QUATERNION q;
	q.s = cos(angle);
	q.v = {sin((angle/2)*axis.x), sin((angle / 2)*axis.y), sin((angle / 2)*axis.z)};
	return q;
}

QUATERNION QuaternionFromToVectors(VECTOR3D from, VECTOR3D to)
{
	QUATERNION q;
	q.v = Normalize(CrossProduct(from, to));
	Magnitude(from);
	q.s = sqrt((Magnitude(from) * Magnitude(from)) * (Magnitude(to) * Magnitude(to))) + DotProduct(from, to);
	return q;
}

QUATERNION Multiply(QUATERNION a, QUATERNION b)
{
	QUATERNION q;
	q.v.x = a.v.x  * b.s   + a.v.y * b.v.z - a.v.z * b.v.y + a.s * b.v.x;
	q.v.y = -a.v.x * b.v.z + a.v.y * b.s   + a.v.z * b.v.x + a.s * b.v.y;
	q.v.z = a.v.x  * b.v.y - a.v.y * b.v.x + a.v.z * b.s   + a.s * b.v.z;
	q.s   = -a.v.x * b.v.x - a.v.y * b.v.y - a.v.z * b.v.z + a.s * b.s  ;

	return q;
}

QUATERNION Conjugate(QUATERNION a)
{
	QUATERNION q;
	q.v.x = -a.v.x;
	q.v.y = -a.v.y;
	q.v.z = -a.v.z;
	q.s   = a.s;
	return q;
}

VECTOR3D RotateWithQuaternion(VECTOR3D a, QUATERNION q)
{
	QUATERNION qConjugate = Conjugate(q);
	QUATERNION p;
	p.s = 0;
	p.v = a;
	QUATERNION sol = Multiply(Multiply(q, p), qConjugate);
	return sol.v;
}
