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

	QHBoxLayout *layout = new QHBoxLayout;

	PlotArea *plot = new PlotArea();
	layout->addWidget(plot);

	window->setLayout(layout);
	window->show();

	return app.exec();
}
