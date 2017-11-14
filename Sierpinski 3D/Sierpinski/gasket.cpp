#include <Windows.h>
#include <cstdlib>
#include <time.h>
#include <GL/gl.h>

#include "gasket.h"

#define POINTS 6000
#define CONTROL_VERTICES 4

Gasket::Gasket()
{
	point vertices[CONTROL_VERTICES] = {
		{-1.0f, -1.0f, 1.0f},
		{0.0f,  1.0f, 1.0f},
		{1.0f, -1.0f, 1.0f},
		{0.0f, 0.0f, -1.0f}
	};

	points = new point[POINTS];

	point p = { 0.0, 0.0, 0.0 };

	srand(time(NULL));

	for (int i = 0; i < POINTS; i++) {
		int index = rand() % CONTROL_VERTICES;

		point newPoint = { (vertices[index].x + p.x) / 2.0f,
						   (vertices[index].y + p.y) / 2.0f,
						   (vertices[index].z + p.z) / 2.0f };

		p = newPoint;
		points[i] = newPoint;
	}
}

void Gasket::draw()
{
	glBegin(GL_POINTS);

	for (int i = 0; i < POINTS; i++) {
		glVertex3f(points[i].x, points[i].y, points[i].z);
	}

	glEnd();
}
