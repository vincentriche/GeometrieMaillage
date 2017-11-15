#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gldisplay.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	public slots:
	void CreateTriangulation();
	void LoadPointsFile();
	void LoadOffFile();
	void SaveOffFile();
	void ResetAll();
	void ToggleWireframe();
	void DelaunayLawson();
	void Voronoi();
	void AddVoronoi();
	void Crust();

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
