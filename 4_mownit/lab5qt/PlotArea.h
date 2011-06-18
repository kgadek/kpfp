#ifndef PLOTAREA_H_
#define PLOTAREA_H_

#include <QtGui/QtGui>

class PlotArea : public QWidget {
	Q_OBJECT
public:
	PlotArea(QWidget *parent = 0);
	QSize sizeHint() const;
protected:
	void mousePressEvent(QMouseEvent *);
	void paintEvent(QPaintEvent *);
private:
	QImage image;
};

#endif /* PLOTAREA_H_ */
