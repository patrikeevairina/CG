#include <cstdio>
#include <stdlib.h>
#include <GL/glut.h>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define M_PI 3.14159265358979323846

float translations[6][3] = //массив для того, чтобы задавать направление движения граням
{
		{ 0.0, 0.0, -0.25 },
		{ 0.0, 0.25, 0.0 },
		{ 0.0, -0.25, 0.0 },
		{ 0.25, 0.0, 0.0 },
		{ -0.25, 0.0, 0.0 },
		{ 0.0, 0.0, 0.25 },
};

float currentTranslations[6][3] = //состояние граней в данный момент
{
		{ 0.0, 0.0, 0.0 },
		{ 0.0, 0.0, 0.0 },
		{ 0.0, 0.0, 0.0 },
		{ 0.0, 0.0, 0.0 },
		{ 0.0, 0.0, 0.0 },
		{ 0.0, 0.0, 0.0 },
};

float normals[6][3] = //массив нормалей
{
		{ 0.0, 0.0, -1.0 },
		{ 0.0, 1.0, 0.0 },
		{ 0.0, -1.0, 0.0 },
		{ 1.0, 0.0, 0.0 },
		{ -1.0, 0.0, 0.0 },
		{ 0.0, 0.0, 1.0 },
};

GLuint loadTexture(const char* filename) //загрузка текстуры
{
	GLuint texture;
	int w, h, count;//высота/ширина/кол-во каналов
	unsigned char* data;
	data = stbi_load(filename, &w, &h, &count, 0);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, count == 4 ? GL_RGBA : GL_RGB, w, h,
		0, count == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	return texture;
}

float vertices[72] = //массив вершин куба
{
		-0.5, 0.5, -0.5,
		-0.5, -0.5, -0.5,
		0.5, -0.5, -0.5,
		0.5, 0.5, -0.5,

		0.5, 0.5, -0.5,
		-0.5, 0.5, -0.5,
		-0.5, 0.5, 0.5,
		0.5, 0.5, 0.5,

		0.5, -0.5, 0.5,
		-0.5, -0.5, 0.5,
		-0.5, -0.5, -0.5,
		0.5, -0.5, -0.5,

		0.5, 0.5, 0.5,
		0.5, -0.5, 0.5,
		0.5, -0.5, -0.5,
		0.5, 0.5, -0.5,

		-0.5, 0.5, 0.5,
		-0.5, -0.5, 0.5,
		-0.5, -0.5, -0.5,
		-0.5, 0.5, -0.5,

		-0.5, 0.5, 0.5,
		-0.5, -0.5, 0.5,
		0.5, -0.5, 0.5,
		0.5, 0.5, 0.5
};

GLfloat texVertices[] = //вершины текстур
{
		0, 0, 1, 0, 1, 1, 0, 1,
		0, 0, 1, 0, 1, 1, 0, 1,
		0, 0, 1, 0, 1, 1, 0, 1,
		0, 0, 1, 0, 1, 1, 0, 1,
		0, 0, 1, 0, 1, 1, 0, 1,
		0, 0, 1, 0, 1, 1, 0, 1,
};

int oldTimeSinceStart = 0, textureMode = 0;
double time;
float alpha = 1.0;
bool openCube;
GLuint texture[7]; //массив текстур
void display()
{
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (float)(timeSinceStart - oldTimeSinceStart) / 1000;
	time += deltaTime * 0.4;
	oldTimeSinceStart = timeSinceStart;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//очистка буферов цвета и глубины
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		double w = glutGet(GLUT_WINDOW_WIDTH);
		double h = glutGet(GLUT_WINDOW_HEIGHT);
		double ar = w / h;
		glOrtho(-1.5 * ar, 1.5 * ar, -1.5, 1.5, -2, 2);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glDisable(GL_BLEND);
	GLfloat material_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; //как текстура взаимодействует со светом
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_diffuse);//внешняя и внутренняя стороны грани "активны"

	GLfloat pos[] = { (float)cos(time * M_PI) * 1.3f, 0.6, (float)sin(time * M_PI) * 1.3f, 1.0 };//движение источника света
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0 }; //цвет источника света

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); //цвет ИС
	glLightfv(GL_LIGHT0, GL_POSITION, pos); //позиция ИС
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0); //формула рассеивания 
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.2); //формула рассеивания 
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.4); //формула рассеивания 

	glDisable(GL_TEXTURE_2D);//отключить текстуру для сферы

	//изменение размеров  и поворот системы координат чтобы было красивее
	glScalef(0.75f, 0.75f, 0.75f);
	glRotatef(5, 1, 0, 0);
	glRotatef(5, 0, 1, 0);

	//создание сферы
	glTranslatef(pos[0], pos[1], pos[2]);
	glColor3d(1.0, 1.0, 1.0);
	glutWireSphere(0.1, 60, 60); 

	for (int i = 0; i < 6; ++i) 
	{
		auto t = translations[i];
		auto& ct = currentTranslations[i];
		float mn = (t[0] > 0 || t[1] > 0 || t[2] > 0) ? 0.2 : -0.2; //направление

		if (openCube) 
		{
			if (fabs(t[0]) > fabs(ct[0])) ct[0] += deltaTime * mn;
			if (fabs(t[1]) > fabs(ct[1])) ct[1] += deltaTime * mn;
			if (fabs(t[2]) > fabs(ct[2])) ct[2] += deltaTime * mn;
		}
		else 
		{
			if (fabs(t[0]) < 0.249 + fabs(ct[0]) && t[0] != 0) ct[0] -= deltaTime * mn;
			if (fabs(t[1]) < 0.249 + fabs(ct[1]) && t[1] != 0) ct[1] -= deltaTime * mn;
			if (fabs(t[2]) < 0.249 + fabs(ct[2]) && t[2] != 0) ct[2] -= deltaTime * mn;
		}

		glLoadIdentity();
		glEnable(GL_TEXTURE_2D);
		GLfloat material_diffuse[] = { 1.0, 1.0, 1.0, alpha };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_diffuse);
		glVertexPointer(3, GL_FLOAT, 0, vertices); //указатель на массив вершин
		glTexCoordPointer(2, GL_FLOAT, 0, texVertices); //указатель на вершины текстур
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glScalef(0.75f, 0.75f, 0.75f);
		glRotatef(15, 1, 0, 0);
		glRotatef(15, 0, 1, 0);

		glTranslatef(ct[0], ct[1], ct[2]);//раздвинуть/сдвинуть грани
		if (textureMode == 0) glBindTexture(GL_TEXTURE_2D, 0);
		if (textureMode == 1) glBindTexture(GL_TEXTURE_2D, texture[i]);
		if (textureMode == 2) glBindTexture(GL_TEXTURE_2D, texture[6]);
		glNormal3f(normals[i][0], normals[i][1], normals[i][2]);
		glDrawArrays(GL_QUADS, i * 4, 4);//нарисовать грани

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	glutSwapBuffers();
}

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	texture[0] = loadTexture("1.png");
	texture[1] = loadTexture("2.png");
	texture[2] = loadTexture("3.png");
	texture[3] = loadTexture("4.png");
	texture[4] = loadTexture("5.png");
	texture[5] = loadTexture("6.png");
	texture[6] = loadTexture("container.png");
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(300, 150);
	glutInitWindowSize(720, 640);
	glutCreateWindow("lab_3");
	init();

	glutDisplayFunc(display);

	glutReshapeFunc([](int width, int height) 
		{
			glViewport(0, 0, width, height);
		});

	glutIdleFunc([]() { glutPostRedisplay(); });//

	glutSpecialFunc([](int key, int x, int y) 
		{
			if (key == GLUT_KEY_UP) openCube = !openCube;
			if (key == GLUT_KEY_DOWN) alpha = alpha == 1 ? 0.3 : 1;
			if (key == GLUT_KEY_LEFT) textureMode += textureMode == 2 ? -2 : 1;
		}
	);
	glutMainLoop();

	return 0;
}
