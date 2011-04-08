#include<gsl/gsl_interp.h>
#include<fstream>
#include<gsl/gsl_spline.h>
#define ILE_PKT 1000
using namespace std;

int main(int argc, char *argv[])
{
  int n,i;
  ifstream plik(argv[1]);
  ofstream plikout(argv[2]);
  double * tabx;
  double * taby;
  double** tabout;
if(plik!=NULL)
  {  
  plik>>n;
  tabx=new double[n];
  taby=new double[n];
	i=0;     
	while(i<n)
      {
	  plik>>tabx[i];
	  plik>>taby[i];
	i++;	
	}
    }
	else return 0;
	if(argv[3][0]=='P')
	{
	gsl_interp_accel *acc=gsl_interp_accel_alloc();
	gsl_interp *workspace=gsl_interp_alloc(gsl_interp_polynomial,n);
	gsl_interp_init(workspace,tabx,taby,n);
	double krok=(tabx[n-1]-tabx[0])/ILE_PKT;
	double x;
	for(x=tabx[0];x<=tabx[n-1];x+=krok)
	{
		plikout<<x<<" "<<gsl_interp_eval(workspace,tabx,taby,x,NULL)<<endl;
	}
gsl_interp_free(workspace);
gsl_interp_accel_free(acc);}
	if(argv[3][0]=='S')
	{
	gsl_interp_accel *acc=gsl_interp_accel_alloc();
	gsl_spline *spline=gsl_spline_alloc(gsl_interp_cspline,n);
	gsl_spline_init(spline,tabx,taby,n);
	double krok=(tabx[n-1]-tabx[0])/ILE_PKT;
	double x;
	for(x=tabx[0];x<=tabx[n-1];x+=krok)
	{
		plikout<<x<<" "<<gsl_spline_eval(spline,x,acc)<<endl;
	}
gsl_spline_free(spline);
gsl_interp_accel_free(acc);}
	if(argv[3][0]=='A')
	{
	gsl_interp_accel *acc=gsl_interp_accel_alloc();
	gsl_spline *akim=gsl_spline_alloc(gsl_interp_akima,n);
	gsl_spline_init(akim,tabx,taby,n);
	double krok=(tabx[n-1]-tabx[0])/ILE_PKT;
	double x;
	for(x=tabx[0];x<=tabx[n-1];x+=krok)
	{
		plikout<<x<<" "<<gsl_spline_eval(akim,x,acc)<<endl;
	}
gsl_spline_free(akim);
gsl_interp_accel_free(acc);}
plik.close();
plikout.close();
}
