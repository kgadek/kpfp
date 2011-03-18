#include <cstdio>
#include <cstdlib>
#include <cmath>

int deg=0, id=0, hIn=0;
double h=1.0, x=1.0, *wspTab=NULL;

void showHelp(char* progname, int quitStat, const char *msg=NULL) {
	printf( "%sUzycie programu:\n  %s id ...\ngdzie:\n"
			"  id - obliczana funkcja:\n"
			"       1    - wielomian. Nastepnie:\n"
			"                x - punkt wokol ktorego obliczamy pochodna\n"
			"                D - stopien wielomianu (l. calkowita)\n"
			"                h - probkowanie pochodnej nast. w odleglosci 10^(-h), h=0, ..., 5\n"
			"                aD, ..., a1, a0 - wsp. wielomianu\n"
			"       2, 3 - funkcja sin(ax)/exp(ax). Nastepnie:\n"
			"                x - punkt wokol ktorego obliczamy pochodna\n"
			"                a - wspolczynnik\n"
			"                h - probkowanie pochodnej nast. w odleglosci 10^(-h), h=0, ..., 5\n", msg?msg:"",progname);
	exit(quitStat);
}
#define RES(a,b) 20, (a), 23, ((b)-(a)), 23, 100*(a-b)/(b)
#define SEPARATE "+---------------------------------+----------------------+---------------------------------------------------+\n"
void showRes(double exD, double d1R, double d1L, double d1C, double exDD, double d2C) {
	printf(
		"  Opis                              Wynik                  Błąd bezwzględny i względny\n" SEPARATE
		"| f'(x)  metoda roznicy prawostr. | %+*.3f | %+*.3e %+*.3e %% |\n"
		"| f'(x)  metoda roznicy lewostr.  | %+*.3f | %+*.3e %+*.3e %% |\n"
		"| f'(x)  metoda roznicy centr.    | %+*.3f | %+*.3e %+*.3e %% |\n" SEPARATE
		"| f'(x)  wartosc dokladna         | %+*.3f | -   -   -   -   -   -   -   -   -   -   -   -   - |\n" SEPARATE
		"| f''(x) metoda roznicy centr.    | %+*.3f | %+*.3e %+*.3e %% |\n" SEPARATE
		"| f''(x) wartosc dokladna         | %+*.3f | -   -   -   -   -   -   -   -   -   -   -   -   - |\n" SEPARATE,
		RES(d1R,exD), RES(d1L,exD), RES(d1C,exD), 20, exD, RES(d2C,exDD), 20, exDD);
}

double d1(double (*fun)(double, int), int left=1) { return (fun(x+h*left,0) - fun(x,0))/(h*left); }
double d1b (double (*fun)(double,int)) { return (fun(x+h,0)-fun(x-h,0))/(h*2); }
double d2b (double (*fun)(double,int)) { return (fun(x+h,0)+fun(x-h,0)-2*fun(x,0))/(h*h); }
double wspWielD(double a, double xx, int n, int d) {
	if( abs(a)<0.0001 || d>n+1) return 0;
	if( d==0 ) return a*pow(xx,n);
	return wspWielD(a*n, xx, n-1, d-1);
}
double fPoly(double xx, int d) {
	double res = 0.0;
	for(int i=0;i<=deg;++i) res += wspWielD(wspTab[i],xx,deg-i,d);
	return res;
}
double fSin(double xx, int d) { return pow(wspTab[0],d)*pow(-1,(int)(d/2))*( d%2?cos(wspTab[0]*xx):sin(wspTab[0]*xx) ); }
double fExp(double xx, int d) { return pow(wspTab[0],d)*exp(wspTab[0]*xx); }

int main(int argc, char **argv) {
	if( argc<3 || (id=atoi(argv[1]))==0 || 3<id || id<1)
		showHelp(argv[0],argc>1, argc!=1?"Podano nieprawidlowa ilosc argumentow lub sa one niepoprawne!\n":"");
	x=atof(argv[2]);
	if(argc>4) hIn=atoi(argv[4]);
	if(			(id==1 && (argc<5 || (deg=atoi(argv[3]))==0 || 5<deg || deg<3 ||
						(wspTab=new double[deg+1])==NULL || 5<hIn || hIn<0 || argc<6+deg)) 
			|| (id>1 && (argc<5 || 5<hIn || hIn<0 || (wspTab=new double[1])==NULL ))) 
		showHelp(argv[0],1,"Podano nieprawidlowe lub niepoprawne parametry!\n");
	h = pow(10,-hIn);
	for(int i=0;i<=deg;++i) wspTab[i] = atof(argv[i+3+2*(id==1)]);
	switch(id) {
		case 1: printf("f(x) = ");
				for(int i=0;i<deg;++i) printf("%c%.3f x^%d ",(i==0)?' ':(wspTab[i]<0?0:'+'),wspTab[i],deg-i);
				printf("%c%.3f,\t\t",(wspTab[deg]<0)?0:'+',wspTab[deg]);
			break;
		default: printf("f(x) = %s(%.3f x),\t\t",(id==2)?"sin":"exp",wspTab[0]);
	}
	printf("h = %.5f,\tx = %.3f\n",h,x);
	double (*fTab[])(double,int) = {fPoly,fSin,fExp};
	showRes( fTab[id-1](x,1), d1(fTab[id-1]), d1(fTab[id-1],-1), d1b(fTab[id-1]), fTab[id-1](x,2), d2b(fTab[id-1]) );
	delete [] wspTab;
	return 0;
}

