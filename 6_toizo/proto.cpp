#include <cstdio>

int X, Y;

enum cell_type { SIMPLE, SOURCE, LIGHT };

struct cell {
	cell(cell_type _type, char _w0, char _w1, char _w2, char _w3, char _rot
		//, char *_name
		)
			: type(_type), rot(_rot), poss(4/rot)
			//,name((_name[0]-'A')<<4 | (_name[1]-1))
	{
		wire[0] = _w0;
		wire[1] = _w1;
		wire[2] = _w2;
		wire[3] = _w3;
	}
	cell() : type(SIMPLE), rot(4)
	//, name(0)
	{
		wire[0] = wire[1] = wire[2] = wire[3] = 0;
		poss = 4/rot;
	}

	enum cell_type type;
	char wire[4]; // top right down left <=> NESW
	char rot;
	char poss;
	//char name;
};

const char *names[] = {
	"A1", "A1", "A1", "A1",
	"B1", "B2", "B1", "B2",
	"C1", "C2", "C3", "C4",
	"D1", "D1", "D1", "D1",
	"E1", "E2", "E1", "E2",
	"F1", "F2", "F3", "F4",
	"G1", "G2", "G3", "G4",
	"H1", "H2", "H3", "H4",
	"I1", "I2", "I3", "I4"
};

cell dummy;
cell devices[][4] = {
	{
		cell(SIMPLE, 1,1,1,1, 4), dummy, dummy, dummy // A1
	},
	{
		cell(SIMPLE, 0,1,0,1, 2), // B1
		cell(SIMPLE, 1,0,1,0, 2), dummy, dummy // B2
	},
	{
		cell(SIMPLE, 0,1,1,0, 1), // C1
		cell(SIMPLE, 0,0,1,1, 1), // C2
		cell(SIMPLE, 1,0,0,1, 1), // C2
		cell(SIMPLE, 1,1,0,0, 1) // C4
	},
	{
		cell(SOURCE, 1,1,1,1, 4), dummy, dummy, dummy // D1
	},
	{
		cell(SOURCE, 0,1,0,1, 2), // E1
		cell(SOURCE, 1,0,1,0, 2), dummy, dummy // E2
	},
	{
		cell(SOURCE, 0,1,1,0, 1), // F1
		cell(SOURCE, 0,0,1,1, 1), // F2
		cell(SOURCE, 1,0,0,1, 1), // F2
		cell(SOURCE, 1,1,0,0, 1) // F4
	},
	{
		cell(SIMPLE, 0,1,1,1, 1), // G1,
		cell(SIMPLE, 1,0,1,1, 1), // G2,
		cell(SIMPLE, 1,1,0,1, 1), // G3,
		cell(SIMPLE, 1,1,1,0, 1) // G4
	},
	{
		cell(SOURCE, 0,1,1,1, 1), // H1,
		cell(SOURCE, 1,0,1,1, 1), // H2,
		cell(SOURCE, 1,1,0,1, 1), // H3,
		cell(SOURCE, 1,1,1,0, 1) // H4
	},
	{
		cell(LIGHT, 0,1,0,0, 1), // I1
		cell(LIGHT, 0,0,1,0, 1), // I2
		cell(LIGHT, 0,0,0,1, 1), // I3
		cell(LIGHT, 1,0,0,0, 1) // I4
	}
};
cell ***board;

int main() {
	int i,j;
	char input[3];

	// READ INPUT: first line (description of matrix)
	scanf("%d%d",&X,&Y); // kolumn, wierszy

	// INIT: initialize board
	board = new cell**[Y];
	board[0] = new cell*[X*Y];
	for(i=1, j=X; i<Y; ++i, j+=X)
		board[i] = board[0] + j;

	// READ INPUT: rest
	j=X*Y; // assert that mult. exactly once
	cell **ptr = &(board[0][0]);
	for(i=0; i<j; ++i, ++ptr) {
		scanf(" %s ", input);
		(*ptr) = &(devices[input[0]-'A'][input[1]-'1']);
	}

	// int** a = new int*[Y];
	// a[0] = new int[X * Y];
	// for (int i = 1; i < Y; i++)
	// 	a[i] = a[0] + i * X;

	// for(int i=0; i<Y; ++i)
	// 	for(int j=0; j<X; ++j)
	// 		a[i][j] = i*j;

	// for(int i=0; i<Y; ++i) {
	// 	for(int j=0; j<X; ++j)
	// 		printf("%4d", a[i][j]);
	// 	printf("\n");
	// }

	return 0;
}
