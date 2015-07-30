#include "ray.h"
#include "material.h"
#include "light.h"

#include "../fileio/imageio.h"

using namespace std;
extern bool debugMode;


// Apply the Blinn-Phong model to this point on the surface of the object, 
//  returning the color of that point.
Vec3d Material::shade( Scene *scene, const ray& r, const isect& i ) const
{
	// YOUR CODE HERE

	// For now, this method just returns the diffuse color of the object.
	// This gives a single matte color for every distinct surface in the
	// scene, and that's it.  Simple, but enough to get you started.
	// (It's also inconsistent with the Phong model...)

	// Your mission is to fill in this method with the rest of the phong
	// shading model, including the contributions of all the light sources.
    // You will need to call both distanceAttenuation() and shadowAttenuation()
    // somewhere in your code in order to compute shadows and light falloff.
	if( debugMode )
		std::cout << "Debugging the Phong code (or lack thereof...)" << std::endl;

	// When you're iterating through the lights,
	// you'll want to use code that looks something
	// like this:
	
	Vec3d l = ke(i) + prod(ka(i), scene->ambient());
	Vec3d q = r.at(i.t);
	for ( vector<Light*>::const_iterator litr = scene->beginLights(); 
		litr != scene->endLights(); 
		++litr )
	{
		Light* pLight = *litr;
		auto viewer = -r.getDirection();
		viewer.normalize();

		auto lightDir = pLight->getDirection(q);
		auto halfway = lightDir + viewer;
		halfway.normalize();

		auto normal = i.N;

		float b = 1.0;
		if (normal * lightDir < 0.0) b = 0.0;

		// Compute geometric terms
		double diffuseShade = max(normal * lightDir, 0);
		double specShade = pow(max(normal * halfway, 0.0), shininess(i));

		auto atten = pLight->distanceAttenuation(q) * pLight->shadowAttenuation(q);

		l = l + b * prod(pLight->getColor(), prod(atten, kd(i) * diffuseShade + ks(i) * specShade));
	}
	return l;
}


TextureMap::TextureMap( string filename )
{
    data = load( filename.c_str(), width, height );
    if( 0 == data )
    {
        width = 0;
        height = 0;
        string error( "Unable to load texture map '" );
        error.append( filename );
        error.append( "'." );
        throw TextureMapException( error );
    }
}

Vec3d TextureMap::getMappedValue( const Vec2d& coord ) const
{
	// YOUR CODE HERE

    // In order to add texture mapping support to the 
    // raytracer, you need to implement this function.
    // What this function should do is convert from
    // parametric space which is the unit square
    // [0, 1] x [0, 1] in 2-space to Image coordinates,
    // and use these to perform bilinear interpolation
    // of the values.
	Vec2d texSpace(coord[0] * width, coord[1] * height);
	int i = (int)texSpace[0];
	int j = (int)texSpace[1];
	
	double tmp;
	double deltaX = modf(texSpace[0], &tmp);
	double deltaY = modf(texSpace[1], &tmp);
	
	double dx1 = 1 - deltaX;
	double dy1 = 1 - deltaY;

	
    return dx1 * dy1 * getPixelAt(i, j) + deltaX * dy1 * getPixelAt(i + 1, j) +
		dx1 * deltaY * getPixelAt(i, j + 1) + deltaX * deltaY * getPixelAt(i + 1, j + 1);
}


Vec3d TextureMap::getPixelAt( int x, int y ) const
{
    // This keeps it from crashing if it can't load
    // the texture, but the person tries to render anyway.
    if (0 == data)
      return Vec3d(1.0, 1.0, 1.0);

    if( x >= width )
       x = width - 1;
    if( y >= height )
       y = height - 1;
	x = max(x, 0);
	y = max(y, 0);
    // Find the position in the big data array...
    int pos = (y * width + x) * 3;
    return Vec3d( double(data[pos]) / 255.0, 
       double(data[pos+1]) / 255.0,
       double(data[pos+2]) / 255.0 );
}

Vec3d MaterialParameter::value( const isect& is ) const
{
    if( 0 != _textureMap )
        return _textureMap->getMappedValue( is.uvCoordinates );
    else
        return _value;
}

double MaterialParameter::intensityValue( const isect& is ) const
{
    if( 0 != _textureMap )
    {
        Vec3d value( _textureMap->getMappedValue( is.uvCoordinates ) );
        return (0.299 * value[0]) + (0.587 * value[1]) + (0.114 * value[2]);
    }
    else
        return (0.299 * _value[0]) + (0.587 * _value[1]) + (0.114 * _value[2]);
}

