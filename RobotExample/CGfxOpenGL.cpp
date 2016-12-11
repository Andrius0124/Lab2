#ifdef _WINDOWS
#include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include "CGfxOpenGL.h"
#include "Robot.h"

// disable implicit float-double casting
#pragma warning(disable:4305)

CGfxOpenGL::CGfxOpenGL()
{
}

CGfxOpenGL::~CGfxOpenGL()
{
}

bool CGfxOpenGL::Init()
{	
	theRobot = new Robot;
	
	rotationAngle = 0.0f;
	moveSpeed = 5.0f;
	fPositionX = fPositionY  = 0;
	fPositionZ = -30;
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	return true;
}

bool CGfxOpenGL::Shutdown()
{
	delete theRobot;

	return true;
}

void CGfxOpenGL::SetupProjection(int width, int height)
{
	if (height == 0)					// don't want a divide by zero
	{
		height = 1;					
	}

	glViewport(0, 0, width, height);		// reset the viewport to new dimensions
	glMatrixMode(GL_PROJECTION);			// set projection matrix current matrix
	glLoadIdentity();						// reset projection matrix

	// calculate aspect ratio of window
	gluPerspective(52.0f,(GLfloat)width/(GLfloat)height,1.0f,1000.0f);

	glMatrixMode(GL_MODELVIEW);				// set modelview matrix
	glLoadIdentity();						// reset modelview matrix

	m_windowWidth = width;
	m_windowHeight = height;
}

void CGfxOpenGL::Prepare(float dt)
{
	

	theRobot->Prepare(dt);
}
void CGfxOpenGL::TurnRight(float dt)
{
	rotationAngle += 25.f * dt;
	if (rotationAngle > 25.f)
		rotationAngle = 25.f;
	theRobot->Prepare(dt);
}
void CGfxOpenGL::TurnLeft(float dt)
{
	rotationAngle -= 25.f * dt;
	if (rotationAngle < -90.f)
		rotationAngle = -90.f;
	theRobot->Prepare(dt);
	
}
void CGfxOpenGL::MoveForward(float dt)
{
	fPositionZ -= cos(rotationAngle*PI/180)* moveSpeed *dt;
	fPositionX -= sin(rotationAngle*PI / 180)* moveSpeed*dt;
	theRobot->Prepare(dt);
}
void CGfxOpenGL::MoveBackward(float dt)
{
	fPositionZ += cos(rotationAngle*PI / 180)* moveSpeed *dt;
	fPositionX += sin(rotationAngle*PI / 180)* moveSpeed*dt;
	theRobot->Prepare(dt);
}

void CGfxOpenGL::Render()
{
	// clear screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// load the identity matrix (clear to default position and orientation)
	glLoadIdentity();

	glPushMatrix();							// put current matrix on stack
		glLoadIdentity();					// reset matrix
		glTranslatef(fPositionX, fPositionY, fPositionZ);	// move to (0, 0, -30)
		glRotatef(rotationAngle,0 ,  1.0f,0);	// rotate the robot on its y-axis
		theRobot->DrawRobot(0,0, 0);		// draw the robot
	glPopMatrix();		
}
