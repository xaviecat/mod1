#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "OpenGLWidget.hpp"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow) {
	ui->setupUi(this);
	this->setWindowTitle("mod1");
}

MainWindow::~MainWindow() {
	delete ui;
}
