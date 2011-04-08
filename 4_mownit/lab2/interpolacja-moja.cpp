#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<fstream>
#include<iostream>

#define N_PKT 1000

using namespace std;

class intLagrange {
 private:
	double *mianownik;
	int n;
 public:
	intLagrange(int x) : mianownik(new double[x]), n(x) {}
	void obliczMianownik(double **tab) {
		int i, j;
		for (i = 0; i < n; i++) {
			mianownik[i] = 1.0;
			for (j = 0; j < n; j++) {
				if (i != j)
					mianownik[i] *= (tab[i][0] - tab[j][0]);
			}
		}
	}
	double **obliczWyrazy(double **tab, int ile) {
		int i, j, k;
		double **tabout;
		tabout = new double *[ile];
		double krok = abs(tab[0][0] - tab[n - 1][0]) / (ile * 1.0);
		double start = tab[0][0];
		double element;
		for (k = 0; k < ile; k++) {
			tabout[k] = new double[2];
			tabout[k][0] = start + k * krok;
			tabout[k][1] = 0;
			for (i = 0; i < n; i++) {
				element = 1.0;
				for (j = 0; j < n; j++) {
					if (i != j)
						element *=
						    (tabout[k][0] - tab[j][0]);
				}
				tabout[k][1] +=
				    element / mianownik[i] * tab[i][1];
			}
		}
		return tabout;
	}
	void ustawDlugosc(int x) {
		n = x;
	}
};

class intNewton {
	double **ilorazRoznicowy;
	int n;
 public:
	intNewton(int x) : ilorazRoznicowy(new double* [x]), n(x) {}
	double rekobliczIloraz(double **tab, int i, int k) {
		if (i == k)
			return tab[k][1];
		else {
			return (rekobliczIloraz(tab, i + 1, k) -
				rekobliczIloraz(tab, i,
						k - 1)) / (tab[k][0] -
							   tab[i][0]);
		}
	}
	void obliczIloraz(double **tab) {
		int i, j;
		for (i = 0; i < n; i++) {
			ilorazRoznicowy[i] = new double[i + 1];
			ilorazRoznicowy[i][0] = tab[i][1];
		}
		for (i = 1; i < n; i++)
			for (j = 1; j < i + 1; j++) {
				ilorazRoznicowy[i][j] =
				    (ilorazRoznicowy[i][j - 1] -
				     ilorazRoznicowy[i - 1][j -
							    1]) / (tab[i][0] -
								   tab[i -
								       j][0]);
			}
	}
	double **obliczWyrazy(double **tab, int ile) {
		int i, k;
		double **tabout;
		tabout = new double *[ile];
		double krok = abs(tab[0][0] - tab[n - 1][0]) / (ile * 1.0);
		for (k = 0; k < ile; k++) {
			double zm = tab[0][0] + k * krok;
			tabout[k] = new double[2];
			tabout[k][0] = zm;
			tabout[k][1] = 0;
			double element = 1;
			for (i = 0; i < n - 1; i++) {
				element *= (zm - tab[i][0]);
				tabout[k][1] +=
				    element * ilorazRoznicowy[i + 1][i + 1];
			}
		}
		return tabout;
	}
	void ustawDlugosc(int x) {
		n = x;
	}
};

int main(int argc, char *argv[])
{
	int n, i;
	double **tab, **tabout;

	if (argc != 4) {
		printf("nieprawidlowa ilosc argumentow\n");
		return 0;
	}

	ifstream plik(argv[1]);
	ofstream plikout(argv[2]);
	if (plik == NULL) {
		return 0;
	}
	plik >> n;
	tab = new double *[n];
	i = 0;
	while (i < n) {
		tab[i] = new double[2];
		plik >> tab[i][0];
		plik >> tab[i][1];
		i++;
	}
	if (argv[3][0] == 'L' && !argv[3][1]) {
		intLagrange interp(n);
		interp.obliczMianownik(tab);
		tabout = interp.obliczWyrazy(tab, N_PKT);
	} else {
		intNewton interp2(n);
		interp2.obliczIloraz(tab);
		tabout = interp2.obliczWyrazy(tab, N_PKT);
	}
	for (i = 0; i < N_PKT; i++) {
		plikout << tabout[i][0] << " " << tabout[i][1] << endl;
	}
	plik.close();
	plikout.close();
	return 0;
}

