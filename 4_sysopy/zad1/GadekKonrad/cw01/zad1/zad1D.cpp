#include <cstdio>
#include <cstdlib>
#include <cassert>


/* makra do uproszczenia zmiany alokacji z
   C (malloc/free) na C++ (new/delete) */
#define MEMALLOC(type) ((type*)malloc(sizeof(type)))
#define MEMFREE(obj) free(obj)


struct _avlNode {
	int key;			/* klucz w węźle */
	int bal;			/* wartosc balansu (potrzebne 3B) */
	struct _avlNode *l[2];		/* link do dzieci */
};
typedef struct _avlNode avlNode; /* trick dla kompatybilności z ANSI C */


void avlInit(avlNode*);			/* zainicjuj wierzcholek */
avlNode* avlQuery(avlNode *,int);	/* szukaj(1) */
avlNode* avlInsert(avlNode **,int);	/* dodaj */
void avlFree(avlNode*);			/* czysc pamiec */
/* (1): avlInsert zwraca wskaznik do dodanego wierzcholka lub do istniejacego
	o tym samym kluczu */


namespace print_tree {
        int a=1, b=0, c=5;
        void _print(avlNode*, int);
        void print_rev(avlNode*, int);
        void print_norm(avlNode*, int);
}


int main() {
	avlNode *r = NULL;
	int i;
	srand(0);
	for(i=0; i<10000000; i++)
		avlInsert(&r, 2);
		//avlInsert(&r, rand());
	print_tree::print_norm(r,10);
	return 0;
}

void avlInit(avlNode *x) {
	x->bal = 0;
	x->l[0] = x->l[1] = NULL;
}

avlNode* avlInsert(avlNode **R, int k) {
	/* A1-A9 to numery sektorow analogiczne jak w oryginale [1].
	   Zrodla:
	   	[1] Donald Knuth, Sztuka programowania, Tom 3 (oryg. The Art of Computer Programming),
			rozdz. 6.2.3: Balanced Trees
		[2] http://www.eternallyconfuzzled.com/tuts/datastructures/jsw_tut_avl.aspx
	 */
	avlNode *head, *p, *q, *r, *s, *t, *u;
	int a,d;
	if((*R) == NULL) {					/* A0: puste drzewo */
		(*R) = MEMALLOC(avlNode);	/* utworz node'a */
		assert( *R != NULL );
		avlInit(*R);			/* zainicjuj */
		(*R)->key = k;			/* ustaw mu wartosc */
		return *R;
	}
	u = NULL;				/* aby sie kompilator nie rzucal */
	head = MEMALLOC(avlNode); 		/* utworzenie HEAD */
	avlInit(head);
	head->l[1] = *R;					/* A1. inicjacja */
	t = head;
	p = s = *R;
	while(1) {						/* A2. szukanie */
		if(k == p->key)			/* exit_success */
			return p;
		d = (k > p->key);
		q = p->l[d];					/* A3/A4. przejscie lewo/prawo */
		if(q == NULL)
			break;
		if(q->bal != 0) {
			t = p;
			s = q;
		}
		p = q;
	}
	p->l[d] = u = q = MEMALLOC(avlNode);			/* A5. wstawianie */
	avlInit(q);
	q->key = k;
	r = p = s->l[ a = (k > s->key) ];			/* A6. poprawa wartosci balansow */
	a = 2*a - 1;
	while(p!=q) {				/* dla kazdego wierzcholka miedzy P a Q (bez Q) */
		d = (k > p->key);		/* 	wybierz kierunek */
		p->bal = 2*d - 1;		/* 	popraw balans */
		p = p->l[d];			/* 	przejdz dalej */
	}
	if(s->bal == 0) {				/* A7.i - lekkie zaburzenie balansu (+1 lub -1) */
		s->bal = a;				/* 	czyli drzewo uroslo */
		return u;
	} else if(s->bal == -a) {			/* A7.ii - wstawienie poprawilo balans drzewa */
		s->bal = 0;
		return u;
	}						/* A7.iii - zaburzenie balansu (+2 lub -2) */
	assert( s->bal == a );
	d = a<0?0:1;
	if(r->bal == a ) {				/* A8. pojedyncza rotacja */
		p = r;				/* rotuj w lewo/prawo (a = +1/-1) wokol wierzch. S */
		s->l[d] = r->l[!d];
		r->l[!d] = s;
		s->bal = r->bal = 0;
	} else {					/* A9. podwojna rotacja */
		p = r->l[!d];			/* rotuj w prawo/lewo (a = +1/-1) wokol wierz. R */
		r->l[!d] = p->l[d];		/* a nastepne */
		p->l[d] = r;			/* rotuj w lewo/prawo (a = +1/-1) wokol wierz. S */
		s->l[d] = p->l[!d];
		p->l[!d] = s;
		s->bal = r->bal = 0;		/* wartosci balansu S i R zaleza od podprzypadkow: */
		if(p->bal == a)			/*                       | (-a,0) , gdy p->bal == a */
			s->bal = -a;		/* ( s->bal , r->bal ) = | (0,0)  , gdy p->bal == 0*/
		else if(p->bal == -a)		/*                       | (0,a)  , gdy p->bal == -a */
			r->bal = a;
		p->bal = 0;
	}
	t->l[s == t->l[1]] = p;		/* wpp popraw wskazanie od ojca P */
	(*R) = head->l[1];
	MEMFREE(head);				/* pozbycie sie tymczasowego HEAD-a */
	return u;				/* zwroc wsk. do dodanego wierzcholka */
}

avlNode* avlQuery(avlNode *R, int k) {
	while(R && R->key != k)
		R = R->l[k > R->key];
	return R;
}

void avlFree(avlNode *x) {
	if(x==NULL)
		return;
	avlFree(x->l[0]);
	avlFree(x->l[1]);
	MEMFREE(x);
}

void print_tree::_print(avlNode *x, int h) {
	if(h<=0) return;
	if(x==NULL) {
		for(h=(1<<(h-1))-1;h>=0;--h)
			putchar('\n');
		return;
	}
	int i=a*(c*(h-1))+b+1;
	print_tree::_print(x->l[1],h-1);
	while(--i>0) putchar(' ');
	printf("%c[;%dm(%3d)%c[%dm\n",
			0x1B,32+x->bal,x->key,
			0x1B,0);
	print_tree::_print(x->l[0],h-1);
}

void print_tree::print_rev(avlNode *x, int h) {
	a=1;
	print_tree::_print(x,h);
}
void print_tree::print_norm(avlNode *x, int h) {
	/*dbg*///printf("__________________________________________________\n");
	a=-1;
	b+=(h-1)*c;
	print_tree::_print(x,h);
	b-=(h-1)*c;
}

