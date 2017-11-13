#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	connect(ui->button1, SIGNAL(released()), this, SLOT(DrawWireframe()));
	connect(ui->button2, SIGNAL(released()), this, SLOT(DrawFaces()));
	connect(ui->button3, SIGNAL(released()), this, SLOT(DelaunayLawson()));
	connect(ui->button4, SIGNAL(released()), this, SLOT(Voronoi()));
	connect(ui->button5, SIGNAL(released()), this, SLOT(AddVoronoi()));
	connect(ui->button6, SIGNAL(released()), this, SLOT(Crust()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::DrawWireframe()
{
	ui->openGLWidget->GetTriangulation().SetGLRenderMode(GL_LINES);
	ui->openGLWidget->update();
}

void MainWindow::DrawFaces()
{
	ui->openGLWidget->GetTriangulation().SetGLRenderMode(GL_TRIANGLES);
	ui->openGLWidget->update();
}

void MainWindow::DelaunayLawson()
{
	ui->openGLWidget->GetTriangulation().DelaunayLawson();
	ui->openGLWidget->update();
}

void MainWindow::Voronoi()
{
	ui->openGLWidget->GetTriangulation().ToggleVoronoi();
	if (ui->openGLWidget->GetTriangulation().isVoronoi == true)
		ui->openGLWidget->GetTriangulation().Voronoi();
	ui->openGLWidget->update();
}

void MainWindow::AddVoronoi()
{
	ui->openGLWidget->GetTriangulation().AddVoronoi();
	ui->openGLWidget->update();
}

void MainWindow::Crust()
{
	ui->openGLWidget->GetTriangulation().ToggleCrust();
	ui->openGLWidget->update();
}