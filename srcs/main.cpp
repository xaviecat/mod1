#include "gui/mainwindow.hpp"

#include <iostream>
#include <QApplication>
// #include <Qt3DCore>
int main(int argc, char *argv[])
{
	QApplication	a(argc, argv);
	MainWindow		w;

	std::cout << "Hello cutie!" << std::endl;

	w.show();
	return a.exec();
}
