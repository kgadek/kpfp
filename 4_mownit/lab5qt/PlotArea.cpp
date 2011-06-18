#include <QtGui/QtGui>
#include <QtCore/Qt>
#include "PlotArea.h"

PlotArea::~PlotArea() {
}

PlotArea::PlotArea(QWidget *parent) : QWidget(parent) {
	image = QImage(500,300,QImage::Format_ARGB32);
	universe.calculate();
}


void PlotArea::paintEvent(QPaintEvent *) {
	QPainter painterX(this);
	QPainter painterY(this);
	painterX.setRenderHint(QPainter::Antialiasing);
	painterY.setRenderHint(QPainter::Antialiasing);

	QLinearGradient gradX;
	QLinearGradient gradY;

	gradX.setColorAt(0.0, QColor(255,0,0,255));
	gradX.setColorAt(1.0, QColor(255,127,0,255));
	painterX.setBrush(gradX);
	gradY.setColorAt(0.0, QColor(0,255,0,255));
	gradY.setColorAt(1.0, QColor(0,255,127,255));
	painterY.setBrush(gradY);

	QPainterPath pathX;
	QPainterPath pathY;
	pathX.moveTo(0,image.height());
	pathY.moveTo(0,image.height());

	for(int i=0;i<10;++i) {
		painterX.drawLine((double)i/10.0*image.width(),0,(double)i/10.0*image.width(),image.height());
		painterX.drawLine(0,(double)i/10.0*image.height(),image.width(),(double)i/10.0*image.height());
	}

	for(int i=1;i<universe.n;++i) {
		pathX.lineTo((double)i/universe.n*image.width(), image.height() * (1 - universe.x[i]/universe.xmax));
		pathY.lineTo((double)i/universe.n*image.width(), image.height() * (1 - universe.y[i]/universe.xmax));
	}
	pathX.lineTo(image.width(),image.height());
	pathY.lineTo(image.width(),image.height());

	pathX.closeSubpath();
	pathY.closeSubpath();

	painterY.drawPath(pathY);
	painterX.drawPath(pathX);

}

QSize PlotArea::sizeHint() const {
	return image.size();
}

QSize PlotArea::minimumSize() const {
	return image.size();
}

QSize PlotArea::maximumSize() const {
	return image.size();
}

void PlotArea::recalc(double a,double b,double c,double d,double x0,double y0,int n,double h) {
	this->universe.a = a;
	this->universe.b = b;
	this->universe.c = c;
	this->universe.d = d;
	this->universe.x0 = x0;
	this->universe.y0 = y0;
	this->universe.n = n;
	this->universe.h = h;
	this->universe.calculate();
	update();
}

void PlotArea::setA(int val) {
	this->universe.setA(val);
	update();
}
void PlotArea::setB(int val) {
	this->universe.setB(val);
	update();
}
void PlotArea::setC(int val) {
	this->universe.setC(val);
	update();
}
void PlotArea::setD(int val) {
	this->universe.setD(val);
	update();
}
void PlotArea::setX0(int val) {
	this->universe.setX0(val);
	update();
}
void PlotArea::setY0(int val) {
	this->universe.setY0(val);
	update();
}
void PlotArea::setN(int val) {
	this->universe.setN(val);
	update();
}
void PlotArea::setH(int val) {
	this->universe.setH(val);
	update();
}
