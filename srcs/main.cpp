#include "gui/MainWindow.hpp"
#include <iostream>
#include <QApplication>
#include "gui/OpenGLWidget.hpp"

int main(int argc, char *argv[])
{
	QApplication	a(argc, argv);
	// OpenGLWidget			drawCoordinate;
	MainWindow	window;

	std::cout << "Hello cutie!" << std::endl;
	// drawCoordinate.show();
	window.show();
	return (a.exec());
}
