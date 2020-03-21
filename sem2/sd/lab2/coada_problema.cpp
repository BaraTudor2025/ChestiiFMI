#include <iostream>
#include <sstream>

struct coada {
	int x, y;
	coada* prev = nullptr;
	coada* next = nullptr;
};

coada* g_head = nullptr;
coada* g_tail = nullptr;

void push_left(int a, int b)
{
	if (!g_head) {
		g_tail = g_head = new coada{a, b, nullptr, g_tail};
	} else {
		coada* nou = new coada{a, b, nullptr, g_head};
		g_head->prev = nou;
		g_head = nou;
	}
}

void push_right(int a, int b)
{
	if (!g_tail) {
		g_head = g_tail = new coada{a, b, g_head, nullptr};
	} else {
		coada* nou = new coada{a, b, g_tail, nullptr};
		g_tail->next = nou;
		g_tail = nou;
	}
}

std::pair<int, int> pop_left()
{
	if (g_head) {
		coada* del = g_head;
		if(g_head = g_head->next)
			g_head->prev = nullptr;
		else
			g_tail = g_head = nullptr;
		auto a = del->x;
		int b = del->y;
		delete del;
		return {a, b};
	}
	return {-1, -1};
}

std::pair<int, int> pop_right()
{
	if (g_tail) {
		coada* del = g_tail;
		if(g_tail = g_tail->prev)
			g_tail->next = nullptr;
		else
			g_tail = g_head = nullptr;
		int x = del->x;
		int y = del->y;
		delete del;
		return {x, y};
	}
	return {-1, -1};
}

bool is_empty() { return !g_head; }

void print()
{
	if (!g_head) {
		std::cout << "coada goala\n";
		return;
	}

	for (coada* it = g_head; it != nullptr; it = it->next) {
		printf("(%d, %d) ", it->x, it->y);
	}
	std::cout << '\n';
}

void markVecini(int mat[10][10], int i, int j, int mark)
{
	if (mat[i + 1][j] == 1) {
		push_right(i + 1, j);
		mat[i + 1][j] = mark;
	} if (mat[i][j + 1] == 1) {
		push_right(i, j + 1);
		mat[i][j + 1] = mark;
	} if (j > 0 && mat[i][j - 1] == 1) {
		push_right(i, j - 1);
		mat[i][j - 1] = mark;
	}
}

int main()
{
	std::stringstream fin(R"(
	7
	0 0 1 0 0 0 0
	0 0 1 1 0 0 0
	0 0 0 0 1 0 0
	0 0 0 1 1 0 0
	1 1 0 0 1 0 1
	1 1 1 0 0 0 1
	1 1 1 0 0 1 1
	)");

	int n;
	fin >> n;
	int mat[10][10];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			fin >> mat[i][j];

	int mark = 2;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (mat[i][j] == 1) {
				mat[i][j] = mark;
				markVecini(mat, i, j, mark);
				while (!is_empty()) {
					auto [x, y] = pop_left();
					markVecini(mat, x, y, mark);
				}
				mark += 1;
			} 
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			if (mat[i][j] == 0)
				std::cout << "  ";
			else
				std::cout << mat[i][j] << ' ';
		std::cout << '\n';
	}

	/*
	push_left(1);
	push_right(2);
	pop_right();
	pop_left();
	push_right(3);
	print();
	*/

	/*
	push_right(1);
	push_left(5);
	push_right(2);
	print();
	pop_right();
	print();
	pop_left();
	print();
	pop_right();
	print();
	*/
}