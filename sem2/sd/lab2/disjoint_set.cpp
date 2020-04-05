#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <algorithm>

template <class _Container>
class insert_iterator { // wrap pushes to back of container as output iterator
public:
    using iterator_category = std::output_iterator_tag;
    using value_type        = void;
    using difference_type   = void;
    using pointer           = void;
    using reference         = void;

    using container_type = _Container;

    explicit insert_iterator(_Container& _Cont) noexcept /* strengthened */ : container(std::addressof(_Cont)) {}

    insert_iterator& operator=(const typename _Container::value_type& _Val) {
        container->insert(_Val);
        return *this;
    }

    insert_iterator& operator=(typename _Container::value_type&& _Val) {
        container->insert(std::move(_Val));
        return *this;
    }

    _NODISCARD insert_iterator& operator*() noexcept /* strengthened */ {
        return *this;
    }

    insert_iterator& operator++() noexcept /* strengthened */ {
        return *this;
    }

    insert_iterator operator++(int) noexcept /* strengthened */ {
        return *this;
    }

protected:
    _Container* container; // pointer to container
};

template <typename T>
class Forest {
	std::unordered_set<T> data;
public:
	Forest() = default;

	template <typename U>
	Forest& add(U&& x)
	{
		data.insert(std::forward<U>(x));
		return *this;
	}

	bool find(const T& x) const
	{
		return data.find(x) != data.end();
	}

	Forest<T> concat(const Forest<T>& other) {
		Forest<T> newForest;
		std::set_union(data.begin(), data.end(), other.data.begin(), other.data.end(), insert_iterator(newForest.data));
		return newForest;
	}

	template <typename F>
	void for_each(F f)
	{
		for (auto& x : data)
			f(x);
	}
};

template <typename T>
void print_elem(const T& x)
{
	std::cout << x << ' ';
}

template <typename T>
class DisjointSet {
	std::map<T, int> elements;
	static inline int sGenSetID = 0;
	static inline int const invalidParent = -1;

public:
	DisjointSet() = default;

	int find(T x) const
	{
		auto it = elements.find(x);
		return it != elements.end() ? it->second : invalidParent;
	}

	void make_union(T x, T y)
	{
		int s1 = find(x);
		int s2 = find(y);
		if (s1 == invalidParent && s2 == invalidParent) {
			//std::cout << sGenSetID << ":";
			//printf("x:(%d,%d) ", x.first, x.second);
			//printf("y:(%d,%d) ", y.first, y.second);
			//std::cout << '\n';
			elements[x] = elements[y] = sGenSetID;
			sGenSetID += 1;
		} else if (s1 != invalidParent && s2 == invalidParent) {
			elements[y] = elements[x];
		} else if (s1 == invalidParent && s2 != invalidParent) {
			elements[x] = elements[y];
		} else {
			for (auto& [elem, setID] : elements)
				if (setID == s1)
					setID = s2;
			}
	}

	auto getSets() const -> std::vector<std::vector<T>>
	{
		std::vector<std::vector<T>> sets(sGenSetID);
		for (const auto& [elem, setID] : elements)
			sets[setID].push_back(elem);
		return sets;
	}
};

int main()
{
	//std::cout << '\n';
	//auto sets = set.getSets();
	//for (const auto& set : sets) {
	//	for (const auto& elem : set)
	//		printf("(%d,%d) ", elem.first, elem.second);
	//		//std::cout << elem.first << ' ' << elem.second << ' ';
	//	std::cout << '\n';
	//}
	std::stringstream fin(R"(
	7
	0 0 1 0 0 0 0
	0 0 1 1 0 0 0
	0 0 0 0 1 0 0
	0 0 0 1 1 0 0
	0 1 0 0 1 0 1
	1 1 1 0 0 0 1
	1 1 1 0 0 1 1
	)");
	int n;
	fin >> n;
	int mat[10][10];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			fin >> mat[i][j];

	DisjointSet<std::pair<int, int>> set;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (mat[i][j] == 1) {
				bool ok = false;
				if (i + 1 < n && mat[i + 1][j] == 1) {
					set.make_union({i, j}, {i + 1, j});
					ok = true;
				} if (j + 1 < n && mat[i][j + 1] == 1) {
					set.make_union({i, j}, {i, j + 1});
					ok = true;
				} if(!ok) {
					set.make_union({i, j}, {i, j});
				}
			} 
		}
	}
	int mark = 2;
	auto sets = set.getSets();
	for (const auto& set : sets) {
		for (const auto& coord : set)
			mat[coord.first][coord.second] = mark;
		mark += 1;
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			if (mat[i][j] == 0)
				std::cout << "  ";
			else
				std::cout << mat[i][j] << ' ';
		std::cout << '\n';
	}
}