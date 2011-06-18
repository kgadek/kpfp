#include <QtGui/QtGui>
#include <QtCore/Qt>
#include "PlotArea.h"

PlotArea::PlotArea(QWidget *parent) : QWidget(parent) {
	image = QImage(100,100,QImage::Format_ARGB32);
}


void PlotArea::paintEvent(QPaintEvent *) {
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	QLinearGradient grad;
	grad.setColorAt(0.0, QColor(255,0,0,255));
	grad.setColorAt(1.0, QColor(255,127,0,255));
	painter.setBrush(grad);

//	painter.drawLine(10,10,0,0);
//	painter.drawLine(10,10,5,5);

	QPainterPath path;
	path.lineTo(10,10);
	path.lineTo(100,0);

	path.closeSubpath();
	painter.drawPath(path);
}

QSize PlotArea::sizeHint() const {
	return image.size();
}


void PlotArea::mousePressEvent(QMouseEvent *evt) {

}
