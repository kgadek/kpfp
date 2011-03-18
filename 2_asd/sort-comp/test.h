/** @file test.h Nag³ówek funkcji/klas testuj¹cych.
 * Zawiera deklaracje klas i funkcji niezbêdnych do dzia³ania algorytmów
 * jak i do przeprowadzenia testów.
 * @warning Do ka¿dego pliku z w³asnym algorytmem nale¿y do³¹czyæ ten plik.
 * @todo zmieniæ typ danych - nie int ale szablony klas
 * @todo dodaæ sprawdzanie stabilnoœci algorytmu
 * @todo dodaæ obs³ugê b³êdów new/delete
 */

#ifndef TEST_H
#define TEST_H
#include <list>

extern int verbose; //0-verbose OFF, inny-verbose ON

/** B³¹d: dane nie zosta³y poprawnie posortowane. */
class ErrBadSortA {}; 
/** B³¹d TestCase - nie mo¿na przywróciæ pierwotnych danych. */
class ErrEmptyRev {};
/** B³¹d: algorytm odwo³a³ siê do elementu poza tablic¹. */
class ErrOutOfArray {
public:
	int x, /**< Nr elementu, do którego nast¹pi³o odwo³anie.  */
		maxx; /**< Górny zakres tablicy [0,maxx).  */
	/**
	 * Konstruktor inicjuj¹cy.
	 * Pobiera dane z klasy TestCase o szczegó³ach odwo³ania by mo¿na by³o
	 * wyœwietliæ szczegó³y b³êdu.
	 * @param _x wyst¹pi³o odwo³anie do elementu o tym numerze.
	 * @param _maxx tablica ma zakres [0,size-1].
	 */
	ErrOutOfArray(int,int);
};

void terminator();

/**
 * Zestaw danych testuj¹cych. Dane wczytywane s¹ z pliku tekstowego.
 */
class TestCase {
protected:
	static int id_cnt; /**< Licznik utworzeñ klasy. */
	char *filename; /**< Nazwa pliku z którego wczytywane s¹ dane. TODO: dodaæ obs³ugê plików. */
	int *tab; /**< Przechowuje dane do sortowania. */
	int *rev; /**< Przechowuje orygina³ danych. Pozwala przywróciæ pierwotn¹ zawartoœæ do *tab. */
public:
	int id; /**< Unikalny numer klasy. Nadawany w kolejnoœci, wg id_cnt. */
	int size; /**< Rozmiar danych - tablica [0,size-1]. */
	/**
	 * Konstruktor.
	 * Ustala id klasy oraz inicjuje wskaŸniki na zero.
	 */
	TestCase();
	/**
	 * Przeci¹¿ony konstruktor.
	 * Dokonuje inicjacji id i wskaŸników, wczytuje nazwê pliku.
	 * @param _fn nazwa pliku.
	 */
	TestCase(const char*);
	/**
	 * Destruktor.
	 * Zwalnia pamiêæ.
	 * @see cleanup()
	 */
	~TestCase();
	/**
	 * Funkcja zamieniaj¹ca elementy miejscami.
	 * @todo zaimplementowaæ.
	 */
	void swap(int*,int*);
	/**
	 * Operator odwo³ania do elementów tablicy *tab.
	 * Przechwytuje próby odwo³ania poza tablicê zwracaj¹c wyj¹tek ErrOutOfArray.
	 * @param x wskazanie nr elementu.
	 * @return zwraca referencjê do obiektu.
	 * @exception ErrOutOfArray nastêpuje, gdy nast¹pi próba wykroczenia poza
	 *		zakres tablicy.
	 * @bug Nie sprawdza, czy tablica zosta³a zadeklarowana.
	 */
	int& operator[](int);
	/**
	 * Kopiuje zawartoœæ *tab do *rev celem póŸniejszego przywrócenia.
	 */
	virtual void copy();
	/**
	 * Przywraca oryginalnie nieposortowan¹ zawartoœæ tablicy *tab.
	 */
	virtual void revert();
	/**
	 * Czyœci pamiêæ i ustawia wskaŸniki na zero.
	 */
	virtual void cleanup();
	/**
	 * Wczytuje dane ze Ÿród³a.
	 * @todo zaimplementowaæ wczytywanie z pliku.
	 */
	virtual void read();
	
};

/**
 * Zestaw danych wczytywany z klawiatury.
 * Pochodna klasy TestCase, redefiniuje read() i destruktor.
 * @todo po³¹czyæ z klas¹ bazow¹ - plikiem bêdzie stdin lub podobne.
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
 * Zestaw danych przechowuj¹cy ci¹g arytmetyczny.
 * S³u¿y zwykle jako sprawdzenie wydajnoœci algorytmów dla ju¿ posortowanych
 * lub posortowanych w odwrotnej kolejnoœci danych.
 */
class TestCase_ASeries : public TestCase {
protected:
	// x_n = a + n*b ,0 <= n < size
	int a; /**< Pierwszy wyraz ci¹gu. */
	int b; /**< Ró¿nica ci¹gu. */
public:
	/**
	 * Poprawiona klasa read, nie korzysta z zewnêtrznych Ÿróde³ danych.
	 */
	void read();
	/**
	 * Konstruktor wczytuj¹cy parametry ci¹gu.
	 * @param _a pierwszy wyraz ci¹gu.
	 * @param _b ró¿nica ci¹gu.
	 * @param _c iloœæ wyrazów ci¹gu.
	 */
	TestCase_ASeries(int,int,int);
	/**
	 * Destruktor dostosowany do klasy.
	 */
	~TestCase_ASeries();
	/**
	 * Pusta funkcja. Zbêdna gdy¿ dane mo¿na wygenerowaæ.
	 */
	void copy();
	/**
	 * Przywróæ oryginalne dane zgodnie ze wzorem a_n = a+n*b dla n=0..size-1
	 */
	void revert();
};

/**
 * Klasa bazowa dla algorytmów sortowania.
 * Implementuje interfejs do przeprowadzania testów i wymusza definicjê
 * funkcji sortuj¹cej jak i funkcji wypisuj¹cej nazwê algorytmu.
 */
class Sort {
public:
	/** Funkcja czysto wirtualna - wyœwietla nazwê algorytmu sortuj¹cego. */
	virtual void name() = 0;
	/** Funkcja czysto wirtualna - algorytm sortuj¹cy. */
	virtual void sort(TestCase&) = 0;
	/**
	 * Wywo³aj algorytm i przeprowadŸ badanie wyników.
	 * Zawiera obs³ugê b³êdów.
	 * @param in wskaŸnik do TestCase lub pochodnych
	 * @todo b³êdy wyrzucaæ na stderr
	 */
	void Run(TestCase*);
};

/**
 * G³ówna klasa testuj¹ca.
 * Przechowuje listê danych testowych i algorytmów.\n
 * Wczytuje dane do zestawu i nastêpnie testuje wszystkie algorytmy na danym
 * teœcie. Potem usuwa dane testowe z pamiêci i przechodzi do nastêpnego zestawu.
 * @todo dodaæ bloki try..catch.
 *			poszukaæ b³êdów.
 */
class SortComp {
private:
	/**
	 * Lista zestawów danych.
	 */
	static std::list<TestCase*> tList;
	/**
	 * Lista algorytmów.
	 */
	static std::list<Sort*> aList;
public:
	/**
	 * Dodawanie algorytmu do listy.
	 * @param a wskaŸnik do obiektu klasy Sort lub pochodnej.
	 */
	void addAlgorithm(Sort* const);
	/**
	 * Dodawanie zestawu testowego do listy.
	 * @param t wskaŸnik do obiektu klasy TestCase lub pochodnej.
	 */
	void addTest(TestCase* const);
	/**
	 * Rozpoczêcie testów.
	 */
	void Run() const;
	/**
	 * Wyœwietlenie listy testowanych algorytmów.
	 */
	void ListAlg() const;
	/**
	 * Destruktor - czyœci pamiêæ z pozosta³oœci po testach jak i z algorytmów.
	 */
	~SortComp();
};

#endif