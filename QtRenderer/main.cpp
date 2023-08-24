#include "QtRenderer.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	QtRenderer w;
	w.show();
	return a.exec();
}



//#include <tbb/tbb.h>
//#include <iostream>
//
//using namespace std;
//using namespace tbb;
//
//int main()
//{
//	parallel_for(0, 10, [](int num) {cout << num << ": Hello TBB!" << endl; });
//	getchar();
//	return 0;
//}