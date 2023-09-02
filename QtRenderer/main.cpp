#include "QtRenderer.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	QtRenderer w;
	w.show();
	return a.exec();
}