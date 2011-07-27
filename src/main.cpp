#include <QtCore/QTextCodec>

#include <QtGui/QApplication>
#include <QtGui/QLabel>


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

	QLabel label("<h1>Вітаю, сьвет!</h1>");
	label.show();

	return app.exec();
}
