#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	connect(ui->button1, SIGNAL(released()), this, SLOT(DrawFaces()));
	connect(ui->button2, SIGNAL(released()), this, SLOT(DrawWireframe()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::DrawWireframe()
{
	ui->glDisplay->DrawWireframe();
}

void MainWindow::DrawFaces()
{
	ui->glDisplay->DrawFaces();
}
