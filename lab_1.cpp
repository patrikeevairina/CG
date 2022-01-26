#include <stdio.h>
#include <gl/glut.h>
#include <math.h>

# define M_PI           3.14159265358979323846 

int pos[2] = { 300,150 };
int windowSize[2] = { 900, 600 };
float left, right, bottom, top;
int oldTimeSinceStart = 0;
float i = -1.0f;

struct Color 
{
	float red;
	float green;
	float blue;
	float alpha;

	Color(float r, float g, float b, float a = 1) 
	{
		red = r; green = g; blue = b; alpha = a;
	}

	Color darken(float darkenAmount) 
	{
		return Color(red * darkenAmount, green * darkenAmount, blue * darkenAmount, alpha * darkenAmount);
	}
};


void luminary(float cx, float cy, float i)
{
	int angles = 32;
	float r = 0.1f;
	glBegin(GL_TRIANGLE_FAN);
	auto color = i < 0 ? Color(0.9, 0.8, 0.3) : Color(0.9, 1, 1);
	glColor3f(color.red, color.green, color.blue);
	for (int i = 0; i < angles; ++i)
	{
		float theta = 2.0f * M_PI * i / angles;
		float x = r * cos(theta);
		float y = r * sin(theta);
		glVertex2f(x + cx, y + cy);
	}
	glEnd();
}

void grass(float d)
{
	glBegin(GL_POLYGON);
	auto color = Color(0.0, 0.9, 0.1).darken(d);
	glColor3f(color.red, color.green, color.blue);
	glVertex2f(left, 0.0);
	glVertex2f(right, 0.0);
	glVertex2f(right, bottom);
	glVertex2f(left, bottom);
	glEnd();
}


void sky(float d)
{
	glBegin(GL_POLYGON);
	auto color = Color(0.0, 0.7, 1.0).darken(d);
	glColor3f(color.red, color.green, color.blue);
	glVertex2f(left, 0.0);
	glVertex2f(right, 0.0);
	glVertex2f(right, top);
	glVertex2f(left, top);
	glEnd();

}

void house(float d)
{
	glBegin(GL_POLYGON);//truba
	auto color = Color(0.6, 0.4, 0.0).darken(d);
	glColor3f(color.red, color.green, color.blue);
	glVertex2f(right / 12, top / 2.6);
	glVertex2f(right / 7.8, top / 2.6);
	glVertex2f(right / 7.8, top / 6);
	glVertex2f(right / 12, top / 6);
	glEnd();

	glBegin(GL_POLYGON);//house
	color = Color(0.6, 0.4, 0.0).darken(d);
	glColor3f(color.red, color.green, color.blue);
	glVertex2f(left/7, -top / 5);
	glVertex2f(right/7, -top / 5);
	glVertex2f(right/7, top/6);
	glVertex2f(left/7, top/6);
	glEnd();

	glBegin(GL_TRIANGLES);//roof
	color = Color(0.7, 0.5, 0.0).darken(d);
	glColor3f(color.red, color.green, color.blue);
	glVertex2f(right / 5.6, top / 6.4);
	glVertex2f(left / 5.6, top / 6.4);
	glVertex2f(0.0, top / 2.4);
	glEnd();

	glBegin(GL_POLYGON);//door
	color = Color(0.5, 0.35, 0.0).darken(d);
	glColor3f(color.red, color.green, color.blue);
	glVertex2f(left / 12, -top / 5);
	glVertex2f(0.0, -top / 5);
	glVertex2f(0.0, top / 24);
	glVertex2f(left / 12, top / 24);
	glEnd();

	//glBegin(GL_POLYGON);//window
	//glColor3f(0.8, 0.8, 0.8);
	//glVertex2f(right / 7.5, top / 9);
	//glVertex2f(right / 3.9, top / 9);
	//glVertex2f(right / 3.9, top / 4);
	//glVertex2f(right / 7.5, top / 4);
	//glEnd();

	
}

void display()
{
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = timeSinceStart - oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;

	glClear(GL_COLOR_BUFFER_BIT);
	float d = 0.5;
	if (i < -0.5) {
		d += 1 + i;
	}
	else if (i < 0) {
		d += -i;
	}

	grass(d);
	sky(d);
	house(d);
	float preI = -i + (i < 0.0f ? 0.0f : 1.0f);
	luminary(1.1f * cos(preI * M_PI), 1.0f * sin(preI * M_PI), i);
	i += 0.0004f * deltaTime;
	if (i > 1.0f) i = -1.0f;
	glutSwapBuffers();
}

void timer(int value) 
{
	glutPostRedisplay();
	glutTimerFunc(30, timer, 0);
}

void resize(int width, int height)
{
	if (height == 0)
	{
		height = 1;
	}

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	float aspect = (float)width / (float)height;


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(0.0, (GLdouble)width, 0.0, (GLdouble)height);
	if (width <= height)
	{
		left = -1.0;
		right = 1.0;
		bottom = -1.0 / aspect;
		top = 1.0 / aspect;
	}
	else
	{
		left = -1.0 * aspect;
		right = 1.0 * aspect;
		bottom = -1.0;
		top = 1.0;
	}
	gluOrtho2D(left, right, bottom, top);
	display();
}

void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(pos[0], pos[1]);
	glutInitWindowSize(windowSize[0], windowSize[1]);
	glutCreateWindow("lab_1");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutTimerFunc(0, timer, 0);
	glutMainLoop();

	return 0;
}
