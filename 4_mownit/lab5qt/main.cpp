#include <QtGui/QApplication>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QWidget>
#include "PlotArea.h"

int main(int argc, char **argv) {

	QApplication app(argc, argv);

	QWidget *window = new QWidget;
	window->setWindowTitle("Blah");

	PlotArea *plot = new PlotArea();

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(plot);

	window->setLayout(layout);
	window->show();
	return app.exec();
}
