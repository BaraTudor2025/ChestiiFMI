#include <iostream>

struct coada {
	int info;
	coada* prev = nullptr;
	coada* next = nullptr;
};

coada* g_head = nullptr;
coada* g_tail = nullptr;

void push_left(int n)
{
	if (!g_head) {
		g_tail = g_head = new coada{n, nullptr, g_tail};
	} else {
		coada* nou = new coada{n, nullptr, g_head};
		g_head->prev = nou;
		g_head = nou;
	}
}

void push_right(int n)
{
	if (!g_tail) {
		g_head = g_tail = new coada{n, g_head, nullptr};
	} else {
		coada* nou = new coada{n, g_tail, nullptr};
		g_tail->next = nou;
		g_tail = nou;
	}
}

int pop_left()
{
	if (g_head) {
		coada* del = g_head;
		if(g_head = g_head->next)
			g_head->prev = nullptr;
		else
			g_tail = g_head = nullptr;
		int i = del->info;
		delete del;
		return i;
	}
	return 0;
}

int pop_right()
{
	if (g_tail) {
		coada* del = g_tail;
		if(g_tail = g_tail->prev)
			g_tail->next = nullptr;
		else
			g_tail = g_head = nullptr;
		int i = del->info;
		delete del;
		return i;
	}
	return 0;
}

void print()
{
	if (!g_head) {
		std::cout << "coada goala\n";
		return;
	}

	for (coada* it = g_head; it != nullptr; it = it->next) {
		std::cout << it->info << ' ';
	}
	std::cout << '\n';
}

int main()
{
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