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

	//glLoadIdentity();
	glOrtho((float)mesh.GetAABB().GetMinAABB().getX(), (float)mesh.GetAABB().GetMaxAABB().getX(),
		(float)mesh.GetAABB().GetMinAABB().getY(), (float)mesh.GetAABB().GetMaxAABB().getY(),
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

/*
	Récupère la position du clic souris pour ajouter un point dans la triangulation.
*/
void GLDisplay::mousePressEvent(QMouseEvent *event)
{
	if (event == NULL || mesh.useTriangulationAlgorithms == false)
		return;

	if (event->buttons() & Qt::LeftButton)
	{
		_position = event->pos();

		double pX = _position.x();
		double pY = _position.y();

		pY = height() - pY;

		double minX = (double)mesh.GetAABB().GetMinAABB().getX();
		double maxX = (double)mesh.GetAABB().GetMaxAABB().getX();
		double minY = (double)mesh.GetAABB().GetMinAABB().getY();
		double maxY = (double)mesh.GetAABB().GetMaxAABB().getY();

		double sizeX = (maxX - minX);
		double sizeY = (maxY - minY);


		pX = pX / width() * sizeX + minX;
		pY = pY / height() * sizeY + minY;

		Vertex v = Vertex(Vector3(pX, pY, 0.0f));
		mesh.AddVertex(v);
		mesh.DelaunayLawsonIncremental();
		update();
	}
}

Triangulation& GLDisplay::GetTriangulation()
{
	return mesh;
}