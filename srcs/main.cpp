#include "gui/mainwindow.hpp"
#include <iostream>
#include <QApplication>
#include "gui/Test.hpp"

int main(int argc, char *argv[])
{
	QApplication	a(argc, argv);
	Test			drawGizmo(60, nullptr, (char *)"Draw Gizmo");

	std::cout << "Hello cutie!" << std::endl;
	drawGizmo.show();

	return (a.exec());
}
