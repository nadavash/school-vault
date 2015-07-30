#define _USE_MATH_DEFINES
#include <cmath>

#include "Sphere.h"

using namespace std;


bool Sphere::intersectLocal( const ray& r, isect& i ) const
{
	Vec3d p = r.getPosition();
	Vec3d d = r.getDirection();

	double d2 = d * d;
	double disc2 = pow(d * p, 2) - d2 * (p * p - 1);
	
	if (disc2 < 0)
		return false;

	double t = 0;
	if (disc2 == 0)
	{
		t = -(d * p) / d2;
	}
	else if (disc2 > 0)
	{
		double disc = sqrt(disc2);
		double temp = -(d * p);
		double t1 = (temp + disc) / d2;
		double t2 = (temp - disc) / d2;
		if (t1 <= RAY_EPSILON && t2 > RAY_EPSILON) {
			t = t2;
		}
		else if (t2 <= RAY_EPSILON && t1 > RAY_EPSILON) {
			t = t1;
		}
		else {
			t = min(max(0, t1), max(0, t2));
		}
	}
	else 
		return false;

	if (t <= RAY_EPSILON)
		return false;

	Vec3d q = r.at(t);
	i.setT(t);
	i.setN(q);
	i.setMaterial(getMaterial());
	i.setObject(this);

	// Set uv coordinates
	double horizontalAngle = atan2(q[1], q[0]) / M_PI + 0.5;
	double verticalAngle = asin(q[2]) / M_PI + 0.5;

	i.uvCoordinates = Vec2d(horizontalAngle, verticalAngle);
	
    return true;
}

