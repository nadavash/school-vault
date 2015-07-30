#pragma warning(disable : 4786)

#include "particleSystem.h"
#include "modelerui.h"


#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cmath>
//#include <limits>
#include "properties.h"

#define NUM_PARTICLES 3500
#define DISTANCE_EPSILON 0.5
#define MAX_LIFETIME 3.5

using namespace std;


static float prevT;
static Mat4f getModelViewMatrix();


/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem()
	: position_(0, 10, 0), prevT_(0),
	restitution_("Restitution", 0.0f, 2.0f, 1.0f, 0.1f),
	airResistance_("Air Resistance", 0, 1.0f, 0.5f, 0.05f),
	friction_("Friction", 0, 1.0f, 0.9f, 0.025f),
	gravity_(Vec3d(0, -9.8, 0)),
	airDrag_(0.1),
	particleTexture_("leather-normal.bmp")
{
	num_particles_ = NUM_PARTICLES;
	// Leave these here; the UI needs them to work correctly.
	dirty = false;
	simulate = false;
	particleTexture_.load();
}




/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
	this->prevT_ = t;
	// These values are used by the UI ...
	// negative bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
	// YOUR CODE HERE
	this->prevT_ = t;
	this->particles_.clear();

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
	this->prevT_ = t;
	this->particles_.clear();

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{
	if (!simulate)
		return;
	float dt = t - this->prevT_;

	while (!particles_.empty() && particles_[0].lifeTime_ > MAX_LIFETIME)
	{
		particles_.erase(particles_.begin(), particles_.begin() + 1);
	}

	airDrag_.kd_ = airResistance_.getValue();

	for (int i = 0; i < particles_.size(); ++i)
	{
		Particle p = particles_[i];
		p.lifeTime_ += dt;
		p.force_ = 0;
		gravity_.apply(&p);
		airDrag_.apply(&p);
		p.velocity_ += dt * p.force_ / p.mass_;
		p.position_ += dt * p.velocity_;
		collision(&p);
		particles_[i] = p;
	}

	// Debugging info
	/*if( t - prevT > .08 )
		printf("(!!) Dropped Frame %lf (!!)\n", t-prevT);*/
	this->prevT_ = prevT = t;
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	if (!simulate)
		return;

	particleTexture_.use();

	for (const auto& p : particles_) 
	{
		glPushMatrix();
			glTranslated(p.position_[0], p.position_[1], p.position_[2]);
				GLfloat modelView[16];
				glGetFloatv(GL_MODELVIEW_MATRIX, modelView);
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						if (i == j)
							modelView[i * 4 + j] = 1.0;
						else
							modelView[i * 4 + j] = 0.0;
					}
				}
				glLoadMatrixf(modelView);

				glScaled(0.1, 0.1, 0.1);

				glBegin(GL_QUADS);
				glTexCoord2d(0, 1);
				glVertex3d(-1, 1, 0);

				glTexCoord2d(1, 1);
				glVertex3d(1, 1, 0);

				glTexCoord2d(1, 0);
				glVertex3d(1, -1, 0);

				glTexCoord2d(0, 0);
				glVertex3d(-1, -1, 0);

				glEnd();

		glPopMatrix();
	}
}

void ParticleSystem::addParticle(Vec3d position, Vec3d initVelocity)
{
	if (!simulate || particles_.size() >= NUM_PARTICLES)
		return;

	Particle p;
	p.position_ = Vec3d(rand() % 1000 - 500, rand() % 1000 - 500, rand() % 1000 - 500);
	p.position_.normalize();
	p.position_ /= 20;

	p.position_ += position;
	//p.velocity_ = Vec3d(rand() % 1000 - 500, rand() % 1000 - 500, rand() % 1000 - 500);
	//p.velocity_.normalize();
	p.velocity_ += initVelocity;
	particles_.push_back(p);
}


/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{
	// TODO (baking is extra credit)
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{
	// TODO (baking is extra credit)
}

void ParticleSystem::initializeParticles()
{
	for (int i = 0; i < NUM_PARTICLES; ++i)
	{
		Particle p = particles_[i];
		p.position_ = position_;
		p.velocity_ = Vec3d(rand() % 1000 - 500, rand() % 1000 - 500, rand() % 1000 - 500);
		p.velocity_.normalize();
		particles_[i] = p;
	}
}

void ParticleSystem::collision(Particle* p)
{
	Vec3d planeNormal(0, 1, 0);
	Vec3d planePosition(0, -2, 0);

	double velNormal = p->velocity_ * planeNormal;
	if ((p->position_ - planePosition) * planeNormal < DISTANCE_EPSILON &&
		 velNormal < 0)
	{
		Vec3d vNorm = (planeNormal * p->velocity_) * planeNormal;
		Vec3d vTan = p->velocity_ - vNorm;
		p->velocity_ = friction_.getValue() * vTan - restitution_.getValue() * vNorm;
		p->position_[1] = -2;
	}

	// Sphere collision
	Vec3d spherePos = Vec3d(0, -2, 4);
	double rad = 1.;

	Vec3d norm = (p->position_ - spherePos);
	if (norm.length() < rad + DISTANCE_EPSILON &&
		p->velocity_ * norm < 0) // Colliding with sphere
	{
		norm.normalize();
		Vec3d vNorm = (norm * p->velocity_) * norm;
		Vec3d vTan = p->velocity_ - vNorm;
		p->velocity_ = vTan - restitution_.getValue() * vNorm;
		p->position_ = spherePos + norm * rad;
	}
}

static Mat4f getModelViewMatrix()
{
	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	Mat4f matMV(m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]);
	return matMV.transpose();
}
