#include "gui/mainwindow.hpp"
#include <iostream>
#include <QApplication>
#include "gui/OpenGLWidget.hpp"
#include <unistd.h>
int main(int argc, char *argv[])
{
	// std::cout << getcwd(NULL, 0) << std::endl;
	QApplication	a(argc, argv);
	// MainWindow		w;
	OpenGLWidget			drawCoordinate;
	std::cout << "Hello cutie!" << std::endl;
	drawCoordinate.show();
	// w.show();
	return (a.exec());
}
