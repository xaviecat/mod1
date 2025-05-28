#include "gui/mainwindow.hpp"

#include <iostream>
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication	a(argc, argv);
	MainWindow		w;

	std::cout << "Hello cutie!" << std::endl;

	w.show();
	return a.exec();
}
