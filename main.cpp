#include <stdio.h>
#include <gl/glut.h>
#include <math.h>

# define M_PI           3.14159265358979323846 

int pos[2] = { 300,100 };
int windowSize[2] = { 700, 700 };
float left, right, bottom, top;
int oldTimeSinceStart = 0;
float i = -1.0f;
const int angles = 32;
float r = 0.1f;

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
	auto color = i < 0 ? Color(0.9, 0.8, 0.3) : Color(0.9, 1, 1);
	float vertices[angles * 2] = {};


	for (auto i = 0; i < angles * 2; i += 2) 
	{
		auto theta = 2.0 * M_PI * (i / 2) / angles;
		auto x = r * cos(theta);
		auto y = r * sin(theta);
		vertices[i] = x + cx;
		vertices[i + 1] = y + cy;
	}
	glColor3f(color.red, color.green, color.blue);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, &vertices);
	glDrawArrays(GL_TRIANGLE_FAN, 0, angles);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void grass(float d)
{
	auto color = Color(0.0, 0.9, 0.1).darken(d);
	glColor3f(color.red, color.green, color.blue);
	float vertices[] = 
	{
			-1, 0,
			1, 0,
			1, -1,
			-1, -1
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, &vertices);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
}


void sky(float d)
{
	auto color = Color(0.0, 0.7, 1.0).darken(d);
	glColor3f(color.red, color.green, color.blue);
	float vertices[] = 
	{
			-1, 1,
			1, 1,
			1, 0,
			-1, 0
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, &vertices);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void house(float d)
{
	{
		auto color = Color(0.6, 0.4, 0.0).darken(d);
		glColor3f(color.red, color.green, color.blue);
		float vertices[] = 
		{
				-0.25, 0.25,
				0.25, 0.25,
				0.25, -0.25,
				-0.25, -0.25
		};
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, &vertices);
		glDrawArrays(GL_QUADS, 0, 4);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	{
		float vertices2[] = 
		{
				0, 0.5,
				-0.25, 0.25,
				0.25, 0.25
		};
		auto color = Color(0.7, 0.5, 0.0).darken(d);
		glColor3f(color.red, color.green, color.blue);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, &vertices2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

}

void display()
{
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = timeSinceStart - oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;

	glClear(GL_COLOR_BUFFER_BIT);

	float d = 0.5;
	if (i < -0.5) 
	{
		d += 1 + i;
	}
	else if (i < 0) 
	{
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

const int fps = 1000 / 60;
void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(fps, timer, 0);
}

void resize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 0, -1.0, 1.0);
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
	glutCreateWindow("lab_2");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutTimerFunc(0, timer, 0);
	glutMainLoop();

	return 0;
}