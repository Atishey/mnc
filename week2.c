#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <Windows.h>
#include <GL/glut.h>

using namespace std;

float moveClouds, cloudDeltaX;
GLfloat boatPosX, boatDeltaX;
bool moveBoat=true, moveBoatReverse, boatCrossed, riverCrossedOnce;

void resetAll()
{
    boatPosX = 0.0;
	boatDeltaX = 2.0;
	moveBoat = false;
	moveBoatReverse = false;
	boatCrossed = false;
	riverCrossedOnce = false;

	moveClouds = 0.0;
	cloudDeltaX = 1.0;


	system("cls");

}

void drawPixel(GLint cx, GLint cy)
{

	glBegin(GL_POINTS);
	glVertex2i(cx, cy);
	glEnd();
}

void plotPixels(GLint h, GLint k, GLint x, GLint y)
{
	drawPixel(x + h, y + k);
	drawPixel(-x + h, y + k);
	drawPixel(x + h, -y + k);
	drawPixel(-x + h, -y + k);
	drawPixel(y + h, x + k);
	drawPixel(-y + h, x + k);
	drawPixel(y + h, -x + k);
	drawPixel(-y + h, -x + k);
}

void drawCircle(GLint h, GLint k, GLint r)
{
	GLint d = 1 - r, x = 0, y = r;
	while (y>x)
	{
		plotPixels(h, k, x, y);
		if (d<0) d += 2 * x + 3;
		else
		{
			d += 2 * (x - y) + 5;
			--y;
		}
		++x;
	}
	plotPixels(h, k, x, y);
}


void drawBackground()
{
	glBegin(GL_POLYGON);
	glColor3f(0.58, 0.85, 1.0);
	glVertex2f(0, 0);
	glVertex2f(0, 700);
	glVertex2f(1000, 700);
	glVertex2f(1000, 0);

	glEnd();
}

void drawRiverBank()
{
	// Left Bank
	glBegin(GL_POLYGON);
	glColor3f(0.52, 0.78, 0.22);
	glVertex2i(0, 0);
	glVertex2i(0, 400);
	glVertex2i(400, 400);
	glVertex2i(150, 0);
	glEnd();

	// Right Bank
	glBegin(GL_POLYGON);
	glColor3f(0.52, 0.78, 0.22);
	glVertex2i(1000, 0);
	glVertex2i(1000, 400);
	glVertex2i(600, 400);
	glVertex2i(850, 0);
	glEnd();
}

void drawWater()
{
	glBegin(GL_POLYGON);
	glColor3f(0.06, 0.66, 0.75);
	glVertex2i(150, 0);
	glVertex2i(400, 400);
	glVertex2i(600, 400);
	glVertex2i(850, 0);
	glEnd();
}

void drawHill(int sx, int sy, int w, int h)
{
	glBegin(GL_TRIANGLES);
	glColor3f(0.13, 0.48, 0.49);
	glVertex2i(sx, sy);
	glVertex2i(sx + (w / 2), sy);
	glVertex2i(sx + (w / 2), sy + h);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.11, 0.55, 0.55);
	glVertex2i(sx + w, sy);
	glVertex2i(sx + (w / 2), sy);
	glVertex2i(sx + (w / 2), sy + h);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.77, 0.87, 0.98);
	glVertex2i(sx + (w / 2), sy + h);
	glVertex2i(sx + (w / 3), sy + (h / 1.5));
	glVertex2i(sx + w - (w / 3), sy + (h / 1.5));

	glEnd();
}

void drawClouds()
{
	// cloud 1

	for (int l = 0; l <= 20; l++)
	{
		glColor3f(1.0, 1.0, 1.0);
		drawCircle(190 + moveClouds, 580, l);

	}


	for (int l = 0; l <= 35; l++)
	{
		glColor3f(1.0, 1.0, 1.0);
		drawCircle(230 + moveClouds, 580, l);
		drawCircle(255 + moveClouds, 580, l);
	}

	for (int l = 0; l <= 20; l++)
	{
		glColor3f(1.0, 1.0, 1.0);
		drawCircle(295 + moveClouds, 580, l);
	}

	//cloud2


	for (int l = 0; l <= 20; l++)
	{
		glColor3f(1.0, 1.0, 1.0);
		drawCircle(770 + moveClouds, 615, l);
	}




	for (int l = 0; l <= 35; l++)
	{

		glColor3f(1.0, 1.0, 1.0);
		drawCircle(810 + moveClouds, 615, l);
		drawCircle(835 + moveClouds, 615, l);
		drawCircle(870 + moveClouds, 615, l);
	}

	for (int l = 0; l <= 20; l++)
	{
		glColor3f(1.0, 1.0, 1.0);
		drawCircle(900 + moveClouds, 615, l);
	}


}

void drawBoat()
{
	int px = 245, py = 200, w = 200, h = 50;

	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.04);
	glVertex2i(px + boatPosX, py);
	glVertex2i(px + boatPosX + w, py);
	glVertex2i(px + boatPosX + w - (w / 4), py - h);
	glVertex2i(px + boatPosX + (w / 4), py - h);
	glEnd();

	// Under water reflection

	glBegin(GL_POLYGON);
	glColor4f(0.06, 0.66, 0.75, .5);
	glVertex2i(px + boatPosX + (w / 8), py - (h / 2));
	glVertex2i(px + boatPosX + w - (w / 8), py - (h / 2));
	glVertex2i(px + boatPosX + w - (w / 5), py - h);
	glVertex2i(px + boatPosX + (w / 5), py - h);
	glEnd();
}

// Animation Functions

void animateClouds()
{
	moveClouds += cloudDeltaX;
	//cout << "Cloud Animation" << endl;

	if (moveClouds == 850)
	{
		moveClouds = -900;
	}

}

void animateBoat()
{
	if (moveBoat == true)
	{
		//cout << "Boat animation" << endl;
		boatPosX += boatDeltaX;
		//boatCharacterLeftX += boatDeltaX;
		//boatCharacterRightX += boatDeltaX;

		if (boatPosX == 300.0)
		{
			boatCrossed = true;
			moveBoat = false;
			riverCrossedOnce = true;
			//checkGameStatus();
		}
	}

	if (moveBoatReverse == true)
	{
		boatPosX -= boatDeltaX;
		//boatCharacterLeftX -= boatDeltaX;
		//boatCharacterRightX -= boatDeltaX;

		if (boatPosX == 0.0)
		{
			boatCrossed = false;
			moveBoatReverse = false;
			//checkGameStatus();
		}
	}

}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Drawings

	drawBackground();
	drawWater();
	drawRiverBank();

	//drawSun(650, 600, 50);

	drawHill(200, 400, 600, 300);
	drawHill(0, 400, 400, 200);
	drawHill(700, 400, 300, 150);

	drawClouds();

	//drawCharacters();
	//drawMissionaryOnBoat();
	//drawCannibalOnBoat();

	drawBoat();

	glFlush();

	// Animations

	animateBoat();
	animateClouds();

	glutPostRedisplay();

}

void keyboardFunc(unsigned char key, int x, int y)
{
	//-------- Boat Move --------
	if (key == 'g' || key == 'G')
	{
		cout << "Keyboard event: G" << endl;
		//if (boatPosX == 0.0 && (boatLeftCharacter != 0 || boatRightCharacter != 0))
		//{
			moveBoat = true;
		//}
		glutPostRedisplay();
	}
	else if (key == 'b' || key == 'B')
	{
		cout << "Keyboard event: B" << endl;
		//if (boatPosX == 300.0 && (boatLeftCharacter != 0 || boatRightCharacter != 0))
		//{
			moveBoatReverse = true;
		//}
		glutPostRedisplay();
	}
	else if(key == 'c'||key =='C') {
        cout<< "close" <<endl;
        exit(0);
	}


}

void myInit(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(4.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1000.0, 0.0, 700.0);
}

void ins(int id){
    if(id == 1) {
        moveBoat = true;
		glutPostRedisplay();
    }
    else if(id == 2) {
        moveBoatReverse = true;
		glutPostRedisplay();
    }
    else if(id == 3){
        exit(0);
    }


}

void menu() {
    glutCreateMenu(ins);
    glutAddMenuEntry("Move Boat",1);
    glutAddMenuEntry("Reverse Boat",2);
    glutAddMenuEntry("Exit",3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}



int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000, 700);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - 1000) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - 700) / 2);
	glutCreateWindow("Missionaries and Cannibals");
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(keyboardFunc);
	resetAll();
    myInit();
    menu();
	glutMainLoop();
	return 0;
}
