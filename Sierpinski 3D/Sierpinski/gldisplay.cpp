#include <iostream>

#include "gldisplay.h"
using namespace std;

GLDisplay::GLDisplay(QWidget *parent) :
	QOpenGLWidget(parent),
	_angle(0.0f)
{
}

void GLDisplay::initializeGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glColor3f(1.0, 1.0, 0.0);
}

void GLDisplay::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glRotatef(_angle, 0.0f, 1.0f, 0.0f);

	mesh.draw();
}

void GLDisplay::resizeGL(int w, int h)
{
	glMatrixMode(GL_PROJECTION);

	glViewport(0, 0, w, h);


	glOrtho((float)mesh.AABB().GetMinAABB().getX(), (float)mesh.AABB().GetMaxAABB().getX(),
		(float)mesh.AABB().GetMinAABB().getY(), (float)mesh.AABB().GetMaxAABB().getY(),
		-2.0f, 100.f);

	glMatrixMode(GL_MODELVIEW);
}

void GLDisplay::mouseMoveEvent(QMouseEvent *event)
{
	if (event == NULL)
		return;

	if (event->buttons() & Qt::RightButton)
	{
		QPoint position = event->pos();
		_angle += (position.x() - _position.x());
		_position = position;

		update();
	}
}

void GLDisplay::mousePressEvent(QMouseEvent *event)
{
	if (event == NULL)
		return;

	if (event->buttons() & Qt::LeftButton)
	{
		_position = event->pos();

		double pX = _position.x();
		double pY = _position.y();

		pY = height() - pY;

		double minX = (double)mesh.AABB().GetMinAABB().getX();
		double maxX = (double)mesh.AABB().GetMaxAABB().getX();
		double minY = (double)mesh.AABB().GetMinAABB().getY();
		double maxY = (double)mesh.AABB().GetMaxAABB().getY();

		double sizeX = (maxX - minX);
		double sizeY = (maxY - minY);


		pX = pX / width() * sizeX + minX;
		pY = pY / height() * sizeY + minY;

		mesh.AddVertex(Vertex(Vector3(pX, pY, 0.0f)));
		mesh.DelaunayLawson();
		update();
	}
}

Triangulation& GLDisplay::GetTriangulation()
{
	return mesh;
}