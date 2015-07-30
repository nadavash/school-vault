#include "prayingmantismodel.h"

const GLdouble EYE_SIZE = 0.25;

static Mat4f getModelViewMatrix();

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
	properties.add(&particleSystem_.restitution_);
	properties.add(&particleSystem_.airResistance_);
	properties.add(&particleSystem_.friction_);
}

void PrayingMantisModel::load() 
{
	shader_.load();
	texture_.load();
}

void PrayingMantisModel::tick() 
{
	float legRot = legRotation_.getValue();

	legRot += upTick_ ? 2 : -2;
	if (legRot >= 30 || legRot <= -30) {
		upTick_ = !upTick_;
	}

	legRotation_.setValue(legRot);
}

void PrayingMantisModel::draw() 
{
	float scale = scale_.getValue();
	float rightArmStretch = rightArmStretch_.getValue();
	float leftArmStretch = leftArmStretch_.getValue();
	float legRot = legRotation_.getValue();

	Mat4f camera = getModelViewMatrix();

	glPushMatrix();
	glTranslatef(0, -2, 4);
	drawSphere(1);
	glPopMatrix();

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
			drawHead(camera);

			// Draw arms
			drawArms(rightArmStretch, leftArmStretch, camera);
		glPopMatrix();
	glPopMatrix();
}

ICamera* PrayingMantisModel::getCamera() 
{
	return nullptr;
}

void PrayingMantisModel::spawnParticles(Mat4f cameraTransforms, Vec3d posOffset, Vec3d velOffset)
{
	Mat4f currentModelView = getModelViewMatrix();
	Mat4f worldMat = cameraTransforms.inverse() * currentModelView;
	Vec3f pos = worldMat * (Vec4f(posOffset[0], posOffset[1], posOffset[2], 1));
	double velMag = velOffset.length();
	Vec3f vel = worldMat * (Vec4f(velOffset[0], velOffset[1], velOffset[2], 1));
	vel.normalize();
	vel *= velMag;
	particleSystem_.addParticle(Vec3d(pos[0], pos[1], pos[2]), Vec3d(vel[0], vel[1], vel[2]));
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

void PrayingMantisModel::drawHead(Mat4f cam) 
{
	glPushMatrix();
		glTranslatef(0, 0, 2);
		glRotatef(40, 1, 0, 0);
		drawCylinder(0.25, 0.3, 0.5);
		spawnParticles(cam, Vec3d(0, 0.25, -0.1), Vec3d(0,0,5));

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

void PrayingMantisModel::drawLegs(float legRot) {
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

void PrayingMantisModel::drawArms(float rightArmStretch, float leftArmStretch, Mat4f cameraTransform) {
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
					glTranslatef(0, 0, 0.7);
					spawnParticles(cameraTransform); // SPAWN PARTICLES!!!
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
					glTranslatef(0, 0, 0.7);
					spawnParticles(cameraTransform); // SPAWN PARTICLES!!!
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}