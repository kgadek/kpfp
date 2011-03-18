#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NODEN(N) ((node*)(N))
#define NODEINT(N) (*(long long*)(N))

struct _node { 
	long long d, w;
	void *C[2];
	short iC[2];
};
typedef struct _node node;

node *r;
long long t,i,len;
char inp[1000002];
short ok;

node initNode(node* x) {
	x->d = x->w = x->iC[0] = x->iC[1] = 0;
}

void readStruct(node *p) {
	int cId = 0;
	long long z;
	while(i<len && cId < 2) {
		switch(inp[i]) {
			case '[' :
				++i;
				readStruct( NODEN( p->C[cId] = malloc(sizeof(node)) ));
				break;
			case ']' : return;
			case '?' :
				NODEINT( p->C[cId] = malloc(sizeof(long long)) ) = 0;
				p->iC[cId] = 1;
				break;
			case ',' :
				++cId;
				break;
			default :
				z = inp[i]-'0';
				while(i+1<len && inp[i+1]<='9' && inp[i+1] >= '0')
					z = 10*z + inp[++i] - '0';
				NODEINT( p->C[cId] = malloc(sizeof(long long)) ) = z;
				p->iC[cId] = 1;
		}
		++i;
	}
}

void parseTree(node* x) {
	if(!ok)
		return;
	x->d = 0;
	if(x->iC[0] && x->iC[1]) {
		long long L = NODEINT(x->C[0]), P = NODEINT(x->C[1]);
		if(!(L && P)) {
			if(L==P)
				x->w = x->d = 2;						// ? | ? --> w=2 d=2
			else
				x->w = 2*(L + P);						// N | ? --> w=2N d=0
		} else if(L == P)
			x->w = 2*L;									// N | N --> w=2N d=0
		else
			ok=0;
	} else {
		node *D[2] = {NULL, NULL};
		if(!x->iC[0])
			parseTree(D[0] = NODEN(x->C[0]));
		if(!x->iC[1])
			parseTree(D[1] = NODEN(x->C[1]));
		if(!ok) return;
		if(D[0] && D[1]) {								// [] | [] --> w=2(w1+a*d1) d=NWD(d1,d2)
			long long a, b, k;	// w1-w1 = -d1*a + d2*b
			if(D[0]->d < D[1]->d && D[0]->d) {
				x->d = D[0]->d;
				a=1;
				b=0;
			} else {
				x->d = D[1]->d;
				a=0;
				b=1;
			}
			k = D[0]->w - D[1]->w;
			if( ( x->d>0 && k%(x->d) ) || (!x->d && (D[0]->w != D[1]->w)) )
				ok = 0;
			if(x->d)
				x->w = (D[1]->w + b*(long long)(k / (x->d))*D[1]->d) << 1;
			else
				x->w = D[0]->w << 1;
			x->d = D[ x->d == D[0]->d ]->d;
		} else {
			long long c = (D[1] != NULL), N = NODEINT(x->C[1-c]);
			if(N) {										// [] | N --> w=2N d=0
				x->w=N<<1;
				N -= D[c]->w;
				if( N < 0 || (D[c]->d==0 && N!=0) || (D[c]->d && N%D[c]->d))
					ok = 0;
			} else {									// [] | ? --> w=2*w1 d=2*d1
				x->w = D[c]->w << 1;
				x->d = D[c]->d << 1;
			}
		}
	}
}

int main() {
	scanf("%d",&t);
	while(t--) {
		r = NODEN( malloc(sizeof(node)) );
		initNode(r);
		scanf(" %s",inp);
		ok = i = 1;
		len = strlen(inp);
		readStruct(r);
		parseTree(r);
		printf("%s\n",ok?"TAK":"NIE");
		free(r);
	}
	return 0;
}

