#include <QtGui/QtGui>
#include <QtCore/Qt>
#include "PlotArea.h"

template<class T> T min(T arg1, T arg2) { return arg1<arg2?arg1:arg2; }
template<class T> T max(T arg1, T arg2) { return arg1<arg2?arg2:arg1; }

PlotArea::~PlotArea() {
	if(x) {
		delete [] x;
		x = 0;
	}
	if(y) {
		delete [] y;
		y = 0;
	}
}

PlotArea::PlotArea(QWidget *parent) : QWidget(parent) {
	image = QImage(500,300,QImage::Format_ARGB32);
	d = 0.008;
	c = 0.07;
	b = 0.1;
	a = 0.1;
	y0 = 1000;
	x0 = 1;
	n = 10000;
	h = 0.1;

	x = new double[n];
	y = new double[n];
	xmax = x[0] = x0;
	y[0] = y0;

	for(int i=0; i+1<n; ++i) {
		x[i+1] = max(0.0, x[i] + h * ( -a*x[i] + c*d*x[i]*y[i] ));
		xmax = max(xmax, x[i+1]);
		y[i+1] = max(0.0, y[i] + h * ( b*y[i] - d*x[i]*y[i] ));
		xmax = max(xmax, y[i+1]);
	}
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

	for(int i=1;i<n;++i) {
		pathX.lineTo((double)i/n*image.width(), image.height() * (1 - x[i]/xmax));
		pathY.lineTo((double)i/n*image.width(), image.height() * (1 - y[i]/xmax));
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

