//
//  Main.cpp
//

#include <string>
#include "GetGlut.h"
#include "DataStructure.h"
#include "SPH.h"

using namespace std;

// prototypes
void initDisplay();
void keyboard(unsigned char key, int x, int y);
void update();
void reshape(int w, int h);
void display();

//declare global variables here
SPH sph;
int winX = 600;
int winY = 600;

int main (int argc, char** argv)
{
	glutInitWindowSize(winX, winY);
	glutInitWindowPosition(0, 0);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("SPH Fluid 2D BINGYANG LIU");
	glutKeyboardFunc(keyboard);
	glutIdleFunc(update);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	initDisplay();

	glutMainLoop();

	return 1;
}

void initDisplay()
{
	sph.Init_Fluid();
	glMatrixMode(GL_PROJECTION);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0);
}

void DrawParticles(){
	Particle *p = sph.Get_Paticles();
	glColor3f(1.0f, 0.0f, 1.0f);
	glPointSize(5.0f);

	glBegin(GL_POINTS);
	for(int i=0; i<sph.Get_Particle_Number(); i++)
		{
			glVertex2f(p[i].pos.x, p[i].pos.y);
		}
	glEnd();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // on [ESC]
		exit(0); // normal exit
		break;
	}
}

void update()
{
	//update your variables here
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glLoadIdentity();
	gluOrtho2D(0.0, sph.Get_World_Size().x, 0.0, sph.Get_World_Size().y);
	glutReshapeWindow(w, h);
}

void display (void)
{
	sph.Animation();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// clear the screen - any drawing before here will not display
	DrawParticles();
	// send the current image to the screen - any drawing after here will not display
	glutSwapBuffers();


}


