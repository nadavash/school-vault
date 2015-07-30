/***********************
 * ParticleSystem class
 ***********************/

/**
 * The particle system class simply "manages" a collection of particles.
 * Its primary responsibility is to run the simulation, evolving particles
 * over time according to the applied forces using Euler's method.
 * This header file contains the functions that you are required to implement.
 * (i.e. the rest of the code relies on this interface)
 * In addition, there are a few suggested state variables included.
 * You should add to this class (and probably create new classes to model
 * particles and forces) to build your system.
 */

#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#pragma warning(disable : 4018)
#pragma warning(disable : 4267)
#pragma warning(disable : 4311)
#pragma warning(disable : 4312)
#pragma warning(disable : 4244)
#pragma warning(disable : 4305)

#include <memory>

#include "vec.h"
#include "properties.h"
#include "vault.h"


struct Particle
{
	Particle() : mass_(1.f), position_(0), velocity_(0), force_(0), lifeTime_(0) { }
	float mass_;
	Vec3d position_;
	Vec3d velocity_;
	Vec3d force_;
	float lifeTime_;
};

class Force
{
public:
	virtual void apply(Particle* p) = 0;
};

class GravityForce : public Force
{
public:
	Vec3d f_;

	GravityForce(Vec3d gravity) : f_(gravity) {}

	void apply(Particle* p) override
	{
		p->force_ += p->mass_ * f_;
	}
};

class ViscousDragForce : public Force
{
public:
	float kd_;

	ViscousDragForce(float kd) : kd_(kd) {}

	void apply(Particle* p) override
	{
		p->force_ -= kd_ * p->velocity_;
	}
};

class ParticleSystem {
public:
	static const float Gravity;

	/** Constructor **/
	ParticleSystem();

	/** Destructor **/
	virtual ~ParticleSystem();

	/** Simulation fxns **/
	// This fxn should render all particles in the system,
	// at current time t.
	virtual void drawParticles(float t);

	// This fxn should save the configuration of all particles
	// at current time t.
	virtual void bakeParticles(float t);

	// This function should compute forces acting on all particles
	// and update their state (pos and vel) appropriately.
	virtual void computeForcesAndUpdateParticles(float t);

	// This function should reset the system to its initial state.
	// When you need to reset your simulation, PLEASE USE THIS FXN.
	// It sets some state variables that the UI requires to properly
	// update the display.  Ditto for the following two functions.
	virtual void resetSimulation(float t);

	// This function should start the simulation
	virtual void startSimulation(float t);

	// This function should stop the simulation
	virtual void stopSimulation(float t);

	// This function should clear out your data structure
	// of baked particles (without leaking memory).
	virtual void clearBaked();	

	void addParticle(Vec3d position, Vec3d initVelocity = Vec3d(0));


	// These accessor fxns are implemented for you
	float getBakeStartTime() { return bake_start_time; }
	float getBakeEndTime() { return bake_end_time; }
	float getBakeFps() { return bake_fps; }
	bool isSimulate() { return simulate; }
	bool isDirty() { return dirty; }
	void setDirty(bool d) { dirty = d; }


	RangeProperty restitution_;
	RangeProperty airResistance_;
	RangeProperty friction_;

private:
	void initializeParticles();
	void collision(Particle* p);

protected:
	Vec3d position_;
	std::vector<Particle> particles_;
	int num_particles_;

	// Constant forces
	GravityForce gravity_;
	ViscousDragForce airDrag_;

	float prevT_;

	/** Some baking-related state **/
	float bake_fps;						// frame rate at which simulation was baked
	float bake_start_time;				// time at which baking started 
										// These 2 variables are used by the UI for
										// updating the grey indicator 
	float bake_end_time;				// time at which baking ended

	/** General state variables **/
	bool simulate;						// flag for simulation mode
	bool dirty;							// flag for updating ui (don't worry about this)


	Texture2D particleTexture_;
};

#endif	// __PARTICLE_SYSTEM_H__
