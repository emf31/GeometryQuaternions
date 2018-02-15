#include "camera.h"
#include <math.h>
#include <stdio.h>

// TODO
// devuelve los valores de distancia de los planos a partir del fov horizontal
FRUSTUM makeFrustum(double fovX, double aspectRatio, double nearValue, double farValue)
{
	const double DEG2RAD = 3.14159265 / 180;


	double tangent = tan((fovX/2)* DEG2RAD); // tangent of half fovX
	double height = nearValue*tangent; // half height of near plane
	double width = height*aspectRatio; // half width of near plane
	

	FRUSTUM ret;

	ret.nearValue = nearValue;
	ret.farValue = farValue;
	ret.top = height;
	ret.bottom = -height;
	ret.left = -width;
	ret.right = width;


	// TODO : rellenar valores de ret
	return ret;
}

MATRIX4 lookAt(VECTOR3D eyePosition, VECTOR3D target, VECTOR3D upVector)
{
	//VECTOR3D forward = Substract(eyePosition, target);//Apunta a -z
	VECTOR3D forward = target;
	VECTOR3D side = CrossProduct(upVector,forward);
	VECTOR3D up = CrossProduct(forward,side);

	forward = Normalize(forward);
	side = Normalize(side);
	up = Normalize(up);

	MATRIX3 m;
	m.column0 = side;
	m.column1 = up;
	m.column2 = forward;

	return InverseOrthogonalMatrix(m, eyePosition);

}

void updateEulerOrientation(EULER & euler)
{
	QUATERNION qYaw,qPitch,qRoll;
	qYaw = QuaternionFromAngleAxis(euler.yaw, { 0,1,0 });
	qPitch = QuaternionFromAngleAxis(euler.pitch, { 1,0,0 });
	qRoll = QuaternionFromAngleAxis(euler.roll, { 0,0,1 });

	euler.orientation = Multiply(Multiply(qYaw, qPitch), qRoll);
	//euler.orientation = Multiply(qYaw, qPitch);

//	printf("Current Pitch: %f ,Yaw: %f\n", euler.pitch, euler.yaw);

}

VECTOR3D getForward(EULER euler)
{
	return RotateWithQuaternion({0,0,-1},euler.orientation);
}

VECTOR3D getLeft(EULER euler)
{
	return RotateWithQuaternion({ 1,0,0 }, euler.orientation);
}
