#include <stdlib.h>
#include <iostream>
#include <math.h>
#include<string.h>
#include <Windows.h>
#include<GL/freeglut.h>
#include <GL/glut.h>
//#include "soil.h"
//#include "main.cpp"

using namespace std;

int score = 0;
bool isWin, isLost;
int leftCannibals, leftMissionaries, rightCannibals, rightMissionaries, onBoatCannibals, onBoatMissionaries;
char toPrint[20];

GLfloat boatPosX, boatDeltaX;
bool moveBoat, moveBoatReverse, boatCrossed, riverCrossedOnce;

float moveClouds, cloudDeltaX;

//Character Details

int leftCharacterPosX, leftCharacterPosY, rightCharacterPosX, rightCharacterPosY, characterWidth, characterHeight;

bool leftMissionaryVisibility[3], leftCannibalVisibility[3], rightMissionaryVisibility[3], rightCannibalVisibility[3], boatMissionaryVisibility[2], boatCannibalVisibility[2];

int boatCharacterLeftX, boatCharacterRightX, boatCharacterY;

// 0 = none, 1 = cannibal, 2 = missionary
int boatLeftCharacter, boatRightCharacter;

int flag=1;
int help;

void *currentfont;

void resetAll()
{

	isWin = false;
	isLost = false;

	score = 0;

	boatLeftCharacter = 0;
	boatRightCharacter = 0;

	boatPosX = 0.0;
	boatDeltaX = 2.0;
	moveBoat = false;
	moveBoatReverse = false;
	boatCrossed = false;
	riverCrossedOnce = false;

	moveClouds = 0.0;
	cloudDeltaX = 1.0;

	leftCharacterPosX = 5;
	leftCharacterPosY = 250;

	rightCharacterPosX = 700;
	rightCharacterPosY = 250;

	characterWidth = 45;
	characterHeight = 80;

	for (int i = 0; i < 3; i++)
	{
		leftMissionaryVisibility[i] = true;
		leftCannibalVisibility[i] = true;

		rightMissionaryVisibility[i] = false;
		rightCannibalVisibility[i] = false;
	}

	for (int i = 0; i < 2; i++)
	{
		boatMissionaryVisibility[i] = false;
		boatCannibalVisibility[i] = false;
	}

	boatCharacterLeftX = 285;
	boatCharacterRightX = 360;
	boatCharacterY = 165;

	system("cls");

}

void checkGameStatus()
{
	cout << "Checking game status..." << endl;

	leftCannibals = 0;
	leftMissionaries = 0;
	rightCannibals = 0;
	rightMissionaries = 0;
	onBoatCannibals = 0;
	onBoatMissionaries = 0;


	for (int i = 0; i < 3; i++)
	{
		if (leftCannibalVisibility[i])
		{
			leftCannibals++;
		}

		if (leftMissionaryVisibility[i])
		{
			leftMissionaries++;
		}

		if (rightCannibalVisibility[i])
		{
			rightCannibals++;
		}

		if (rightMissionaryVisibility[i])
		{
			rightMissionaries++;
		}
	}

	if (boatLeftCharacter == 1)
	{
		onBoatCannibals++;
	}
	if (boatRightCharacter == 1)
	{
		onBoatCannibals++;
	}

	if (boatLeftCharacter == 2)
	{
		onBoatMissionaries++;
	}

	if (boatRightCharacter == 2)
	{
		onBoatMissionaries++;
	}

	cout << "Left    : cannibals = " << leftCannibals << ", missionaries = " << leftMissionaries << endl;
	cout << "Right   : cannibals = " << rightCannibals << ", missionaries = " << rightMissionaries << endl;
	cout << "On Boat : cannibals = " << onBoatCannibals << ", missionaries = " << onBoatMissionaries << endl;

	if (!boatCrossed)
	{
		if ((leftCannibals + onBoatCannibals) > (leftMissionaries + onBoatMissionaries) && (leftMissionaries + onBoatMissionaries) > 0)
		{
			isLost = true;
		}
		if ((rightCannibals) > (rightMissionaries) && rightMissionaries > 0)
		{
			isLost = true;
		}
	}
	else if (boatCrossed)
	{
		if ((rightCannibals + onBoatCannibals) > (rightMissionaries + onBoatMissionaries) && (rightMissionaries + onBoatMissionaries) > 0)
		{
			isLost = true;
		}
		if ((leftCannibals) > (leftMissionaries) && leftMissionaries > 0)
		{
			isLost = true;
		}
	}

	if (boatCrossed && (rightCannibals + onBoatCannibals) == 3 && (rightMissionaries + onBoatMissionaries) == 3)
	{
		isWin = true;
	}

	if (isLost)
	{
		int msgRes = MessageBox(NULL, "You Lose...", "Retry", MB_RETRYCANCEL);

		if (msgRes == IDRETRY)
		{
			resetAll();
		}
		else if (msgRes == IDCANCEL)
		{
			exit(1);
		}
	}

	if (isWin)
	{
		MessageBox(NULL, "Congratulation. You Win!!!", "Congratulation", MB_OK);
		int msgRes = MessageBox(NULL, "Do you want to play again?", "Restart?", MB_YESNO);

		if (msgRes == IDYES)
		{
			resetAll();
		}
		else if (msgRes == IDNO)
		{
			exit(1);
		}
	}


}

int getCannibalBlankPosition()
{
	if (!boatCrossed)
	{
		for (int i = 0; i < 3; i++)
		{
			if (leftCannibalVisibility[i] == false)
			{
				return i;
			}
		}

		return -1;
	}
	else if (boatCrossed)
	{
		for (int i = 0; i < 3; i++)
		{
			if (rightCannibalVisibility[i] == false)
			{
				return i;
			}
		}

		return -1;
	}
}

int getMissionaryBlankPosition()
{
	if (!boatCrossed)
	{
		for (int i = 0; i < 3; i++)
		{
			if (leftMissionaryVisibility[i] == false)
			{
				return i;
			}
		}

		return -1;
	}
	else if (boatCrossed)
	{
		for (int i = 0; i < 3; i++)
		{
			if (rightMissionaryVisibility[i] == false)
			{
				return i;
			}
		}

		return -1;
	}
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

void drawSun(GLfloat x, GLfloat y, GLfloat radius) {
	int i;
	double const PI = 3.1416;
	int triangleAmount = 500; //# of triangles used to draw circle

							  //GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0, 0.93, 0.35);
	glVertex2f(x, y); // center of circle
	for (i = 0; i <= triangleAmount; i++) {
		glVertex2f(
			x + (radius * cos(i *  twicePi / triangleAmount)),
			y + (radius * sin(i * twicePi / triangleAmount))
		);
	}
	glEnd();


	radius = radius - 5;

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.99, 0.85, 0.21);
	glVertex2f(x, y); // center of circle
	for (i = 0; i <= triangleAmount; i++) {
		glVertex2f(
			x + (radius * cos(i *  twicePi / triangleAmount)),
			y + (radius * sin(i * twicePi / triangleAmount))
		);
	}
	glEnd();

	radius = radius - 5;

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.98, 0.75, 0.18);
	glVertex2f(x, y); // center of circle
	for (i = 0; i <= triangleAmount; i++) {
		glVertex2f(
			x + (radius * cos(i *  twicePi / triangleAmount)),
			y + (radius * sin(i * twicePi / triangleAmount))
		);
	}
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

void drawMissionary(int px, int py)
{
	int w = characterWidth, h = characterHeight;

	// Hair
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + 12, py + 66);
	glVertex2i(px + 34, py + 66);
	glVertex2i(px + 34, py + 76);
	glVertex2i(px + 12, py + 76);
	glEnd();

	// Head
	glBegin(GL_POLYGON);
	glColor3f(0.99, 0.87, 0.75);
	glVertex2i(px + 20, py + 58);
	glVertex2i(px + 25, py + 58);
	glVertex2i(px + 25, py + 62);
	glVertex2i(px + 20, py + 62);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.99, 0.87, 0.75);
	glVertex2i(px + 15, py + 62);
	glVertex2i(px + 30, py + 62);
	glVertex2i(px + 30, py + 72);
	glVertex2i(px + 15, py + 72);
	glEnd();

	// Face
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + 17, py + 67);
	glVertex2i(px + 21, py + 67);
	glVertex2i(px + 21, py + 69);
	glVertex2i(px + 17, py + 69);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + 24, py + 67);
	glVertex2i(px + 28, py + 67);
	glVertex2i(px + 28, py + 69);
	glVertex2i(px + 24, py + 69);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + 20, py + 64);
	glVertex2i(px + 25, py + 64);
	glVertex2i(px + 25, py + 65);
	glVertex2i(px + 20, py + 65);
	glEnd();

	// Hand
	glBegin(GL_POLYGON);
	glColor3f(0.99, 0.87, 0.75);
	glVertex2i(px + 10, py + 56);
	glVertex2i(px + 5, py + 56);
	glVertex2i(px + 5, py + 33);
	glVertex2i(px + 8, py + 33);
	glVertex2i(px + 8, py + 53);
	glVertex2i(px + 10, py + 53);
	glVertex2i(px + 10, py + 56);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.99, 0.87, 0.75);
	glVertex2i(px + 34, py + 56);
	glVertex2i(px + 39, py + 56);
	glVertex2i(px + 39, py + 33);
	glVertex2i(px + 36, py + 33);
	glVertex2i(px + 36, py + 53);
	glVertex2i(px + 34, py + 53);
	glVertex2i(px + 34, py + 56);
	glEnd();

	// Body
	glBegin(GL_POLYGON);
	glColor3f(0.68, 0.50, 0.77);
	glVertex2i(px + 10, py + 34);
	glVertex2i(px + 34, py + 34);
	glVertex2i(px + 34, py + 58);
	glVertex2i(px + 10, py + 58);
	glEnd();


	// Pant
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.33, 0.56);
	glVertex2i(px + 10, py + 8);
	glVertex2i(px + 17, py + 8);
	glVertex2i(px + 17, py + 34);
	glVertex2i(px + 10, py + 34);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.33, 0.56);
	glVertex2i(px + 26, py + 8);
	glVertex2i(px + 34, py + 8);
	glVertex2i(px + 34, py + 34);
	glVertex2i(px + 26, py + 34);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.33, 0.56);
	glVertex2i(px + 10, py + 34);
	glVertex2i(px + 34, py + 34);
	glVertex2i(px + 34, py + 20);
	glVertex2i(px + 10, py + 20);
	glEnd();



	// Shoes
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + 7, py + 3);
	glVertex2i(px + 19, py + 3);
	glVertex2i(px + 19, py + 8);
	glVertex2i(px + 7, py + 8);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + 25, py + 3);
	glVertex2i(px + 38, py + 3);
	glVertex2i(px + 38, py + 8);
	glVertex2i(px + 25, py + 8);
	glEnd();

}

void drawCannibal(int px, int py)
{
	int w = characterWidth, h = characterHeight;

	// Hair
	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(px + 17, py + 72);
	glVertex2i(px + 21, py + 72);
	glVertex2i(px + 21, py + 78);
	glVertex2i(px + 17, py + 78);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(px + 24, py + 72);
	glVertex2i(px + 28, py + 72);
	glVertex2i(px + 28, py + 78);
	glVertex2i(px + 24, py + 78);
	glEnd();

	// Head
	glBegin(GL_POLYGON);
	glColor3f(0.90, 0.91, 0.08);
	glVertex2i(px + 20, py + 58);
	glVertex2i(px + 25, py + 58);
	glVertex2i(px + 25, py + 62);
	glVertex2i(px + 20, py + 62);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.90, 0.91, 0.08);
	glVertex2i(px + 15, py + 62);
	glVertex2i(px + 30, py + 62);
	glVertex2i(px + 30, py + 72);
	glVertex2i(px + 15, py + 72);
	glEnd();

	// Face
	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(px + 17, py + 67);
	glVertex2i(px + 21, py + 67);
	glVertex2i(px + 21, py + 69);
	glVertex2i(px + 17, py + 69);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(px + 24, py + 67);
	glVertex2i(px + 28, py + 67);
	glVertex2i(px + 28, py + 69);
	glVertex2i(px + 24, py + 69);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(px + 20, py + 64);
	glVertex2i(px + 25, py + 64);
	glVertex2i(px + 25, py + 65);
	glVertex2i(px + 20, py + 65);
	glEnd();

	// Hand
	glBegin(GL_POLYGON);
	glColor3f(0.90, 0.91, 0.08);
	glVertex2i(px + 10, py + 56);
	glVertex2i(px + 5, py + 56);
	glVertex2i(px + 5, py + 33);
	glVertex2i(px + 8, py + 33);
	glVertex2i(px + 8, py + 53);
	glVertex2i(px + 10, py + 53);
	glVertex2i(px + 10, py + 56);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.90, 0.91, 0.08);
	glVertex2i(px + 34, py + 56);
	glVertex2i(px + 39, py + 56);
	glVertex2i(px + 39, py + 33);
	glVertex2i(px + 36, py + 33);
	glVertex2i(px + 36, py + 53);
	glVertex2i(px + 34, py + 53);
	glVertex2i(px + 34, py + 56);
	glEnd();

	// Body
	glBegin(GL_POLYGON);
	glColor3f(0.77, 0.15, 0.10);
	glVertex2i(px + 10, py + 34);
	glVertex2i(px + 34, py + 34);
	glVertex2i(px + 34, py + 58);
	glVertex2i(px + 10, py + 58);
	glEnd();


	// Pant
	glBegin(GL_POLYGON);
	glColor3f(0.77, 0.15, 0.10);
	glVertex2i(px + 10, py + 8);
	glVertex2i(px + 17, py + 8);
	glVertex2i(px + 17, py + 34);
	glVertex2i(px + 10, py + 34);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.77, 0.15, 0.10);
	glVertex2i(px + 26, py + 8);
	glVertex2i(px + 34, py + 8);
	glVertex2i(px + 34, py + 34);
	glVertex2i(px + 26, py + 34);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.77, 0.15, 0.10);
	glVertex2i(px + 10, py + 34);
	glVertex2i(px + 34, py + 34);
	glVertex2i(px + 34, py + 20);
	glVertex2i(px + 10, py + 20);
	glEnd();



	// Shoes
	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(px + 7, py + 3);
	glVertex2i(px + 19, py + 3);
	glVertex2i(px + 19, py + 8);
	glVertex2i(px + 7, py + 8);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(px + 25, py + 3);
	glVertex2i(px + 38, py + 3);
	glVertex2i(px + 38, py + 8);
	glVertex2i(px + 25, py + 8);
	glEnd();
}

void drawMissionaryOnBoat()
{
	int px = boatCharacterLeftX, py = boatCharacterY, w = characterWidth, h = characterHeight;

	if (boatMissionaryVisibility[0])
	{
		drawMissionary(px, py);
	}

	px = boatCharacterRightX;

	if (boatMissionaryVisibility[1])
	{
		drawMissionary(px, py);
	}


}

void drawCannibalOnBoat()
{
	int px = boatCharacterLeftX, py = boatCharacterY, w = characterWidth, h = characterHeight;

	if (boatCannibalVisibility[0])
	{
		drawCannibal(px, py);
	}

	px = boatCharacterRightX;

	if (boatCannibalVisibility[1])
	{
		drawCannibal(px, py);
	}


}


void drawCharacters()
{
	int px, py, w = characterWidth, h = characterHeight;

	px = leftCharacterPosX;
	py = leftCharacterPosY;

	if (leftCannibalVisibility[0])
	{
		drawCannibal(px, py);
	}

	if (leftCannibalVisibility[1])
	{
		drawCannibal(px + w + 5, py);
	}

	if (leftCannibalVisibility[2])
	{
		drawCannibal(px + (2 * w) + (2 * 5), py);
	}

	if (leftMissionaryVisibility[0])
	{
		drawMissionary(px + (3 * w) + (3 * 5), py);
	}

	if (leftMissionaryVisibility[1])
	{
		drawMissionary(px + (4 * w) + (4 * 5), py);
	}

	if (leftMissionaryVisibility[2])
	{
		drawMissionary(px + (5 * w) + (5 * 5), py);
	}

	px = rightCharacterPosX;
	py = rightCharacterPosY;

	if (rightMissionaryVisibility[0])
	{
		drawMissionary(px, py);
	}

	if (rightMissionaryVisibility[1])
	{
		drawMissionary(px + w + 5, py);
	}

	if (rightMissionaryVisibility[2])
	{
		drawMissionary(px + (2 * w) + (2 * 5), py);
	}

	if (rightCannibalVisibility[0])
	{
		drawCannibal(px + (3 * w) + (3 * 5), py);
	}

	if (rightCannibalVisibility[1])
	{
		drawCannibal(px + (4 * w) + (4 * 5), py);
	}

	if (rightCannibalVisibility[2])
	{
		drawCannibal(px + (5 * w) + (5 * 5), py);
	}

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
		boatCharacterLeftX += boatDeltaX;
		boatCharacterRightX += boatDeltaX;

		if (boatPosX == 300.0)
		{
			boatCrossed = true;
			moveBoat = false;
			riverCrossedOnce = true;
			checkGameStatus();
		}
	}

	if (moveBoatReverse == true)
	{
		boatPosX -= boatDeltaX;
		boatCharacterLeftX -= boatDeltaX;
		boatCharacterRightX -= boatDeltaX;

		if (boatPosX == 0.0)
		{
			boatCrossed = false;
			moveBoatReverse = false;
			checkGameStatus();
		}
	}

}

void scoredisplay (int posx, int posy, int posz, int space_char, int scorevar)
{
        int j=0,p,k;
        GLvoid *font_style1 = GLUT_BITMAP_TIMES_ROMAN_24;

        p = scorevar;
        j = 0;
        k = 0;
        while(p > 9)
        {
            k = p % 10;
            glRasterPos3f ((posx-(j*space_char)),posy, posz);
            glutBitmapCharacter(font_style1,48+k);
            j++;
            p /= 10;
        }
            glRasterPos3f ((posx-(j*space_char)), posy, posz);
            glutBitmapCharacter(font_style1,48+p);

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);



	// Drawings

	drawBackground();
	drawWater();
	drawRiverBank();

	drawSun(650, 600, 50);

	drawHill(200, 400, 600, 300);
	drawHill(0, 400, 400, 200);
	drawHill(700, 400, 300, 150);

	drawClouds();

	drawCharacters();
	drawMissionaryOnBoat();
	drawCannibalOnBoat();

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
		//cout << "Keyboard event: G" << endl;
		if (boatPosX == 0.0 && (boatLeftCharacter != 0 || boatRightCharacter != 0))
		{
			moveBoat = true;
			score++;
			cout <<"cur score :"<< score << endl;
            //glColor3f(0.82,0.18,0.02);
            //drawStrokeText("Score",425,40,0);
		}
		else {
            cout << "Empty boat" << endl;
            MessageBox(NULL, "Please select missionary or cannibal to put them into the boat!!!", "Empty", MB_OK);
		}
		//scoredisplay(400,0,0,8,score);
		glutPostRedisplay();
		//scoredisplay(400,0,0,8,score);
		//glutPostRedisplay();
	}
	else if (key == 'b' || key == 'B')
	{
		//cout << "Keyboard event: B" << endl;
		if (boatPosX == 300.0 && (boatLeftCharacter != 0 || boatRightCharacter != 0))
		{
			moveBoatReverse = true;
			score++;
			cout <<"cur score :"<< score << endl;
		}

		else {
            cout << "Empty boat" << endl;
            MessageBox(NULL, "Please select missionary or cannibal to put them into the boat!!!", "Empty", MB_OK);
		}

        //scoredisplay(400,0,0,8,score);
		glutPostRedisplay();
	}
	else if (key == 'r' || key == 'R')
	{
		//cout << "Keyboard event: R" << endl;
		resetAll();
		glutPostRedisplay();
	}

	else if(key == 'q' || key == 'Q')
    {
        int msgRes = MessageBox(NULL, "Are you sure to quit?", "Really??", MB_YESNO);

		if (msgRes == IDYES)
		{
			exit(0);
		}
		else if (msgRes == IDNO)
		{
			glutPostRedisplay();
		}
       // MessageBox(NULL, "Are you sure to quit?", "Exit!!Sure??", MB_OK);
        //exit(0);
    }

    else
    {
        MessageBox(NULL, "Please select any key from the following options:\n1.\tG\t-\tMove the boat forward\n2.\tB\t-\tMove the boat backward\n3.\tR\t-\tReset the game\n4.\tQ\t-\tQuit the game\n", "Help Box", MB_OK);
    }

}

void mouseFunc(int b, int s, int x, int y) {

	if (s == GLUT_DOWN && b == GLUT_LEFT_BUTTON) {

		int px, py, w = characterWidth, h = characterHeight;
		//MessageBox(NULL, "Please select missionary or cannibal to put them into the boat!!!", "Help!!", MB_OK);

		if (!boatCrossed)
		{
		    //
		    //x = maxX - x/(globalWidth/maxX);
		    //MessageBox(NULL, "Please select missionary or cannibal to put them into the boat!!!", "Help!!", MB_OK);
			px = leftCharacterPosX ;
			py = leftCharacterPosY + 120 ;

			if (x >= px && x <= (px + w) && y >= py && y <= (py + h) && leftCannibalVisibility[0])
			{

				cout << "Left: Cannibal 0" << endl;


				if (boatLeftCharacter == 0)
				{
					leftCannibalVisibility[0] = false;
					boatCannibalVisibility[0] = true;
					boatLeftCharacter = 1;
				}
				else if (boatRightCharacter == 0)
				{
					leftCannibalVisibility[0] = false;
					boatCannibalVisibility[1] = true;
					boatRightCharacter = 1;
				}

				//cout << px << ',' << px + w << ',' << py << ',' << py + h << endl;
				//cout << x << '\t' << y << endl;
			}
			else if (x >= (px + w + 5) && x <= (px + w + 5 + w) && y >= py && y <= (py + h) && leftCannibalVisibility[1])
			{
				cout << "Left: Cannibal 1" << endl;

				if (boatLeftCharacter == 0)
				{
					leftCannibalVisibility[1] = false;
					boatCannibalVisibility[0] = true;
					boatLeftCharacter = 1;
				}
				else if (boatRightCharacter == 0)
				{
					leftCannibalVisibility[1] = false;
					boatCannibalVisibility[1] = true;
					boatRightCharacter = 1;
				}
			}
			else if (x >= (px + (2 * w) + (2 * 5)) && x <= (px + (2 * w) + (2 * 5) + w) && y >= py && y <= (py + h) && leftCannibalVisibility[2])
			{
				cout << "Left: Cannibal 2" << endl;


				if (boatLeftCharacter == 0)
				{
					leftCannibalVisibility[2] = false;
					boatCannibalVisibility[0] = true;
					boatLeftCharacter = 1;
				}
				else if (boatRightCharacter == 0)
				{
					leftCannibalVisibility[2] = false;
					boatCannibalVisibility[1] = true;
					boatRightCharacter = 1;
				}
			}
			else if (x >= (px + (3 * w) + (3 * 5)) && x <= (px + (3 * w) + (3 * 5) + w) && y >= py && y <= (py + h) && leftMissionaryVisibility[0])
			{
				cout << "Left: Missionary 0" << endl;
				if (boatLeftCharacter == 0)
				{
					leftMissionaryVisibility[0] = false;
					boatMissionaryVisibility[0] = true;
					boatLeftCharacter = 2;
					//leftMissionaries--;
				}
				else if (boatRightCharacter == 0)
				{
					leftMissionaryVisibility[0] = false;
					boatMissionaryVisibility[1] = true;
					boatRightCharacter = 2;
				}
			}
			else if (x >= (px + (4 * w) + (4 * 5)) && x <= (px + (4 * w) + (4 * 5) + w) && y >= py && y <= (py + h) && leftMissionaryVisibility[1])
			{
				cout << "Left: Missionary 1" << endl;

				if (boatLeftCharacter == 0)
				{
					leftMissionaryVisibility[1] = false;
					boatMissionaryVisibility[0] = true;
					boatLeftCharacter = 2;
				}
				else if (boatRightCharacter == 0)
				{
					leftMissionaryVisibility[1] = false;
					boatMissionaryVisibility[1] = true;
					boatRightCharacter = 2;
				}
			}
			else if (x >= (px + (5 * w) + (5 * 5)) && x <= (px + (5 * w) + (5 * 5) + w) && y >= py && y <= (py + h) && leftMissionaryVisibility[2])
			{
				cout << "Left: Missionary 2" << endl;

				if (boatLeftCharacter == 0)
				{
					leftMissionaryVisibility[2] = false;
					boatMissionaryVisibility[0] = true;
					boatLeftCharacter = 2;
				}
				else if (boatRightCharacter == 0)
				{
					leftMissionaryVisibility[2] = false;
					boatMissionaryVisibility[1] = true;
					boatRightCharacter = 2;
				}
			}

			if (boatLeftCharacter != 0 || boatRightCharacter != 0)
			{
				w = characterWidth, h = characterHeight;


				if (x >= boatCharacterLeftX && x <= (boatCharacterLeftX + w) && y >= (boatCharacterY + 260) && y <= ((boatCharacterY + 260) + h))
				{
					/*cout << boatCharacterLeftX << ',' << boatCharacterLeftX + w << ',' << (boatCharacterY + 260) << ',' << (boatCharacterY + 260) + h << endl;
					cout << x << '\t' << y << endl;*/
					cout << "Left: Left Character On boat" << endl;

					if (boatLeftCharacter == 1)
					{
						leftCannibalVisibility[getCannibalBlankPosition()] = true;
						boatCannibalVisibility[0] = false;
						boatLeftCharacter = 0;


					}
					else if (boatLeftCharacter == 2)
					{
						leftMissionaryVisibility[getMissionaryBlankPosition()] = true;
						boatMissionaryVisibility[0] = false;
						boatLeftCharacter = 0;
					}

				}
				else if (x >= boatCharacterRightX && x <= (boatCharacterRightX + w) && y >= (boatCharacterY + 260) && y <= ((boatCharacterY + 260) + h))
				{
					cout << "Left: Right Character On boat" << endl;

					if (boatRightCharacter == 1)
					{
						leftCannibalVisibility[getCannibalBlankPosition()] = true;
						boatCannibalVisibility[1] = false;
						boatRightCharacter = 0;
					}
					else if (boatRightCharacter == 2)
					{
						leftMissionaryVisibility[getMissionaryBlankPosition()] = true;
						boatMissionaryVisibility[1] = false;
						boatRightCharacter = 0;
					}
				}

			}

		}
		else if (boatCrossed)
		{
			px = rightCharacterPosX;
			py = rightCharacterPosY + 120;

			if (x >= px && x <= (px + w) && y >= py && y <= (py + h) && rightMissionaryVisibility[0])
			{
				//cout << px << ',' << px + w << ',' << py << ',' << py + h << endl;
				//cout << x << '\t' << y << endl;

				cout << "Right: Missionary 0" << endl;

				if (boatLeftCharacter == 0)
				{
					rightMissionaryVisibility[0] = false;
					boatMissionaryVisibility[0] = true;
					boatLeftCharacter = 2;
				}
				else if (boatRightCharacter == 0)
				{
					rightMissionaryVisibility[0] = false;
					boatMissionaryVisibility[1] = true;
					boatRightCharacter = 2;
				}
			}
			else if (x >= (px + w + 5) && x <= (px + w + 5 + w) && y >= py && y <= (py + h) && rightMissionaryVisibility[1])
			{
				cout << "Right: Missionary 1" << endl;
				if (boatLeftCharacter == 0)
				{
					rightMissionaryVisibility[1] = false;
					boatMissionaryVisibility[0] = true;
					boatLeftCharacter = 2;
				}
				else if (boatRightCharacter == 0)
				{
					rightMissionaryVisibility[1] = false;
					boatMissionaryVisibility[1] = true;
					boatRightCharacter = 2;
				}
			}
			else if (x >= (px + (2 * w) + (2 * 5)) && x <= (px + (2 * w) + (2 * 5) + w) && y >= py && y <= (py + h) && rightMissionaryVisibility[2])
			{
				cout << "Right: Missionary 2" << endl;

				if (boatLeftCharacter == 0)
				{
					rightMissionaryVisibility[2] = false;
					boatMissionaryVisibility[0] = true;
					boatLeftCharacter = 2;
				}
				else if (boatRightCharacter == 0)
				{
					rightMissionaryVisibility[2] = false;
					boatMissionaryVisibility[1] = true;
					boatRightCharacter = 2;
				}
			}
			else if (x >= (px + (3 * w) + (3 * 5)) && x <= (px + (3 * w) + (3 * 5) + w) && y >= py && y <= (py + h) && rightCannibalVisibility[0])
			{
				cout << "Right: Cannibal 0" << endl;

				if (boatLeftCharacter == 0)
				{
					rightCannibalVisibility[0] = false;
					boatCannibalVisibility[0] = true;
					boatLeftCharacter = 1;
				}
				else if (boatRightCharacter == 0)
				{
					rightCannibalVisibility[0] = false;
					boatCannibalVisibility[1] = true;
					boatRightCharacter = 1;
				}
			}
			else if (x >= (px + (4 * w) + (4 * 5)) && x <= (px + (4 * w) + (4 * 5) + w) && y >= py && y <= (py + h) && rightCannibalVisibility[1])
			{
				cout << "Right: Cannibal 1" << endl;

				if (boatLeftCharacter == 0)
				{
					rightCannibalVisibility[1] = false;
					boatCannibalVisibility[0] = true;
					boatLeftCharacter = 1;
				}
				else if (boatRightCharacter == 0)
				{
					rightCannibalVisibility[1] = false;
					boatCannibalVisibility[1] = true;
					boatRightCharacter = 1;
				}
			}
			else if (x >= (px + (5 * w) + (5 * 5)) && x <= (px + (5 * w) + (5 * 5) + w) && y >= py && y <= (py + h) && rightCannibalVisibility[2])
			{
				cout << "Right: Cannibal 2" << endl;

				if (boatLeftCharacter == 0)
				{
					rightCannibalVisibility[2] = false;
					boatCannibalVisibility[0] = true;
					boatLeftCharacter = 1;
				}
				else if (boatRightCharacter == 0)
				{
					rightCannibalVisibility[2] = false;
					boatCannibalVisibility[1] = true;
					boatRightCharacter = 1;
				}
			}

			if (boatLeftCharacter != 0 || boatRightCharacter != 0)
			{
				w = characterWidth, h = characterHeight;


				if (x >= boatCharacterLeftX && x <= (boatCharacterLeftX + w) && y >= (boatCharacterY + 260) && y <= ((boatCharacterY + 260) + h))
				{
					/*cout << boatCharacterLeftX << ',' << boatCharacterLeftX + w << ',' << (boatCharacterY + 260) << ',' << (boatCharacterY + 260) + h << endl;
					cout << x << '\t' << y << endl;*/

					cout << "Right: Left Character On boat" << endl;

					if (boatLeftCharacter == 1)
					{
						rightCannibalVisibility[getCannibalBlankPosition()] = true;
						boatCannibalVisibility[0] = false;
						boatLeftCharacter = 0;
					}
					else if (boatLeftCharacter == 2)
					{
						rightMissionaryVisibility[getMissionaryBlankPosition()] = true;
						boatMissionaryVisibility[0] = false;
						boatLeftCharacter = 0;
					}


				}
				else if (x >= boatCharacterRightX && x <= (boatCharacterRightX + w) && y >= (boatCharacterY + 260) && y <= ((boatCharacterY + 260) + h))
				{
					cout << "Right: Right Character On boat" << endl;

					if (boatRightCharacter == 1)
					{
						rightCannibalVisibility[getCannibalBlankPosition()] = true;
						boatCannibalVisibility[1] = false;
						boatRightCharacter = 0;
					}
					else if (boatRightCharacter == 2)
					{
						rightMissionaryVisibility[getMissionaryBlankPosition()] = true;
						boatMissionaryVisibility[1] = false;
						boatRightCharacter = 0;

					}
				}

			}
		}


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

    if(id == 1)
    {

        MessageBox(NULL, "Please select missionary or cannibal by moving the cursor to the respective candidate and clicking it to move in and out of the boat", "Happy to help :)", MB_OK);
    }
    else if(id == 2)
    {
        MessageBox(NULL, "Please select any key from the following options:\n1.\tG\t-\tMove the boat forward\n2.\tB\t-\tMove the boat backward\n3.\tR\t-\tReset the game\n4.\tQ\t-\tQuit the game\n", "Happy to help :)", MB_OK);
    }


}

void menu() {
    help = glutCreateMenu(ins);

    glutAddMenuEntry("Mouse",1);
    glutAddMenuEntry("Keyboard",2);

   int  mainmenu = glutCreateMenu(ins);
    glutAddSubMenu("Help??",help);


    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void game()
{
	//glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000, 700);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - 1000) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - 700) / 2);
	glutCreateWindow("Missionaries and Cannibals");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);
	resetAll();
	menu();
	//scoredisplay(800,400,0,1,score);
	myInit();
	//glutFullScreen();
	glutMainLoop();
	//return 0;
}
