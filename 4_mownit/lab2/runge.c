#include<fstream>
#include<gsl/gsl_interp.h>
#include<gsl/gsl_spline.h>

#define ILE_PKT_FUNKCJI 40
#define ILE_PKT 1000
using namespace std;

int main(int argc, char* argv[])
{
ofstream plikout(argv[1]);
double *tabx;
double *taby;
tabx=new double[40];
taby=new double[40];
double krok=2.0/(ILE_PKT_FUNKCJI);
double x;
int i=0;
for(x=-1.0;x<=1.0;x+=krok)
{
	tabx[i]=x;
	taby[i]=1.0/(1.0+25*x*x);
	i++;
}
krok=(tabx[ILE_PKT_FUNKCJI-1]-tabx[0])/(ILE_PKT-1);
if(argv[2][0]=='P')
	{
gsl_interp_accel *acc=gsl_interp_accel_alloc();
gsl_interp *workspace=gsl_interp_alloc(gsl_interp_polynomial,ILE_PKT_FUNKCJI);
gsl_interp_init(workspace,tabx,taby,ILE_PKT_FUNKCJI);
	for(x=tabx[0];x<=tabx[ILE_PKT_FUNKCJI-1];x+=krok)
	{
		plikout<<x<<" "<<gsl_interp_eval(workspace,tabx,taby,x,NULL)<<endl;
	}
gsl_interp_free(workspace);
gsl_interp_accel_free(acc);}
	if(argv[2][0]=='S')
	{
	gsl_interp_accel *acc=gsl_interp_accel_alloc();
	gsl_spline *spline=gsl_spline_alloc(gsl_interp_cspline,ILE_PKT_FUNKCJI);
	gsl_spline_init(spline,tabx,taby,ILE_PKT_FUNKCJI);
	for(x=tabx[0];x<=tabx[ILE_PKT_FUNKCJI-1];x+=krok)
	{
		plikout<<x<<" "<<gsl_spline_eval(spline,x,acc)<<endl;
	}
gsl_spline_free(spline);
gsl_interp_accel_free(acc);}
}
