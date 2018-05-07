#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<GL/glut.h>
#include "soil.h"
#include "game.h"


int obj,windowid1,windowid2;
char s[]="Start";



void drawBitmapText(char *string,float x,float y,float z)
{
    char *c;
    glRasterPos3f(x, y,z);

    for (c=string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}

void drawStrokeText(char*string,int x,int y,int z)
{
      char *c;
      glPushMatrix();
      glTranslatef(x, y+8,z);
      glScalef(0.35f,-0.15f,z);

      for (c=string; *c != '\0'; c++)
      {
            glutStrokeCharacter(GLUT_STROKE_ROMAN , *c);
      }
      glPopMatrix();
}



void init()
{
	glClearColor(0,0,0,0.7);
}


void reshape(int w,int h)
{

    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,w,h,0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}


void drawButton(int xMin, int yMin, int xMax, int yMax, int state, char *s){
    glBegin(GL_POLYGON);

    glColor3f(0.4,0.8,0);
        glVertex2d(xMin, yMin);
        glVertex2d(xMax, yMin);
        glVertex2d(xMax, yMax);
        glVertex2d(xMin, yMax);
    glEnd();

    float border = 0.5;
    if(state == 0){
        border = 1;
    }
    glBegin(GL_POLYGON);
    glColor3f(0,0,1);
        glVertex2f(xMin+border, yMin+border);
        glVertex2f(xMax-border, yMin+border);
        glVertex2f(xMax-border, yMax-border);
        glVertex2f(xMin+border, yMax-border);
    glEnd();

    glColor3f(1,0,0);

    drawStrokeText(s,xMin+10,yMin+40,0);


}

void render(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
    glColor3f(0.82,0.18,0.02);
	drawStrokeText("COMPUTER GRAPHICS PROJECT",425,40,0);
	glColor3f(0.8,0.8,0);
	drawStrokeText("Missionaries and Cannibals",425,110,0);
        glColor3f(1,0.7,0.1);
	drawStrokeText("1. ANMOL PANDITA    1PE15CS025",350,320,0);
        glColor3f(1,0.7,0.1);
	drawStrokeText("2. ATISHEY JAIN     1PE15CS031",350,390,0);
        glColor3f(1,1,1);
	drawStrokeText("Press s to start",500,690,0);
    //drawButton(575,600,725,680,1,"START");
    //drawButton(855,600,975,680,1,"HS");
    glutSwapBuffers();

}


void keys(unsigned char key,int x,int y){
    if(key=='s'){
        glClear(GL_COLOR_BUFFER_BIT);
        glutDestroyWindow(windowid1);
        game();
    }

    if(key=='q'){
        exit(0);
    }
}




int main(int argc, char* argv[])
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(1350,750);
    glutInitWindowPosition(0,0);
    windowid1=glutCreateWindow("Missionaries and Cannibals");
    glutKeyboardFunc(keys);
    glutDisplayFunc(render);
    //glutIdleFunc(render);
    glutReshapeFunc(reshape);
//    glutMouseFunc(mouse);
    init();
    glutFullScreen();
    glutMainLoop();
	return 0;
}















