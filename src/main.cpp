#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "GLInclude.h"
#include "camera.h"
#include "shapes.h"

void Display(void);
void Init(void);
void Render(void);
void Lighting(void);
void InitCamera(int);
void HandleKeyboard(unsigned char key,int x, int y);
void HandleReshape(int,int);
void HandleIdle(void);
void HandleMouseMotion(int x, int y);
void HandleMousePassiveMotion(int x, int y);
void move(VECTOR3D direction);

int fullscreen = FALSE;

int currentbutton = -1;
double rotatespeed = 3;
double tSpeed = 0.05;
float t = 0;
float lastx = 0;
float lasty = 0;
float xrot = 0;
float yrot = 0;

CAMERA camera;
FRUSTUM centerFrustum;
EULER eulerCamera;



double rotateangle = 0;


int main(int argc,char **argv)
{
    camera.screenwidth = 600;
    camera.screenheight = 400;

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutCreateWindow("Geometria Proyectiva");
    if (fullscreen)
        glutFullScreen();
    glutDisplayFunc( Display);
    glutReshapeFunc(HandleReshape);
    glutReshapeWindow(camera.screenwidth,camera.screenheight);
    glutIdleFunc(HandleIdle);
    glutKeyboardFunc(HandleKeyboard);

	glutPassiveMotionFunc(HandleMousePassiveMotion);
	glutMotionFunc(HandleMouseMotion);
	
    Init();
    InitCamera(0);
    Lighting();
    
    glutMainLoop();
    return(0);
}

void Init(void)
{
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POINT_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DITHER);
    glDisable(GL_CULL_FACE);
    
    glLineWidth(1.0);
    glPointSize(1.0);
    
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glFrontFace(GL_CW);
    glClearColor(0.0,0.0,0.0,0.0);         /* Background colour */
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}

void Display(void)
{
    glDrawBuffer(GL_BACK_LEFT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    glDrawBuffer(GL_BACK);
    
	double nearValue = 0.1;
	double farValue = 10000;
    
    double aspectRatio  = camera.screenwidth / (double)camera.screenheight;
    FRUSTUM centerFrustum = makeFrustum(camera.aperture, aspectRatio, nearValue, farValue);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	//gluPerspective(camera.aperture, aspectRatio, nearValue, farValue);//QUITAR
	glFrustum(centerFrustum.left, centerFrustum.right, centerFrustum.bottom, centerFrustum.top, centerFrustum.nearValue, centerFrustum.farValue);
    
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    VECTOR3D target = Add(camera.position, camera.direction);
   // gluLookAt(camera.position.x,camera.position.y,camera.position.z, target.x , target.y, target.z, camera.up.x,camera.up.y,camera.up.z); //QUITAR
    
// TODO

	updateEulerOrientation(eulerCamera);

    MATRIX4 lookAtMatrix = lookAt(camera.position, getForward(eulerCamera), camera.up);

    glLoadMatrixf(lookAtMatrix.m);

    glViewport(0,0,camera.screenwidth,camera.screenheight);
    
    Render();
    
    glutSwapBuffers();
}


void Render(void)
{
    GLfloat specular[4] = {1.0,1.0,1.0,1.0};
    GLfloat shiny[1] = {5.0};
    
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    
    glPushMatrix();
    glRotatef(rotateangle,0.0,1.0,0.0);

    drawAxis();
    
    glPopMatrix();
}

void Lighting(void)
{
    GLfloat fullambient[4] = {1.0,1.0,1.0,1.0};
    GLfloat position[4] = {0.0,0.0,0.0,0.0};
    GLfloat ambient[4]  = {0.2,0.2,0.2,1.0};
    GLfloat diffuse[4]  = {1.0,1.0,1.0,1.0};
    GLfloat specular[4] = {0.0,0.0,0.0,1.0};
    
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);
    glDisable(GL_LIGHT4);
    glDisable(GL_LIGHT5);
    glDisable(GL_LIGHT6);
    glDisable(GL_LIGHT7);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,fullambient);
    glLightfv(GL_LIGHT0,GL_POSITION,position);
    glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
    glEnable(GL_LIGHT0);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
}

void HandleKeyboard(unsigned char key,int x, int y)
{
    switch (key) {
        case ESC:
        case 'Q':
        case 'q':
            exit(0);
            break;
        case 'R':
        case 'r':
            rotateangle += rotatespeed;
            break;
            
        case '+':
            t += tSpeed;
            t = t > 1 ? 1:t;
            break;

        case '-':
            t -= tSpeed;
            t = t <0 ? 0:t;
            break;

        case 'h':
        case 'H':
            InitCamera(0);
            break;

		case 'w':
		case 'W':
			move(MultiplyWithScalar(-1,getForward(eulerCamera)));
			break;
		case 's':
		case 'S':
			move(MultiplyWithScalar(1, getForward(eulerCamera)));
			break;
		case 'a':
		case 'A':
			move(MultiplyWithScalar(-1, getLeft(eulerCamera)));
			break;
		case 'd':
		case 'D':
			move(MultiplyWithScalar(1, getLeft(eulerCamera)));
			break;


    }
}

void HandleIdle(void)
{
    //rotateangle += rotatespeed;
    glutPostRedisplay();
}

void HandleReshape(int w,int h)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
    camera.screenwidth = w;
    camera.screenheight = h;
}


void HandleMouseMotion(int x, int y) {
	int diffx = x - lastx; //check the difference between the current x and the last x position
	int diffy = y - lasty; //check the difference between the current y and the last y position
	lastx = x; //set lastx to the current x position
	lasty = y; //set lasty to the current y position

			   //printf("Handle mouse: %d %d\n", diffx , diffy);

	float fdiffy = std::fmax(std::fminf((float)diffx, 0.5f), -0.5f);
	float fdiffx = std::fmax(std::fminf((float)diffy, 0.5f), -0.5f);

	//printf("FLoat Handle mouse: %f %f\n", fdiffx, fdiffy);

	eulerCamera.pitch += DTOR*fdiffx;
	eulerCamera.yaw += DTOR*fdiffy;

	//printf("Euler Handle mouse: %f %f\n", eulerCamera.pitch, eulerCamera.yaw);
};
void HandleMousePassiveMotion(int x, int y) {
	int diffx = x - lastx; //check the difference between the current x and the last x position
	int diffy = y - lasty; //check the difference between the current y and the last y position
	lastx = x; //set lastx to the current x position
	lasty = y; //set lasty to the current y position

	//printf("Handle mouse: %d %d\n", diffx , diffy);

	float fdiffy = -std::fmax(std::fminf((float)diffx, 1.f), -1.f);
	float fdiffx = -std::fmax(std::fminf((float)diffy, 1.f), -1.f);

	//printf("FLoat Handle mouse: %f %f\n", fdiffx, fdiffy);

	eulerCamera.pitch += DTOR*fdiffx*0.25;
	eulerCamera.yaw += DTOR*fdiffy*0.25;

	eulerCamera.pitch += DTOR*fdiffx*0.25;
	eulerCamera.yaw += DTOR*fdiffy*0.25;

	//printf("Euler Handle mouse: %f %f\n", eulerCamera.pitch, eulerCamera.yaw);

	//xrot += (float)diffy; //set the xrot to xrot with the addition of the difference in the y position
	//yrot += (float)diffx;// set the xrot to yrot with the addition of the difference in the x position
};

void move(VECTOR3D direction) {
	camera.position = Add(camera.position,MultiplyWithScalar(1,direction));
}

void InitCamera(int mode)
{
    camera.aperture = 45;

    camera.position.x = 0;
    camera.position.y = 0;
    camera.position.z = 20;
    
    camera.direction.x = -camera.position.x;
    camera.direction.y = -camera.position.y;
    camera.direction.z = -camera.position.z;
    
    camera.up.x = 0;
    camera.up.y = 1;
    camera.up.z = 0;

	eulerCamera.yaw = 0.f;
	eulerCamera.pitch = PI/2;
	eulerCamera.roll = 0.f;
	eulerCamera.orientation.s = 0.f;
	eulerCamera.orientation.v = camera.direction;
}

