#include <iostream>
#include <fstream>
#include <sstream>

struct stiva {
	int info;
	stiva* next;
}*g_head = nullptr;

void push(int n)
{
	stiva* nou = new stiva{n, g_head};
	g_head = nou;
}

bool is_empty() { return !g_head; }

int pop()
{
	if (!is_empty()) {
		stiva* head = g_head;
		g_head = g_head->next;
		int info = head->info;
		delete head;
		return info;
	}
	return -1;
}

int top()
{
	if(g_head)
		return g_head->info;
	return -1;
}

void print()
{
	for (stiva* it = g_head; it != nullptr; it = it->next) {
		std::cout << it->info << ' ';
	}
	std::cout << '\n';
}

void clear()
{
	while (!is_empty())
		pop();
}

int main()
{

	std::cout << "\npb 2.1\n";
	push(1);
	pop();
	push(2);
	push(3);
	print();
	clear();

	std::cout << "\npb 2.3\n";
	std::stringstream fin(R"(
	3
	8
	1 2 2 1 3 3 4 4
	12
	1 2 3 4 4 5 6 6 5 3 2 1
	8
	1 2 2 3 1 4 3 4
	)");
	int cases;
	fin >> cases;
	while (cases --> 0) {
		int n, nou;
		fin >> n;
		for (int i = 0; i < n; i++) {
			fin >> nou;
			if (top() == nou)
				pop();
			else
				push(nou);
		}
		if (is_empty())
			std::cout << "valid\n";
		else
			std::cout << "invalid\n";
		clear();
	}
}