#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <fstream>




#ifdef __APPLE__  // include Mac OS X verions of headers
#include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#include <GL/glut.h>
#endif

using namespace std;

#define XOFF          50
#define YOFF          50
#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600

int K = 400; // Total number of frames for the animation cycle
int currentFrame = 0; // Current frame of animation

struct Circle {
	int x, y, r;
};
vector<Circle> circles; // Vector to store multiple circles
struct Circle2
{
	float xd, yd, rd, originalrd;
};
vector<Circle2> circles2;
Circle mycircle;
float scale = 1.0f;
void display1(void);
void display2(void);
void myinit(void);
void drawCircle(int xCenter, int yCenter, int radius);
void updateAnimation(int value);
void display3();

/* Function to handle file input; modification may be needed */
void file_in(void);

/*-----------------
The main function
------------------*/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	/* Use both double buffering and Z buffer */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowPosition(XOFF, YOFF);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Assignment 1");
	cout << "Choose to draw a customed circle (Type 1) or  Input a file (Type 2) or Generate Animation (Type 3):" << endl;
	int option;
	cin >> option;
	if (option == 1) {
		cout << "Input the parameters" << endl;
		cin >> mycircle.x >> mycircle.y >> mycircle.r;
		glutDisplayFunc(display1);
	}


	/* Function call to handle file input here */
	if (option == 2) {
		file_in();
		glutDisplayFunc(display2);
	}

	if (option == 3)
	{
		file_in();
		glutDisplayFunc(display3);
		glutTimerFunc(1000 / K, updateAnimation, 0); // Start the animation
	}

	myinit();
	glutMainLoop();

	return 0;
}

/*----------
file_in(): file input function. Modify here.
------------*/
void file_in(void)
{
	string filename;
	cout << "Type the input file name: " << endl;
	cin >> filename;
	ifstream ifs;
	ifs.open(filename);
	int circlenumber;
	ifs >> circlenumber;
	while (circlenumber > 0)
	{
		Circle newcircle;
		ifs >> newcircle.x >> newcircle.y >> newcircle.r;
		circles.push_back(newcircle);
		circlenumber--;
	}

	// Compute scaling factors based on the circles to fit them in the window
	int minX = 0, maxX = 0, minY = 0, maxY = 0;
	for (const auto& circle : circles) {
		minX = min(minX, circle.x - circle.r);
		maxX = max(maxX, circle.x + circle.r);
		minY = min(minY, circle.y - circle.r);
		maxY = max(maxY, circle.y + circle.r);
	}
	float scaleX = WINDOW_WIDTH / float(maxX - minX);
	float scaleY = WINDOW_HEIGHT / float(maxY - minY);
	scale = min(scaleX, scaleY) - 0.02f;

	Circle2 newcircle2;
	for (const auto& circle : circles) {
		newcircle2.xd = circle.x * scale + WINDOW_WIDTH / 2;
		newcircle2.yd = circle.y * scale + WINDOW_HEIGHT / 2;
		newcircle2.originalrd = circle.r * scale;
		newcircle2.rd = 0;
		circles2.push_back(newcircle2);
	}

}

void updateAnimation(int value) {
	currentFrame = (currentFrame + 1) % K;
	for (auto& circle : circles2) {
		circle.rd = circle.originalrd * ((float)currentFrame / K); // Gradually increase radius
	}
	glutPostRedisplay(); // Request a redraw
	glutTimerFunc(1000 / K, updateAnimation, 0); // Schedule next update
}


void circlePoint(int centerX, int centerY, int x, int y) {
	glVertex2i(centerX + x, centerY + y);
	glVertex2i(centerX - x, centerY + y);
	glVertex2i(centerX + x, centerY - y);
	glVertex2i(centerX - x, centerY - y);
	glVertex2i(centerX + y, centerY + x);
	glVertex2i(centerX - y, centerY + x);
	glVertex2i(centerX + y, centerY - x);
	glVertex2i(centerX - y, centerY - x);
}

void drawCircle(int xCenter, int yCenter, int radius) {
	int x = 0;
	int y = radius;
	int p = 1 - radius; // Initial decision parameter
	while (x < y) {
		// Drawing symmetric points in all octants
		circlePoint(xCenter, yCenter, x, y);
		if (p < 0) {
			p += 2 * x + 3;
		}
		else {
			p += 2 * (x - y) + 5;
			y--;
		}
		x++;
	}
}

/*---------------------------------------------------------------------
display(): This function is called once for _every_ frame.
---------------------------------------------------------------------*/
void display1()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0f, 0.84f, 0.0f);         /* draw in golden yellow */
	glPointSize(1.0);                     /* size of each point */

	glBegin(GL_POINTS);
	drawCircle(mycircle.x, mycircle.y, mycircle.r);             /* draw a vertex here */
	glEnd();

	glFlush();                            /* render graphics */

	glutSwapBuffers();                    /* swap buffers */
}

void display2()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0f, 0.84f, 0.0f);         /* draw in golden yellow */
	glPointSize(1.0);                     /* size of each point */

	glBegin(GL_POINTS);
	for (const auto& circle : circles)
	{
		drawCircle(circle.x * scale + WINDOW_WIDTH / 2, circle.y * scale + WINDOW_HEIGHT / 2, circle.r * scale);
	}
	glEnd();

	glFlush();                            /* render graphics */

	glutSwapBuffers();                    /* swap buffers */
}

void display3()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0f, 0.84f, 0.0f);         /* draw in golden yellow */
	glPointSize(1.0);                     /* size of each point */

	glBegin(GL_POINTS);
	for (const auto& circle : circles2)
	{
		drawCircle(circle.xd, circle.yd, circle.rd);
	}
	glEnd();

	glFlush();                            /* render graphics */

	glutSwapBuffers();                    /* swap buffers */
}

/*---------------------------------------------------------------------
myinit(): Set up attributes and viewing
---------------------------------------------------------------------*/
void myinit()
{
	glClearColor(0.0f, 0.0f, 0.92f, 0.0f);    /* blue background*/

	/* set up viewing */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
	glMatrixMode(GL_MODELVIEW);
}