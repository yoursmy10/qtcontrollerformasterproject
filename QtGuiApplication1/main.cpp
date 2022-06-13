
#include "multiwindow.h"
#include <QtWidgets/QApplication>







int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	QFile file(":/QSS/MacOS.qss");
	if (file.open(QFile::ReadOnly))
	{
		QString styleSheet = QLatin1String(file.readAll());

		a.setStyleSheet(styleSheet);
		file.close();
	}
	else
	{

		QMessageBox::warning(NULL, "warning", "Open failed", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	}
	
	
	multiwindow w;
	w.show();
	return a.exec();
}
