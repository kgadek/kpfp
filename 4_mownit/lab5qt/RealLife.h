#ifndef REALLIFE_H_
#define REALLIFE_H_

template<class T> T min(T, T);
template<class T> T max(T, T);

class RealLife {
	double y0; //początkowa ilość ofiar
	double x0; //początkowa ilość drapieżników
	int n; //ilość kroków czasowych
	double h; //krok czasowy
	double *x; //populacja drapieżników
			//x_{n+1} = x_n + h (-a x_n + c d x_n y_n)
	double maxXY;
	double *y; //populacja ofiar
			//y_{n+1} = y_n + h (b y_n - d x_n y_n)
	double a; //wsp. śmierci drapieżników z powodu braku ofiar
	double b; //wsp. narodzin ofiar, gdy nie ma drapieżników
	double c; //efektywność, z jaką drapieżnik wykorzystuje energię pozyskaną ze zjedzenia ofiar (0, 1]
	double d; //efektywność uśmiercania ofiar przez drapieżników
public:
	void setA(double);
	void setB(double);
	void setC(double);
	void setD(double);
	void setX0(int);
	void setY0(int);
	void setN(int);
	void setH(double);

	double getX(int);
	double getY(int);
	int getN();
	double getMaxXY();

	RealLife();
	~RealLife();
	void calculate();
};

#endif /* REALLIFE_H_ */
