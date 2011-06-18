#ifndef PLOTAREA_H_
#define PLOTAREA_H_

#include <QtGui/QtGui>

template<class T> T min(T, T);
template<class T> T max(T, T);

class PlotArea : public QWidget {
	Q_OBJECT
public:
	PlotArea(QWidget *parent = 0);
	~PlotArea();
	QSize sizeHint() const;
public slots:
	void recalc(double,double,double,double,double,double,int,double);
protected:
	void paintEvent(QPaintEvent *);
private:
	double y0; //początkowa ilość ofiar
	double x0; //początkowa ilość drapieżników
	int n; //ilość kroków czasowych
	double h; //krok czasowy
	double *x; //populacja drapieżników
			//x_{n+1} = x_n + h (-a x_n + c d x_n y_n)
	double xmax;
	double *y; //populacja ofiar
			//y_{n+1} = y_n + h (b y_n - d x_n y_n)
	double a; //wsp. śmierci drapieżników z powodu braku ofiar
	double b; //wsp. narodzin ofiar, gdy nie ma drapieżników
	double c; //efektywność, z jaką drapieżnik wykorzystuje energię pozyskaną ze zjedzenia ofiar (0, 1]
	double d; //efektywność uśmiercania ofiar przez drapieżników

	QImage image;
	void calculate();
};

#endif /* PLOTAREA_H_ */
