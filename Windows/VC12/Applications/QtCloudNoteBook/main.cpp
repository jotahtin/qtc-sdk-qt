#include "qtcloudnotebook.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtCloudNoteBook w;
	w.show();
	return a.exec();
}
