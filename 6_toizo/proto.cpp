#include <cstdio>

int X, Y;

enum cell_type { simple, source, light };

struct cell {
	enum cell_type type;
	char wire[4]; // top right down left <=> NESW
};

int main() {
	scanf("%d%d",&X,&Y);

	return 0;
}
