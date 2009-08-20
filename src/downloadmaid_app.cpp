#include "downloadmaid.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DownloadMaid w;
	w.showMaximized();
	return a.exec();
}
