#ifndef PLOTAREA_H_
#define PLOTAREA_H_

#include <QtGui/QtGui>
#include "RealLife.h"

class PlotArea : public QWidget {
	Q_OBJECT
public:
	PlotArea(QWidget *parent = 0);
	~PlotArea();
	QSize sizeHint() const;
	QSize maximumSize() const;
	QSize minimumSize() const;
public slots:
	void recalc(double,double,double,double,double,double,int,double);
	void setA(int);
	void setB(int);
	void setC(int);
	void setD(int);
	void setX0(int);
	void setY0(int);
	void setN(int);
	void setH(int);
protected:
	void paintEvent(QPaintEvent *);
private:
	QImage image;
	RealLife universe;
};

#endif /* PLOTAREA_H_ */
