#include <cmath>
#include <float.h>
#include "trimesh.h"

using namespace std;

Trimesh::~Trimesh()
{
	for( Materials::iterator i = materials.begin(); i != materials.end(); ++i )
		delete *i;
}

bool Trimesh::hasPerVertexNormals()
{
	return !(this->normals.empty());
}

// must add vertices, normals, and materials IN ORDER
void Trimesh::addVertex( const Vec3d &v )
{
    vertices.push_back( v );
}

void Trimesh::addMaterial( Material *m )
{
    materials.push_back( m );
}

void Trimesh::addNormal( const Vec3d &n )
{
    normals.push_back( n );
}

void Trimesh::addTextureUV( const Vec2d &n )
{
    textureuvs.push_back( n );
}

// Returns false if the vertices a,b,c don't all exist
bool Trimesh::addFace( int a, int b, int c )
{
    int vcnt = vertices.size();

    if( a >= vcnt || b >= vcnt || c >= vcnt )
        return false;

    TrimeshFace *newFace = new TrimeshFace( scene, new Material(*this->material), this, a, b, c );
    newFace->setTransform(this->transform);
    faces.push_back( newFace );
    scene->add(newFace);
    return true;
}

char *
Trimesh::doubleCheck()
// Check to make sure that if we have per-vertex materials or normals
// they are the right number.
{
    if( !materials.empty() && materials.size() != vertices.size() )
        return "Bad Trimesh: Wrong number of materials.";
    if( !normals.empty() && normals.size() != vertices.size() )
        return "Bad Trimesh: Wrong number of normals.";

    return 0;
}

// Calculates and returns the normal of the triangle too.
bool TrimeshFace::intersectLocal( const ray& r, isect& i ) const
{
    // YOUR CODE HERE:
    // Add triangle intersection code here.
    // it currently ignores all triangles and just return false.
    //
    // Note that you are only intersecting a single triangle, and the vertices
    // of the triangle are supplied to you by the trimesh class.
    //
    // You should retrieve the vertices using code like this:
    //
    // const Vec3d& a = parent->vertices[ids[0]];
    // const Vec3d& b = parent->vertices[ids[1]];
    // const Vec3d& c = parent->vertices[ids[2]];
	//
	// TrimeshFace::parent->hasPerVertexNormals tells you if the triangle has per-vertex normals.
	// If it does, you should compute and return the interpolated normal at the intersection point.
	// If it does not, you should return the normal of the triangle's supporting plane.
	// 
	const Vec3d& a = parent->vertices[ids[0]];
	const Vec3d& b = parent->vertices[ids[1]];
	const Vec3d& c = parent->vertices[ids[2]];

	// Calculate plane properties.
	Vec3d planeNormal = (b - a) ^ (c - a);
	if (planeNormal.length2() == 0)
		return false;

	planeNormal.normalize();

	double planeD = planeNormal * a;
	double denom = planeNormal * r.getDirection();
	if (denom == 0)
		return false;
	
	// Calculate ray t value and intersection point q.
	double t = (planeD - (planeNormal * r.getPosition())) / denom;

	if (t <= RAY_EPSILON)
		return false;

	Vec3d q = r.at(t);
	double abq = ((b - a) ^ (q - a)) * planeNormal;
	double qbc = ((c - b) ^ (q - b)) * planeNormal;
	double aqc = ((a - c) ^ (q - c)) * planeNormal;

	// Check if q intersects with triangle ABC
	if (aqc < 0 || qbc < 0 || abq < 0)
		return false;

	double abc = ((b - a) ^ (c - a)) * planeNormal;

	double alpha = qbc / abc;
	double beta = aqc / abc;
	double gamma = abq / abc;

	Vec3d surfaceNormal = planeNormal;
	// Get the surface normal
	if (parent->hasPerVertexNormals())
	{
		const Vec3d& normalA = parent->normals[ids[0]];
		const Vec3d& normalB = parent->normals[ids[1]];
		const Vec3d& normalC = parent->normals[ids[2]];


		Vec3d normalQ = alpha * normalA + beta * normalB + gamma * normalC;
		normalQ.normalize();
		surfaceNormal = normalQ;
	}

	i.setN(surfaceNormal);
	i.setT(t);
	i.setObject(this);
	
	// Interpolate material properties.
	if (parent->materials.empty())
		i.setMaterial(getMaterial());
	else
	{
		auto* matA = parent->materials[ids[0]];
		auto* matB = parent->materials[ids[1]];
		auto* matC = parent->materials[ids[2]];

		// TODO(nadavash):MEMORY LEAK HERE POSSIBLE. FIX!
		Material* interpolated = new Material(alpha * (*matA));
		*interpolated += beta * (*matB);
		*interpolated += gamma * (*matC);
		i.material = interpolated;
	}

	// Interpolate texture coordinates
	if (!parent->textureuvs.empty())
	{
		auto texA = parent->textureuvs[ids[0]];
		auto texB = parent->textureuvs[ids[1]];
		auto texC = parent->textureuvs[ids[2]];

		Vec2d textureUv = texA * alpha + texB * beta + texC * gamma;
		i.setUVCoordinates(textureUv);
	}

	return true;
}


void
Trimesh::generateNormals()
// Once you've loaded all the verts and faces, we can generate per
// vertex normals by averaging the normals of the neighboring faces.
{
    int cnt = vertices.size();
    normals.resize( cnt );
    int *numFaces = new int[ cnt ]; // the number of faces assoc. with each vertex
    memset( numFaces, 0, sizeof(int)*cnt );
    
    for( Faces::iterator fi = faces.begin(); fi != faces.end(); ++fi )
    {
        Vec3d a = vertices[(**fi)[0]];
        Vec3d b = vertices[(**fi)[1]];
        Vec3d c = vertices[(**fi)[2]];
        
        Vec3d faceNormal = ((b-a) ^ (c-a));
		faceNormal.normalize();
        
        for( int i = 0; i < 3; ++i )
        {
            normals[(**fi)[i]] += faceNormal;
            ++numFaces[(**fi)[i]];
        }
    }

    for( int i = 0; i < cnt; ++i )
    {
        if( numFaces[i] )
		{
            normals[i]  /= numFaces[i];
			if (normals[i].length() != 0)
			{
				normals[i].normalize();
			}
		}
    }

    delete [] numFaces;
}

