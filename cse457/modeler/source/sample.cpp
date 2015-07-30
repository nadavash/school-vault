// The sample box model.  You should build a file
// very similar to this for when you make your model in
// order to plug in to the animator project.

#pragma warning (disable : 4305)
#pragma warning (disable : 4244)
#pragma warning (disable : 4786)
#pragma warning (disable : 4312)


#include "modelerview.h"
#include "modelerdraw.h"
#include "modelerui.h"
#include "vault.h"
#include "FL/glut.h"
#include "model.h"
#include <cmath>
#include <time.h>
#include "prayingmantismodel.h"


/** The scene, which includes the lights and models. */


class Scene : public Model {
protected:
///////////////////////////////// TEXTURES ////////////////////////////////////
	Texture2D texture;
	Texture2D bumpMap;

///////////////////////////////// SHADERS /////////////////////////////////////
	ShaderProgram shader;
	ShaderProgram shaderWithTexture;
	ShaderProgram cartoonShader;
	ShaderProgram cloudShader;
	ShaderProgram bumpShader;

//////////////////////////////// PROPERTIES ///////////////////////////////////
	// Switches for spheres
	BooleanProperty useTexture;
	// Switch for showing/hiding reference unit sphere
	BooleanProperty showReferenceUnitSphere;
	// Switch to drawn normals on revolution
	RangeProperty showNormalsOnRevolution;

	// Lets you pick what shapes to use for the default model!
	ChoiceProperty shapeChoice;

	ChoiceProperty shaderChoice;

	// Some slider properties
	RangeProperty rotateX, rotateY;
	RangeProperty brightness;

	// Diffuse color picker
	RGBProperty diffuse;


	// Scene lights
	PointLight pointLight;
	DirectionalLight directionalLight;

	PrayingMantisModel prayingMantis;
	float tickVal;

public:
	
	

	/** Construct the scene */
	Scene() :
		// You have to call the parent class's constructor, to provide a
		// name for the model.
		Model("Scene"),

		// Construct textures and shaders. 
		// They won't be loaded until the model is drawn for the first time.
		texture("checkers.png"),
		bumpMap("leather-normal.bmp"),
		shader("shader.vert", "shader.frag", NULL),
		shaderWithTexture("shaderwithtexture.vert", "shaderwithtexture.frag", NULL),
		cartoonShader("cartoon.vert", "cartoon.frag", NULL),
		cloudShader("clouds.vert", "clouds.frag", NULL),
		bumpShader("bump.vert", "bump.frag", NULL),
		prayingMantis(),

		// Call the constructors for the lights
		pointLight("Point Light", GL_LIGHT1, /**direction part**/ -5, 5, 5, /**diffuse part**/ 1.0, 0.5, 0.5,
		/**specular part**/ 1.0, 0.5, 0.5, /**ambient part**/ .2f, 0.1, 0.1 /**attenuation part**/, 0.4, 0.7, 0),
		directionalLight("Directional Light", GL_LIGHT0, /**direction part**/ 5, 5, 5, /**diffuse part**/ 0.0f, 0.5, 0.5f,
		/**specular part**/ 0.0f, 0.5f, 0.5f)


		// Now, call the constructors for each Property:
		, useTexture("Use Checkered Texture", true),
		showReferenceUnitSphere("Show Reference Unit Sphere", false),
		showNormalsOnRevolution("Size of Normals on Revolution", 0, 1, 0.0, 0.1),
		shapeChoice("Model Shape:", "Sphere|Cube|Cylinder|Torus|Icosahedron|Teapot|Revolution|Praying Mantis", 0), //457 animator UI only allows shapes
		shaderChoice("Shader To Use:", "None|Default|Default with texture|Cartoon|Clouds|Bum mapping", 0),
		rotateX("Rotate Basic Shape X", -180, 180, 0, 1),
		rotateY("Rotate Basic Shape Y", -180, 180, 0, 1),
		brightness("Brightness", 0.0f, 1.0f, 1.0f, 0.1f),
		diffuse("Diffuse Color", 1.0, 0.7, .4)
	{
		// If you have child Models, like the MobileLight model from model.h,
		// you can add their property groups, and they will appear in the list
		// in the top left corner of Modeler, under this model's entry:
		properties.add(pointLight.getProperties())
				  .add(directionalLight.getProperties())
				  .add(prayingMantis.getProperties())
				  ;

		// Finally, add all the properties to this model's PropertyGroup.
		properties.add(&useTexture);
		properties.add(&showReferenceUnitSphere)
			      .add(&showNormalsOnRevolution)
				  .add(&shapeChoice);
		properties.add(&shaderChoice)
				.add(&rotateX)
				.add(&rotateY);
		properties.add(&brightness)
				  .add(&diffuse)
				  ;
		tickVal = 0;
	}



	/**
	 * Modeler calls this method once an OpenGL context becomes available,
	 * (which happens before the first time draw() is called), and whenever
	 * you click the "Reload Textures and Shaders" menu option.
	 *
	 * Make sure you call the load() methods of each of your textures and
	 * shaders here.
	 */
	void load() {
		texture.load();
		bumpMap.load();
		shader.load();
		shaderWithTexture.load();
		cartoonShader.load();
		cloudShader.load();
		bumpShader.load();
	}

	/**
	 * Modeler calls this method many times a second when the Animate
	 * checkbox is checked.
	 */
	void tick() {
		// You can put code here to increment animation counters for
		// extra credit.
		prayingMantis.tick();
		tickVal += 0.01;
	}
	
	/** Draw the model. */
	void drawModel() {

		int divisions=16; 		
        switch(ModelerDrawState::Instance()->m_quality)
        {
        case HIGH: 
			divisions = 64; break;
		case MEDIUM: 
			divisions = 20; break;
		case LOW:
			divisions = 12; break;
		case POOR:
			divisions = 8; break;
        }

		// Remember our current modelview matrix, which is in world space.
		glPushMatrix();

		    // Rotate around the X-axis
			glRotatef(rotateX.getValue(), 1, 0, 0);

			// Rotate around the Y-axis
			glRotatef(rotateY.getValue(), 0, 1, 0);


			// Draw some object at the origin.
			switch (shapeChoice.getValue()) {
			case 0: // sphere
				glRotatef(-90.f,1.f,0.f,0.f);
				glutSolidSphere(1.0, divisions, divisions);
				glRotatef(90.f,1.f,0.f,0.f);
				break;
			case 1: // box
				glRotatef(-45.f,0.f,1.f,0.f);
				glutSolidCube(2);
				glRotatef(45.f,0.f,1.f,0.f);
				break;
			case 2: // cylinder
				glRotatef(90.f,1.f,0.f,0.f);
				drawCylinder(1.8,1.0,1.8);
				glRotatef(-90.f,1.f,0.f,0.f);
				break;
			case 3: // torus
				glutSolidTorus(.5,1,divisions,divisions);
				break;
			case 4: // icosahedron
				glutSolidIcosahedron();
				break;
			case 5: // teapot
				glutSolidTeapot(1);
				break;
			case 6:
				drawRevolution(1, showNormalsOnRevolution.getValue());
				break;
			case 7:
				prayingMantis.draw();
				break;
			}

			if (showReferenceUnitSphere.getValue() ) {
			    // Now go 1 units in the +y direction,
				// and 1 units in the NEGATIVE x direction.
				glTranslated(-2, -1, 0);
			    // Now go 1 units in the +y direction,
				// and 1 units in the NEGATIVE x direction.
					glRotatef(-90.f,1.f,0.f,0.f);
					drawSphere(.5);
					glRotatef(90.f,1.f,0.f,0.f);
				glPopMatrix();
			}
			
			// Go back to "world space"
			glPopMatrix();

		{

			// Draw the floor.  Here's a basic example of texture mapping.
			glBegin(GL_QUADS);
			glTexCoord2f(0, 1); // specify the texture coordinate
			glNormal3f(0, 1, 0); // specify the surface's normal at this vertex
			glVertex3f(-10, -2, -10); // both before its corresponding vertex

			glTexCoord2f(1, 1);
			glNormal3f(0, 1, 0);
			glVertex3f(10, -2, -10);

			glTexCoord2f(1, 0);
			glNormal3f(0, 1, 0);
			glVertex3f(10, -2, 10);

			glTexCoord2f(0, 0);
			glNormal3f(0, 1, 0);
			glVertex3f(-10, -2, 10);
			glEnd();
		}
	}


	/** Draw the scene. */
	void draw() {
		// The lights must be drawn FIRST, so the other scene elements
		// can get lit!
		pointLight.draw();
		directionalLight.draw();
		//reset colors
		setDiffuseColor(
			diffuse.getRed(),
			diffuse.getGreen(),
			diffuse.getBlue());

		setAmbientColor(0.2, 0.2, 0.2);

		setSpecularColor(1,1,1);
		setShininess(pow(2.71828183f, 3));

		// Use the texture if desired.
		if (useTexture.getValue()) {
			texture.use();
		} else {
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		//// Use the shader if desired.
		GLint brightnessVariableLocation;
		GLfloat tickVariableLocation;
		switch (shaderChoice.getValue()) {
		case 0:
			if (glUseProgram) { glUseProgram(0); } // don't use shader
			break;
		case 1:
			shader.use();
			// glGetUniformLocation gets the memory location of a variable with the given char* name, in this case "brightness"
			// for the given shader program, identified by its ID.
		    brightnessVariableLocation = glGetUniformLocation( shader.getID(), "brightness" );
			// glUniform1f sets the value of a particular uniform variable location with a single float value (hence the suffix "1f")
			glUniform1f(brightnessVariableLocation, brightness.getValue());
			break;
		case 2:
			shaderWithTexture.use();
			brightnessVariableLocation = glGetUniformLocation(shaderWithTexture.getID(), "brightness");
			glUniform1f(brightnessVariableLocation, brightness.getValue());
			break;
		case 3:
			cartoonShader.use();
			break;
		case 4:
			cloudShader.use();
			tickVariableLocation = glGetUniformLocation(cloudShader.getID(), "tick");
			glUniform1f(tickVariableLocation, tickVal);
			break;
		case 5:
			bumpMap.use();
			bumpShader.use();
			brightnessVariableLocation = glGetUniformLocation(bumpShader.getID(), "brightness");
			glUniform1f(brightnessVariableLocation, brightness.getValue());
			break;
		}


		// Call a class method that draws our model.
		drawModel();
		// Stop applying shaders to objects.
		// This if-statement makes sure that glUseProgram is not a null
		// function pointer (which it will be if GLEW couldn't initialize).
		if (glUseProgram) {
			glUseProgram(0);
		}

		// Stop applying textures to objects
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

/**
 * The program starts here.
 */

int main()
{
	// Tell the FLTK library to let us use OpenGL
	Fl::gl_visual(FL_RGB);

	init_load_curve_file();

	// Instantiate Modeler
	ModelerUserInterface ui;

	// Give Modeler your scene.
	// Modeler will free this pointer when it exits.
	ui.setModel(new Scene());


	// Run the application
    return ui.run();
}