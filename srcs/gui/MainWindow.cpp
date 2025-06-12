#include "MainWindow.hpp"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow) {
	ui->setupUi(this);
	this->setWindowTitle("mod1");
	connect(ui->OpenGLWidget, )
}

MainWindow::~MainWindow() {
	delete ui;
}
