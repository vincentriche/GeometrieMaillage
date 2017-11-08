#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include <QOpenGLWidget>
#include <QMouseEvent>

#include <gasket.h>
#include <triangulation.h>

class GLDisplay : public QOpenGLWidget
{
	Q_OBJECT
public:
	explicit GLDisplay(QWidget *parent = 0);

	virtual void initializeGL();

	virtual void paintGL();

	virtual void resizeGL(int w, int h);


	virtual Triangulation& GetTriangulation();

protected:
	virtual void mouseMoveEvent ( QMouseEvent * event );
	virtual void mousePressEvent ( QMouseEvent * event );

private:
	Triangulation mesh;

	float _angle;
	QPoint _position;

signals:

public slots:

};

#endif // GLDISPLAY_H
