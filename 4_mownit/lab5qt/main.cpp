#include <QtGui/QApplication>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QWidget>

int main(int argc, char **argv) {

	QApplication app(argc, argv);

	QWidget *window = new QWidget;
	window->setWindowTitle("Blah");

	window->show();
	return app.exec();
}
