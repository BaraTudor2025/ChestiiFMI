#include <iostream>
#include <sstream>
#include <vector>

int main()
{
	// pb 2.5
	std::stringstream fin(R"(
	2
	8
	1 1 2 2 2 1 2 1
	9
	1 1 2 2 2 1 2 1 2
	)");
	int cases;
	fin >> cases;
	std::vector<int> vals;

	while (cases --> 0) {
		int n, val;
		int candidat = 0, count = 0;
		fin >> n;
		for (int i = 0; i < n; i++) {
			fin >> val;
			vals.push_back(val);
			if (count == 0) {
				candidat = val;
				count = 1;
			} else if (candidat == val)
				count += 1;
			else
				count -= 1;
		}

		int aparitii = 0;
		for (int i = 0; i < n; i++)
			if (candidat == vals[i])
				aparitii += 1;

		if (aparitii > n / 2)
			std::cout << "majoritar: " << candidat;
		else
			std::cout << "nu-i majoritar";
		std::cout << '\n';
		vals.clear();
	}
}