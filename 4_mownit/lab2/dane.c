#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main(int argc, char* argv[])
{
        if(argc!=3)
	  {
	    printf("zla liczba argumentow\n");
	    return 0;
	  }
	int a=atoi(argv[1]);
	srand(time(NULL));
	double** tab;
	double krok=0;
	tab=(double**)malloc(a*sizeof(double*));
	tab[0]=(double*)malloc(2*sizeof(double));
	tab[0][0]=-3.0;
	tab[0][1]=(rand()*2.0)/RAND_MAX*1.0;
	int i;
	for(i=1;i<a;i++)
	{
	  	krok=(rand()*1.0+1.0)/(RAND_MAX+2.0);
	  	tab[i]=(double*)malloc(2*sizeof(double));
	 	tab[i][0]=tab[i-1][0]+krok;
	  	tab[i][1]=((rand()%20)-3.0)/3.2;
	}
	FILE* plik=fopen(argv[2],"w");
	fprintf(plik,"%d\n",a);
	if(plik!=NULL)
	{
		for(i=0;i<a;i++)fprintf(plik,"%g %g\n",tab[i][0],tab[i][1]);	
	}
	fclose(plik);
}
