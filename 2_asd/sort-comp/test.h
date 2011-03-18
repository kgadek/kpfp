/** @file test.h Nag��wek funkcji/klas testuj�cych.
 * Zawiera deklaracje klas i funkcji niezb�dnych do dzia�ania algorytm�w
 * jak i do przeprowadzenia test�w.
 * @warning Do ka�dego pliku z w�asnym algorytmem nale�y do��czy� ten plik.
 * @todo zmieni� typ danych - nie int ale szablony klas
 * @todo doda� sprawdzanie stabilno�ci algorytmu
 * @todo doda� obs�ug� b��d�w new/delete
 */

#ifndef TEST_H
#define TEST_H
#include <list>

extern int verbose; //0-verbose OFF, inny-verbose ON

/** B��d: dane nie zosta�y poprawnie posortowane. */
class ErrBadSortA {}; 
/** B��d TestCase - nie mo�na przywr�ci� pierwotnych danych. */
class ErrEmptyRev {};
/** B��d: algorytm odwo�a� si� do elementu poza tablic�. */
class ErrOutOfArray {
public:
	int x, /**< Nr elementu, do kt�rego nast�pi�o odwo�anie.  */
		maxx; /**< G�rny zakres tablicy [0,maxx).  */
	/**
	 * Konstruktor inicjuj�cy.
	 * Pobiera dane z klasy TestCase o szczeg�ach odwo�ania by mo�na by�o
	 * wy�wietli� szczeg�y b��du.
	 * @param _x wyst�pi�o odwo�anie do elementu o tym numerze.
	 * @param _maxx tablica ma zakres [0,size-1].
	 */
	ErrOutOfArray(int,int);
};

void terminator();

/**
 * Zestaw danych testuj�cych. Dane wczytywane s� z pliku tekstowego.
 */
class TestCase {
protected:
	static int id_cnt; /**< Licznik utworze� klasy. */
	char *filename; /**< Nazwa pliku z kt�rego wczytywane s� dane. TODO: doda� obs�ug� plik�w. */
	int *tab; /**< Przechowuje dane do sortowania. */
	int *rev; /**< Przechowuje orygina� danych. Pozwala przywr�ci� pierwotn� zawarto�� do *tab. */
public:
	int id; /**< Unikalny numer klasy. Nadawany w kolejno�ci, wg id_cnt. */
	int size; /**< Rozmiar danych - tablica [0,size-1]. */
	/**
	 * Konstruktor.
	 * Ustala id klasy oraz inicjuje wska�niki na zero.
	 */
	TestCase();
	/**
	 * Przeci��ony konstruktor.
	 * Dokonuje inicjacji id i wska�nik�w, wczytuje nazw� pliku.
	 * @param _fn nazwa pliku.
	 */
	TestCase(const char*);
	/**
	 * Destruktor.
	 * Zwalnia pami��.
	 * @see cleanup()
	 */
	~TestCase();
	/**
	 * Funkcja zamieniaj�ca elementy miejscami.
	 * @todo zaimplementowa�.
	 */
	void swap(int*,int*);
	/**
	 * Operator odwo�ania do element�w tablicy *tab.
	 * Przechwytuje pr�by odwo�ania poza tablic� zwracaj�c wyj�tek ErrOutOfArray.
	 * @param x wskazanie nr elementu.
	 * @return zwraca referencj� do obiektu.
	 * @exception ErrOutOfArray nast�puje, gdy nast�pi pr�ba wykroczenia poza
	 *		zakres tablicy.
	 * @bug Nie sprawdza, czy tablica zosta�a zadeklarowana.
	 */
	int& operator[](int);
	/**
	 * Kopiuje zawarto�� *tab do *rev celem p�niejszego przywr�cenia.
	 */
	virtual void copy();
	/**
	 * Przywraca oryginalnie nieposortowan� zawarto�� tablicy *tab.
	 */
	virtual void revert();
	/**
	 * Czy�ci pami�� i ustawia wska�niki na zero.
	 */
	virtual void cleanup();
	/**
	 * Wczytuje dane ze �r�d�a.
	 * @todo zaimplementowa� wczytywanie z pliku.
	 */
	virtual void read();
	
};

/**
 * Zestaw danych wczytywany z klawiatury.
 * Pochodna klasy TestCase, redefiniuje read() i destruktor.
 * @todo po��czy� z klas� bazow� - plikiem b�dzie stdin lub podobne.
 */
class TestCase_IO : public TestCase {
public:
	/**
	 * Wczytuje dane z klawiatury.
	 */
	void read();
	/**
	 * Destruktor dopasowany do klasy.
	 */
	~TestCase_IO();
};

/**
 * Zestaw danych przechowuj�cy ci�g arytmetyczny.
 * S�u�y zwykle jako sprawdzenie wydajno�ci algorytm�w dla ju� posortowanych
 * lub posortowanych w odwrotnej kolejno�ci danych.
 */
class TestCase_ASeries : public TestCase {
protected:
	// x_n = a + n*b ,0 <= n < size
	int a; /**< Pierwszy wyraz ci�gu. */
	int b; /**< R�nica ci�gu. */
public:
	/**
	 * Poprawiona klasa read, nie korzysta z zewn�trznych �r�de� danych.
	 */
	void read();
	/**
	 * Konstruktor wczytuj�cy parametry ci�gu.
	 * @param _a pierwszy wyraz ci�gu.
	 * @param _b r�nica ci�gu.
	 * @param _c ilo�� wyraz�w ci�gu.
	 */
	TestCase_ASeries(int,int,int);
	/**
	 * Destruktor dostosowany do klasy.
	 */
	~TestCase_ASeries();
	/**
	 * Pusta funkcja. Zb�dna gdy� dane mo�na wygenerowa�.
	 */
	void copy();
	/**
	 * Przywr�� oryginalne dane zgodnie ze wzorem a_n = a+n*b dla n=0..size-1
	 */
	void revert();
};

/**
 * Klasa bazowa dla algorytm�w sortowania.
 * Implementuje interfejs do przeprowadzania test�w i wymusza definicj�
 * funkcji sortuj�cej jak i funkcji wypisuj�cej nazw� algorytmu.
 */
class Sort {
public:
	/** Funkcja czysto wirtualna - wy�wietla nazw� algorytmu sortuj�cego. */
	virtual void name() = 0;
	/** Funkcja czysto wirtualna - algorytm sortuj�cy. */
	virtual void sort(TestCase&) = 0;
	/**
	 * Wywo�aj algorytm i przeprowad� badanie wynik�w.
	 * Zawiera obs�ug� b��d�w.
	 * @param in wska�nik do TestCase lub pochodnych
	 * @todo b��dy wyrzuca� na stderr
	 */
	void Run(TestCase*);
};

/**
 * G��wna klasa testuj�ca.
 * Przechowuje list� danych testowych i algorytm�w.\n
 * Wczytuje dane do zestawu i nast�pnie testuje wszystkie algorytmy na danym
 * te�cie. Potem usuwa dane testowe z pami�ci i przechodzi do nast�pnego zestawu.
 * @todo doda� bloki try..catch.
 *			poszuka� b��d�w.
 */
class SortComp {
private:
	/**
	 * Lista zestaw�w danych.
	 */
	static std::list<TestCase*> tList;
	/**
	 * Lista algorytm�w.
	 */
	static std::list<Sort*> aList;
public:
	/**
	 * Dodawanie algorytmu do listy.
	 * @param a wska�nik do obiektu klasy Sort lub pochodnej.
	 */
	void addAlgorithm(Sort* const);
	/**
	 * Dodawanie zestawu testowego do listy.
	 * @param t wska�nik do obiektu klasy TestCase lub pochodnej.
	 */
	void addTest(TestCase* const);
	/**
	 * Rozpocz�cie test�w.
	 */
	void Run() const;
	/**
	 * Wy�wietlenie listy testowanych algorytm�w.
	 */
	void ListAlg() const;
	/**
	 * Destruktor - czy�ci pami�� z pozosta�o�ci po testach jak i z algorytm�w.
	 */
	~SortComp();
};

#endif