#include <iostream>

#include "gldisplay.h"
using namespace std;

GLDisplay::GLDisplay(QWidget *parent) :
	QGLWidget(parent),
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

	glOrtho((float)mesh.GetAABB().GetMinAABB().getX(), (float)mesh.GetAABB().GetMaxAABB().getX(),
			(float)mesh.GetAABB().GetMinAABB().getY(), (float)mesh.GetAABB().GetMaxAABB().getY(),
			(float)mesh.GetAABB().GetMinAABB().getZ(), (float)mesh.GetAABB().GetMaxAABB().getZ());

	glMatrixMode(GL_MODELVIEW);
}

void GLDisplay::mouseMoveEvent(QMouseEvent *event)
{
	if( event != NULL ) {
		QPoint position = event->pos();

		_angle += (position.x() - _position.x());

		_position = position;

		updateGL();
	}
}

void GLDisplay::mousePressEvent(QMouseEvent *event)
{
	if( event != NULL )
		_position = event->pos();
}


void GLDisplay::DrawWireframe()
{
	mesh.SetGLRenderMode(GL_LINES);
}

void GLDisplay::DrawFaces()
{
	mesh.SetGLRenderMode(GL_TRIANGLES);
}
