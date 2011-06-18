#include <cstdio>
#include <cstdlib>

const double y0 = 1000; //początkowa ilość ofiar
const double x0 = 1; //początkowa ilość drapieżników
const int n = 10000; //ilość kroków czasowych
const double h = 0.1; //krok czasowy

double *x; //populacja drapieżników
		//x_{n+1} = x_n + h (-a x_n + c d x_n y_n)
double *y; //populacja ofiar
		//y_{n+1} = y_n + h (b y_n - d x_n y_n)

const double a = 0.1; //wsp. śmierci drapieżników z powodu braku ofiar
const double b = 0.1; //wsp. narodzin ofiar, gdy nie ma drapieżników
const double c = 0.07; //efektywność, z jaką drapieżnik wykorzystuje energię pozyskaną ze zjedzenia ofiar (0, 1]
const double d = 0.008; //efektywność uśmiercania ofiar przez drapieżników

template<class T> T min(T arg1, T arg2) { return arg1<arg2?arg1:arg2; }
template<class T> T max(T arg1, T arg2) { return arg1<arg2?arg2:arg1; }

int main() {
	int i;

	x = new double[n];
	y = new double[n];
	x[0] = x0;
	y[0] = y0;

	for(i=0; i+1<n; ++i) {
		x[i+1] = max(0.0, x[i] + h * ( -a*x[i] + c*d*x[i]*y[i] ));
		y[i+1] = max(0.0, y[i] + h * ( b*y[i] - d*x[i]*y[i] ));
	}

	printf("#\tCzas\tDrapieznikow\tOfiar\n");
	for(i=0;i<n;++i)
		printf("%9d\t%8.3f\t%8.3f\n",i,x[i],y[i]);

	return 0;
}
