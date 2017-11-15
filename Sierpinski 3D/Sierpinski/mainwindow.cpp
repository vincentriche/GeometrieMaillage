#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qfiledialog.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	connect(ui->button1, SIGNAL(released()), this, SLOT(ToggleWireframe()));
	connect(ui->button2, SIGNAL(released()), this, SLOT(DelaunayLawson()));
	connect(ui->button3, SIGNAL(released()), this, SLOT(Voronoi()));
	connect(ui->button4, SIGNAL(released()), this, SLOT(AddVoronoi()));
	connect(ui->button5, SIGNAL(released()), this, SLOT(Crust()));
	connect(ui->button6, SIGNAL(released()), this, SLOT(CreateTriangulation()));
	connect(ui->button7, SIGNAL(released()), this, SLOT(LoadPointsFile()));
	connect(ui->button8, SIGNAL(released()), this, SLOT(LoadOffFile()));
	connect(ui->button9, SIGNAL(released()), this, SLOT(SaveOffFile()));
	connect(ui->button10, SIGNAL(released()), this, SLOT(ResetAll()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::ToggleWireframe()
{
	if(ui->openGLWidget->GetTriangulation().GetGLRenderMode() == GL_LINES)
		ui->openGLWidget->GetTriangulation().SetGLRenderMode(GL_TRIANGLES);
	else
		ui->openGLWidget->GetTriangulation().SetGLRenderMode(GL_LINES);
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

void MainWindow::CreateTriangulation()
{
	ui->openGLWidget->GetTriangulation().Reset();
	ui->openGLWidget->GetTriangulation().NaiveTriangulation();
	ui->openGLWidget->resizeGL(width(), height());
	ui->openGLWidget->update();
}

void MainWindow::LoadPointsFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "Files\\", tr("XY, TRI, TXT File (*.xy *.tri *.txt)"));

	if (fileName == NULL)
		return;
	
	ui->openGLWidget->GetTriangulation().Reset();
	ui->openGLWidget->GetTriangulation().NaiveFileTriangulation(fileName);
	ui->openGLWidget->resizeGL(width(), height());
	ui->openGLWidget->update();
}

void MainWindow::LoadOffFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "Files\\", tr("OFF File (*.off)"));

	if (fileName == NULL)
		return;

	ui->openGLWidget->GetTriangulation().Reset();
	ui->openGLWidget->GetTriangulation().ReadOffFile(fileName);
	ui->openGLWidget->resizeGL(width(), height());
	ui->openGLWidget->update();
}

void MainWindow::SaveOffFile()
{
	ui->openGLWidget->GetTriangulation().SaveOffFile();
	ui->openGLWidget->resizeGL(width(), height());
	ui->openGLWidget->update();
}

void MainWindow::ResetAll()
{
	ui->openGLWidget->GetTriangulation().Reset();
	ui->openGLWidget->resizeGL(width(), height());
	ui->openGLWidget->update();
}