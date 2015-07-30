#include "prayingmantismodel.h"

const GLdouble EYE_SIZE = 0.25;

static void drawHead();
static void drawLegs(float legRotation);
static void drawArms(float rightArmStretch, float leftArmStretch);

PrayingMantisModel::PrayingMantisModel()
	: Model("Praying Mantis"),
	shader_("shader.vert", "shader.frag", NULL),
	texture_("checkers.png"),
	scale_("Scale", 0.1f, 2.f, 1.f, 0.1f),
	rightArmStretch_("Right Arm Stretch", 0, 50, 0, 1),
	leftArmStretch_("Left Arm Stretch", 0, 50, 0, 1),
	legRotation_("Leg Rotation", -30, 30, 0, 1),
	upTick_(false)
{
	properties.add(&scale_);
	properties.add(&rightArmStretch_);
	properties.add(&leftArmStretch_);
	properties.add(&legRotation_);
}

void PrayingMantisModel::load() {
	shader_.load();
	texture_.load();
}

void PrayingMantisModel::tick() {
	float legRot = legRotation_.getValue();

	legRot += upTick_ ? 2 : -2;
	if (legRot >= 30 || legRot <= -30) {
		upTick_ = !upTick_;
	}

	legRotation_.setValue(legRot);
}

void PrayingMantisModel::draw() {
	float scale = scale_.getValue();
	float rightArmStretch = rightArmStretch_.getValue();
	float leftArmStretch = leftArmStretch_.getValue();
	float legRot = legRotation_.getValue();

	// Draw the body
	glPushMatrix();
		glRotatef(-25, 1, 0, 0);
		glScalef(scale, scale, scale);
		drawCylinder(2.5, 0.5, 0.3);
		drawLegs(legRot);

		// Draw upper body
		glPushMatrix();
			glTranslatef(0, 0, 2.5);
			glRotatef(-20, 1, 0, 0);
			drawCylinder(2, 0.3, 0.3);
			
			// Draw head
			drawHead();

			// Draw arms
			drawArms(rightArmStretch, leftArmStretch);
		glPopMatrix();
	glPopMatrix();
}

ICamera* PrayingMantisModel::getCamera() {
	return nullptr;
}

static void drawHead() {
	glPushMatrix();
		glTranslatef(0, 0, 2);
		glRotatef(40, 1, 0, 0);
		drawCylinder(0.25, 0.3, 0.5);

		glPushMatrix();
			glTranslatef(0, 0.1, 0.25);
			glPushMatrix();
				glTranslatef(EYE_SIZE * 2, 0, 0);
				drawSphere(EYE_SIZE);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-EYE_SIZE * 2, 0, 0);
				drawSphere(EYE_SIZE);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

static void drawLegs(float legRot) {
	// Draw upper legs
	glPushMatrix();
		glTranslatef(0.f, -0.2, 1.75);
		glRotatef(45, 1, 0, 0);

		glRotatef(-20, 1, 0, 0);
		glRotatef(-75 + legRot, 0, 1, 0);
		glRotatef(20, 1, 0, 0);

		drawCylinder(1, 0.1, 0.1);
			
		glPushMatrix();
			glTranslatef(0, 0, 1);
			glRotatef(60, 1, 0, 0);
			drawCylinder(1, 0.1, 0.1);
		glPopMatrix();

	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.f, -0.2, 1.75);
		glRotatef(45, 1, 0, 0);

		glRotatef(-20, 1, 0, 0);
		glRotatef(75 + legRot, 0, 1, 0);
		glRotatef(20, 1, 0, 0);

		drawCylinder(1, 0.1, 0.1);

		glPushMatrix();
			glTranslatef(0, 0, 1);
			glRotatef(60, 1, 0, 0);
			drawCylinder(1, 0.1, 0.1);
		glPopMatrix();

	glPopMatrix();

	// Draw lower
	glPushMatrix();
		glTranslatef(0.f, 0.2f, 0.75);
		glRotatef(45, 1, 0, 0);

		glRotatef(-20, 1, 0, 0);
		glRotatef(-120 - legRot, 0, 1, 0);
		glRotatef(20, 1, 0, 0);

		drawCylinder(1, 0.1, 0.1);

		glPushMatrix();
			glTranslatef(0, 0, 1);
			glRotatef(60, 1, 0, 0);
			drawCylinder(1, 0.1, 0.1);
		glPopMatrix();

	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.f, 0.2f, 0.75);
		glRotatef(45, 1, 0, 0);

		glRotatef(-20, 1, 0, 0);
		glRotatef(120 - legRot, 0, 1, 0);
		glRotatef(20, 1, 0, 0);

		drawCylinder(1, 0.1, 0.1);

		glPushMatrix();
			glTranslatef(0, 0, 1);
			glRotatef(60, 1, 0, 0);
			drawCylinder(1, 0.1, 0.1);
		glPopMatrix();

	glPopMatrix();
}

static void drawArms(float rightArmStretch, float leftArmStretch) {
	glPushMatrix();
		glTranslatef(0, 0, 1.5);
		glPushMatrix();
			glRotatef(135 - leftArmStretch, 1, 0, 0);
			glTranslatef(0.4, 0, 0);
			drawCylinder(1, 0.15, 0.15);
			glPushMatrix();
				glTranslatef(0, 0, 1);
				glRotatef(-135 + leftArmStretch * 1.5f, 1, 0, 0);
				drawCylinder(1, 0.15, 0.15);
				glPushMatrix();
					glTranslatef(0, 0, 1);
					glRotatef(150 - leftArmStretch * 2, 1, 0, 0);
					drawCylinder(0.7, 0.15, 0.15);
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
		glPushMatrix();
			glRotatef(135 - rightArmStretch, 1, 0, 0);
			glTranslatef(-0.4, 0, 0);
			drawCylinder(1, 0.15, 0.15);
			glPushMatrix();
				glTranslatef(0, 0, 1);
				glRotatef(-135 + rightArmStretch * 1.5f, 1, 0, 0);
				drawCylinder(1, 0.15, 0.15);
				glPushMatrix();
					glTranslatef(0, 0, 1);
					glRotatef(150 - rightArmStretch * 2, 1, 0, 0);
					drawCylinder(0.7, 0.15, 0.15);
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}