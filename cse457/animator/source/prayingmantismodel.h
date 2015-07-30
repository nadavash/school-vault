#ifndef PRAYINGMANTISMODEL_H
#define PRAYINGMANTISMODEL_H

#include "model.h"
#include "modelerview.h"
#include "modelerdraw.h"
#include "FL/glut.H"
#include "vault.h"

class PrayingMantisModel : public Model {
public:
	PrayingMantisModel();

	void load() override;
	void tick() override;
	void draw() override;
	ParticleSystem* getParticleSystem() override { return &particleSystem_; }

	ICamera* getCamera() override;

private:
	void drawHead(Mat4f cam);
	void drawLegs(float legRotation);
	void drawArms(float rightArmStretch, float leftArmStretch, Mat4f camera);
	void spawnParticles(Mat4f cameraTransforms, Vec3d posOffset = Vec3d(0), Vec3d velOffset = Vec3d(0));

protected:
	Texture2D texture_;

	ShaderProgram shader_;
	ParticleSystem particleSystem_;

	RangeProperty scale_;
	RangeProperty rightArmStretch_;
	RangeProperty leftArmStretch_;
	RangeProperty legRotation_;

	bool upTick_;
};

#endif  // PRAYINGMANTISMODEL_H