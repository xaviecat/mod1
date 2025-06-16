#include "gui/mainwindow.hpp"
#include <iostream>
#include <QApplication>
#include "gui/OpenGLWidget.hpp"

int main(int argc, char *argv[])
{
	QApplication	a(argc, argv);
	OpenGLWidget			drawCoordinate;

	std::cout << "Hello cutie!" << std::endl;
	drawCoordinate.show();

	return (a.exec());
}
