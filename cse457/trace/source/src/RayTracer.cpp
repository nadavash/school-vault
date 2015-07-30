// The main ray tracer.

#pragma warning (disable: 4786)

#include "RayTracer.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"

#include "parser/Tokenizer.h"
#include "parser/Parser.h"

#include "ui/TraceUI.h"
#include <cmath>
#include <algorithm>

extern TraceUI* traceUI;

#include <iostream>
#include <fstream>

using namespace std;

// Use this variable to decide if you want to print out
// debugging messages.  Gets set in the "trace single ray" mode
// in TraceGLWindow, for example.
bool debugMode = false;

// Trace a top-level ray through normalized window coordinates (x,y)
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of 0.
Vec3d RayTracer::trace( double x, double y )
{
	// Clear out the ray cache in the scene for debugging purposes,
	if (!traceUI->isMultithreading())
		scene->intersectCache.clear();

    ray r( Vec3d(0,0,0), Vec3d(0,0,0), ray::VISIBILITY );

    scene->getCamera().rayThrough( x,y,r );
	Vec3d ret = traceRay( r, Vec3d(1.0,1.0,1.0), 0 );
	ret.clamp();
	return ret;
}

// Do recursive ray tracing!  You'll want to insert a lot of code here
// (or places called from here) to handle reflection, refraction, etc etc.
Vec3d RayTracer::traceRay( const ray& r, 
	const Vec3d& thresh, int depth )
{

		isect i;

		if (scene->intersect(r, i)) {
			// YOUR CODE HERE

			// An intersection occured!  We've got work to do.  For now,
			// this code gets the material for the surface that was intersected,
			// and asks that material to provide a color for the ray.  

			// This is a great place to insert code for recursive ray tracing.
			// Instead of just returning the result of shade(), add some
			// more steps: add in the contributions from reflected and refracted
			// rays.


			const Material& m = i.getMaterial();

			double n_i;
			double n_t;

			if (i.N * -r.getDirection() >= 0) { // entering material
				n_i = 1.0003; // coming from air
				n_t = m.index(i);
			}
			else {
				n_i = m.index(i);
				n_t = 1.0003; // going into air
				i.N = -i.N; // normal is flipped
			}

			Vec3d shadeIntensity = m.shade(scene, r, i);

			Vec3d reflectedIntensity(0, 0, 0);
			Vec3d transmittedIntensity(0, 0, 0);

			if ((!traceUI->getTerminationEnabled() && depth < traceUI->getDepth()) ||
				(traceUI->getTerminationEnabled() && thresh.length() > traceUI->getTermination())) {

				if (!m.kr(i).iszero()) {
					Vec3d reflectionDirection = r.getDirection() - 2 * (r.getDirection() * i.N) * i.N;
					reflectionDirection.normalize();

					ray reflectedRay(r.at(i.t), reflectionDirection, ray::REFLECTION);

					Vec3d reflectedTrace = traceRay(reflectedRay, prod(thresh, m.kr(i)), depth + 1);
					reflectedIntensity = prod(m.kr(i), reflectedTrace);
				}

				if (!m.kt(i).iszero()) {
					double refractionCoefficient = n_i / n_t;

					Vec3d V = -r.getDirection();

					double cos_theta_i = i.N * V;

					double cos_theta_t_squared = 1 - (pow(refractionCoefficient, 2) * (1 - pow(cos_theta_i, 2)));

					Vec3d transmittedTrace(0, 0, 0);

					if (cos_theta_t_squared >= 0) { // not total internal reflection
						double cos_theta_t = sqrt(cos_theta_t_squared);

						Vec3d transmittedDirection = (refractionCoefficient * cos_theta_i - cos_theta_t) * i.N - refractionCoefficient * V;
						transmittedDirection.normalize();

						ray transmittedRay(r.at(i.t), transmittedDirection, ray::REFRACTION);

						//std::cout << m.kt(i).n[0] << " " << m.kt(i).n[1] << " " << m.kt(i).n[2] << "\n";
						//std::cout << "prod = " << prod(thresh, m.kt(i)) << "\n";

						transmittedTrace = traceRay(transmittedRay, prod(thresh, m.kt(i)), depth + 1);

					}

					transmittedIntensity = prod(m.kt(i), transmittedTrace);
				}
			}

			return shadeIntensity + reflectedIntensity + transmittedIntensity;

		}
		else {
			// No intersection.  This ray travels to infinity, so we color
			// it according to the background color, which in this (simple) case
			// is just black.

			return Vec3d(0.0, 0.0, 0.0);
		}
	// we reached the end of recursion
	return Vec3d(0.0, 0.0, 0.0);
}

RayTracer::RayTracer()
	: scene( 0 ), buffer( 0 ), buffer_width( 0 ), buffer_height( 0 ), m_bBufferReady( false )
{
}


RayTracer::~RayTracer()
{
	delete scene;
	delete [] buffer;
}

void RayTracer::getBuffer( unsigned char *&buf, int &w, int &h )
{
	buf = buffer;
	w = buffer_width;
	h = buffer_height;
}

double RayTracer::aspectRatio()
{
	return sceneLoaded() ? scene->getCamera().getAspectRatio() : 1;
}

bool RayTracer::loadScene( const char* fn )
{
	ifstream ifs( fn );
	if( !ifs ) {
		string msg( "Error: couldn't read scene file " );
		msg.append( fn );
		traceUI->alert( msg );
		return false;
	}
	
	// Strip off filename, leaving only the path:
	string path( fn );
	if( path.find_last_of( "\\/" ) == string::npos )
		path = ".";
	else
		path = path.substr(0, path.find_last_of( "\\/" ));

	// Call this with 'true' for debug output from the tokenizer
	Tokenizer tokenizer( ifs, false );
    Parser parser( tokenizer, path );
	try {
		delete scene;
		scene = 0;
		scene = parser.parseScene();
	} 
	catch( SyntaxErrorException& pe ) {
		traceUI->alert( pe.formattedMessage() );
		return false;
	}
	catch( ParserException& pe ) {
		string msg( "Parser: fatal exception " );
		msg.append( pe.message() );
		traceUI->alert( msg );
		return false;
	}
	catch( TextureMapException e ) {
		string msg( "Texture mapping exception: " );
		msg.append( e.message() );
		traceUI->alert( msg );
		return false;
	}


	if( ! sceneLoaded() )
		return false;


	// Initialize the scene's BSP tree
	scene->initBSPTree();

	
	return true;
}

void RayTracer::traceSetup( int w, int h )
{
	if( buffer_width != w || buffer_height != h )
	{
		buffer_width = w;
		buffer_height = h;

		bufferSize = buffer_width * buffer_height * 3;
		delete [] buffer;
		buffer = new unsigned char[ bufferSize ];

	}
	memset( buffer, 0, w*h*3 );
	m_bBufferReady = true;
}

void RayTracer::tracePixel( int i, int j )
{
	Vec3d col(0,0,0);

	if( ! sceneLoaded() )
		return;

	double x = double(i) / double(buffer_width);
	double y = double(j) / double(buffer_height);

	if (traceUI->getAntialiasEnabled()) {
		int segments = 5;

		double sampleXBoxSideLength = (1.0 / (segments * double(buffer_width)));
		double sampleYBoxSideLength = (1.0 / (segments * double(buffer_height)));

		for (int xChange = -(segments / 2); xChange <= (segments / 2); xChange++) {
			for (int yChange = -(segments / 2); yChange <= (segments / 2); yChange++) {

				double jitterX = ((rand() - (RAND_MAX / 2)) / RAND_MAX) * sampleXBoxSideLength;
				double jitterY = ((rand() - (RAND_MAX / 2)) / RAND_MAX) * sampleYBoxSideLength;

				double newX = x + (sampleXBoxSideLength * xChange) + jitterX;
				double newY = y + (sampleYBoxSideLength * yChange) + jitterY;
				col = col + trace(newX, newY);
			}
		}

		col = col / (pow(segments, 2));
	}
	else {
		col = trace(x, y);
	}

	unsigned char *pixel = buffer + ( i + j * buffer_width ) * 3;

	pixel[0] = (int)( 255.0 * col[0]);
	pixel[1] = (int)( 255.0 * col[1]);
	pixel[2] = (int)( 255.0 * col[2]);
}

