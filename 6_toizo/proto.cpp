#include <cstdio>

enum cell_type { SIMPLE, SOURCE, LIGHT };

struct cell {
	cell(cell_type _type, char _w0, char _w1, char _w2, char _w3, char _name)
			: type(_type), name(_name) {
		wire[0] = _w0;
		wire[1] = _w1;
		wire[2] = _w2;
		wire[3] = _w3;
	}
	cell() : type(SIMPLE), name(0) {
		wire[0] = wire[1] = wire[2] = wire[3] = 0;
	}

	enum cell_type type;
	char wire[4]; // top right down left <=> NESW
	char name;
};

cell devices[] = {
	cell(SIMPLE, 1,1,1,1, 'A'),
	cell(SIMPLE, 0,1,0,1, 'B'),
	cell(SIMPLE, 0,1,1,0, 'C'),
	cell(SOURCE, 1,1,1,1, 'D'),
	cell(SOURCE, 0,1,0,1, 'E'),
	cell(SOURCE, 0,1,1,0, 'F'),
	cell(SIMPLE, 0,1,1,1, 'G'),
	cell(SOURCE, 0,1,1,1, 'H'),
	cell(LIGHT , 0,1,0,0, 'I'),
};

int X, Y;
cell ***board;
int** rots;

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

	rots = new int*[Y];
	rots[0] = new int[X * Y];
	for (i = 1; i < Y; i++)
		rots[i] = rots[0] + i * X;

	// READ INPUT: rest
	j=X*Y; // assert that mult. exactly once
	cell **ptr_board = &(board[0][0]);
	int *ptr_rots = &(rots[0][0]);
	for(i=0; i<j; ++i, ++ptr_board, ++ptr_rots) {
		scanf(" %s ", input);
		(*ptr_board) = &(devices[input[0]-'A']);
		(*ptr_rots) = input[1]-'1';
	}

	// PRINT OUTPUT
	printf("%d %d", X, Y);
	for(i=0; i<Y; ++i) {
		printf("\n%c%d", board[i][0]->name, rots[i][0] + 1);
		for(j=1; j<X; ++j) 
			printf(" %c%d", board[i][j]->name, (rots[i][j]) + 1);
	}
	printf("\n");

	return 0;
}
