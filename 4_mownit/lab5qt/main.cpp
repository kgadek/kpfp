#include <QtGui/QApplication>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QGridLayout>
#include "PlotArea.h"

int main(int argc, char **argv) {
	QApplication app(argc, argv);

	QWidget *window = new QWidget;
	window->setWindowTitle("Real Life simulator: zwierzyna kontra predator");

	QHBoxLayout *layout = new QHBoxLayout;
	QVBoxLayout *settings = new QVBoxLayout();

	PlotArea *plot = new PlotArea();
	layout->addWidget(plot);

	QLabel *labA = new QLabel("Wsp. smierci drapieznikow z powodu braku ofiar (0,1]");
	QSlider *slA = new QSlider(Qt::Horizontal);
	slA->setMaximum(1000);
	slA->setMinimum(1);
	slA->setValue(100);
	settings->addWidget(labA);
	settings->addWidget(slA);
	QObject::connect(slA,SIGNAL(valueChanged(int)),plot,SLOT(setA(int)));

	QLabel *labB = new QLabel("Wsp. narodzin ofiar, gdy nie ma drapieznikow (0,1]");
	QSlider *slB = new QSlider(Qt::Horizontal);
	slB->setMaximum(1000);
	slB->setMinimum(1);
	slB->setValue(100);
	settings->addWidget(labB);
	settings->addWidget(slB);
	QObject::connect(slB,SIGNAL(valueChanged(int)),plot,SLOT(setB(int)));

	QLabel *labC = new QLabel("Efektywnosc, z jaka drapieznik wykorzystuje energie\npozyskana ze zjedzenia ofiar (0,1]");
	QSlider *slC = new QSlider(Qt::Horizontal);
	slC->setMaximum(1000);
	slC->setMinimum(1);
	slC->setValue(70);
	settings->addWidget(labC);
	settings->addWidget(slC);
	QObject::connect(slC,SIGNAL(valueChanged(int)),plot,SLOT(setC(int)));

	QLabel *labD = new QLabel("Efektywnosc usmiercania ofiar przez\ndrapieznikow (0,1]");
	QSlider *slD = new QSlider(Qt::Horizontal);
	slD->setMaximum(1000);
	slD->setMinimum(1);
	slD->setValue(8);
	settings->addWidget(labD);
	settings->addWidget(slD);
	QObject::connect(slD,SIGNAL(valueChanged(int)),plot,SLOT(setD(int)));

	QLabel *labH = new QLabel("Krok czasowy (0,1]");
	QSlider *slH = new QSlider(Qt::Horizontal);
	slH->setMaximum(1000);
	slH->setMinimum(1);
	slH->setValue(100);
	settings->addWidget(labH);
	settings->addWidget(slH);
	QObject::connect(slH,SIGNAL(valueChanged(int)),plot,SLOT(setH(int)));

	QLabel *labN = new QLabel("Ilosc krokow (0,1000000]");
	QSlider *slN = new QSlider(Qt::Horizontal);
	slN->setMaximum(100000);
	slN->setMinimum(1);
	slN->setValue(10000);
	settings->addWidget(labN);
	settings->addWidget(slN);
	QObject::connect(slN,SIGNAL(valueChanged(int)),plot,SLOT(setN(int)));

	QLabel *labX = new QLabel("Poczatkowa ilosc drapieznikow\n(0,10000]");
	QSlider *slX = new QSlider(Qt::Horizontal);
	slX->setMaximum(1000);
	slX->setMinimum(1);
	slX->setValue(1);
	settings->addWidget(labX);
	settings->addWidget(slX);
	QObject::connect(slX,SIGNAL(valueChanged(int)),plot,SLOT(setX0(int)));

	QLabel *labY = new QLabel("Poczatkowa ilosc ofiar (0,10000]");
	QSlider *slY = new QSlider(Qt::Horizontal);
	slY->setMaximum(10000);
	slY->setMinimum(1);
	slY->setValue(1000);
	settings->addWidget(labY);
	settings->addWidget(slY);
	QObject::connect(slY,SIGNAL(valueChanged(int)),plot,SLOT(setY0(int)));

	layout->addLayout(settings);

	window->setLayout(layout);
	window->adjustSize();
	window->setFixedWidth(900);
	window->show();

	return app.exec();
}
