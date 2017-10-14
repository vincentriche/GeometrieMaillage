#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include <QGLWidget>
#include <QMouseEvent>

#include <gasket.h>
#include <triangulation.h>

class GLDisplay : public QGLWidget
{
	Q_OBJECT
public:
	explicit GLDisplay(QWidget *parent = 0);

	virtual void initializeGL();

	virtual void paintGL();

	virtual void resizeGL(int w, int h);

	void DrawWireframe();
	void DrawFaces();

protected:
	virtual void mouseMoveEvent ( QMouseEvent * event );
	virtual void mousePressEvent ( QMouseEvent * event );

private:
	void drawSierpinski();
	Triangulation mesh;

	float _angle;
	QPoint _position;

signals:

public slots:

};

#endif // GLDISPLAY_H
