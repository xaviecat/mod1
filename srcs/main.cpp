#include "gui/mainwindow.hpp"
#include <iostream>
#include <QApplication>
#include "gui/Test.hpp"

int main(int argc, char *argv[])
{
	QApplication	a(argc, argv);
	Test			drawCoordinate;

	std::cout << "Hello cutie!" << std::endl;
	drawCoordinate.show();

	return (a.exec());
}
