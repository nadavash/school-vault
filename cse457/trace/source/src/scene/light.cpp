#include <cmath>

#include "light.h"


using namespace std;

//#define SINGLE_SIDED

double DirectionalLight::distanceAttenuation( const Vec3d& P ) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


Vec3d DirectionalLight::shadowAttenuation( const Vec3d& P ) const
{
    // YOUR CODE HERE:
	Vec3d dir = getDirection(P);
	ray shadowRay(P, dir, ray::SHADOW);

	isect i;
	if (getScene()->intersect(shadowRay, i))
	{
		Vec3d trans = i.getMaterial().kt(i);
		if (trans.length() > 0.0) { // thing is transparent
			return prod(trans, this->shadowAttenuation(shadowRay.at(i.t)));
		}
		else {
			return trans;
		}
	}

	return Vec3d(1, 1, 1);
}

Vec3d DirectionalLight::getColor() const
{
	return color;
}

Vec3d DirectionalLight::getDirection( const Vec3d& P ) const
{
	return -orientation;
}

double PointLight::distanceAttenuation(const Vec3d& P) const
{
	double r2 = (this->position - P).length2();
	double r = sqrt(r2);

	double atten = this->constantTerm + this->linearTerm * r + this->quadraticTerm * r2;
	if (atten == 0)
		return 1.0;
	return min(1.0 / atten, 1.0);
}

Vec3d PointLight::getColor() const
{
	return color;
}

Vec3d PointLight::getDirection( const Vec3d& P ) const
{
	Vec3d ret = position - P;
	ret.normalize();
	return ret;
}


Vec3d PointLight::shadowAttenuation(const Vec3d& P) const
{
    // YOUR CODE HERE:
	Vec3d dir = getDirection(P);
	ray shadowRay(P, dir, ray::SHADOW);

	isect i;
	if (getScene()->intersect(shadowRay, i))
	{
		double tLight = (this->position - P).length();
		if (tLight > i.t) {
			Vec3d trans = i.getMaterial().kt(i);
			if (trans.length() > 0.0) { // thing is transparent
				return prod(trans, this->shadowAttenuation(shadowRay.at(i.t)));
			}
			else {
				return trans;
			}
		}
	}

    return Vec3d(1,1,1);
}
