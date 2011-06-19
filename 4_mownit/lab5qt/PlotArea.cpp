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

	for(int i=1;i<universe.getN();++i) {
		pathX.lineTo((double)i/universe.getN()*image.width(), image.height() * (1 - universe.getX(i)/universe.getMaxXY()));
		pathY.lineTo((double)i/universe.getN()*image.width(), image.height() * (1 - universe.getY(i)/universe.getMaxXY()));
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

void PlotArea::setA(int val) {
	this->universe.setA((double)val/sliderAmax);
	update();
}
void PlotArea::setB(int val) {
	this->universe.setB((double)val/sliderBmax);
	update();
}
void PlotArea::setC(int val) {
	this->universe.setC((double)val/sliderCmax);
	update();
}
void PlotArea::setD(int val) {
	this->universe.setD((double)val/sliderDmax);
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
	this->universe.setH((double)val/sliderHmax);
	update();
}
