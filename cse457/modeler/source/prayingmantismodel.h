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

	ICamera* getCamera() override;

protected:
	Texture2D texture_;

	ShaderProgram shader_;

	RangeProperty scale_;
	RangeProperty rightArmStretch_;
	RangeProperty leftArmStretch_;
	RangeProperty legRotation_;
	bool upTick_;
};

#endif  // PRAYINGMANTISMODEL_H