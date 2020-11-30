//module;
#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <initializer_list>
#include <iostream>
#include <stack>

//export module Graf;

//export namespace Graf
//{


enum class GrafType : std::int8_t {
	Matrix,
	ListVector,
	ListSet
};

struct GrafConfig {
	GrafType type;
	bool one_index;
	bool orientat;
};

class IGraf {
public:
	IGraf(int nrNoduri, int nrMuchii, bool one_index, GrafType type) 
		: type(type), nr_noduri(nrNoduri + (int)one_index), nr_muchii(nrMuchii), one_index(one_index), start_index(one_index)
	{ }
	virtual ~IGraf() = default;

	// poate sa fie cu 1 mai mult pentru 1-index, asa ca foloseste actual_nr_noduri
	const int nr_noduri;
	const int nr_muchii;
	const bool one_index;
	const int start_index;
	const GrafType type;
	//const bool orientat;

	//int actual_nr_noduri() const;
	int actual_nr_noduri() const { return nr_noduri - start_index; }

	virtual void connect(int n1, int n2) = 0;
	virtual bool is_connected(int n1, int n2) const = 0;
	virtual void for_neighbours(int n, std::function<void(int)> f) const = 0;
	//virtual void add_node(int n) = 0;
	//virtual void hint_nr_edges(int n) = 0;

	void for_nodes(std::function<void(int)> f) const
	{
		for (int i = start_index; i < nr_noduri; i++)
			f(i);
	}
};

constexpr auto UNREACHABLE_NODE = std::numeric_limits<int>::max();

class IGrafPonderat : public IGraf{
public:

	IGrafPonderat(int nrNoduri, int nrMuchii, bool one_index, GrafType type) 
		: IGraf(nrNoduri, nrMuchii, one_index, type) { }
	virtual ~IGrafPonderat() = default;

	virtual void connect(int n1, int n2, int p) = 0;
	virtual bool is_connected(int n1, int n2) const = 0;
	virtual int pondere(int n1, int n2) const = 0;
	virtual void for_neighbours(int n, std::function<void(int, int)> f) const = 0;

	void for_neighbours(int n, std::function<void(int)> f) const final {
		for_neighbours(n, [&](int i, int p) { f(i); });
	}

private:
	void connect(int n1, int n2) final {
		throw std::runtime_error("functia gresita 'connect' pentru graf ponderat");
	}
};

auto readGraf(std::string file, GrafConfig config) -> std::unique_ptr<IGraf>;
auto makeGraf(int nrNoduri, int nrMuchii, bool oneIndex, GrafType type) -> std::unique_ptr<IGraf>;

auto readGrafPonderat(std::string file, GrafConfig config)->std::unique_ptr<IGrafPonderat>;
auto makeGrafPonderat(int nrNoduri, int nrMuchii, bool oneIndex, GrafType type)->std::unique_ptr<IGrafPonderat>;

auto transposeGraf(IGraf* g) -> std::unique_ptr<IGraf>;
auto transposeGrafPonderat(IGrafPonderat* g) -> std::unique_ptr<IGrafPonderat>;

template<typename T>
void printG(IGraf* g, const T& vec)
{
	for (int i = g->start_index; i < g->nr_noduri; i++)
		std::cout << vec[i] << ' ';
	std::cout << '\n';
}
template<typename T>
void print(T& vec)
{
	for (auto&& e : vec)
		std::cout << e << ' ';
	std::cout << '\n';
}

inline void make_graf_to_from(IGraf* to, const IGraf* from)
{
	for (int i = from->start_index; i < from->nr_noduri; i++) {
		from->for_neighbours(i, [&](int n) {
			to->connect(i, n);
		});
	}
}

inline void print_muchii(IGraf* g)
{
	g->for_nodes([&](int i) {
		g->for_neighbours(i, [&](int n) {
			std::cout << i << ' ' << n << '\n';
		});
	});
}


inline void print_link(int to, int from, const std::vector<int>& parent)
{
	if (to != from && from != -1) {
		print_link(to, parent[from], parent);
		std::cout << from << ' ';
	}
}


static int reprez(std::vector<int>& tata, int u)
{
	if (tata[u] == 0)
		return u;
	tata[u] = reprez(tata, tata[u]);
	return tata[u];
}


struct SearchVisitor {
	virtual void start(int p) {}
	virtual void in(int p) {}
	virtual void finalize(int p) {}
	virtual void pre(int p, int n) {}
	virtual void post(int p, int n) {} // for dfs
	virtual void already_visited(int p, int n) {}
	virtual ~SearchVisitor() = default;
};

//struct SearchVisitorPonderat2 : SearchVisitor {
//	virtual void start(int p) {}
//	virtual void in(int p) {}
//	virtual void finalize(int p) {}
//	virtual void pre(int a, int b, int p) {}
//	virtual void post(int a, int b, int p) {} // for dfs
//	virtual void already_visited(int a, int b, int p) {}
//
//	virtual void pre(int p, int n) final { throw std::runtime_error("cant use ne-ponderat pre"); }
//	virtual void post(int p, int n) final { throw std::runtime_error("cant use ne-ponderat post"); } // for dfs
//	virtual void already_visited(int p, int n) final { throw std::runtime_error("cant use ne-ponderat already_visited"); }
//
//	virtual ~SearchVisitorPonderat2() = default;
//};

struct SearchVisitorPonderat {
	virtual void start(int p) {}
	virtual void in(int p) {}
	virtual void pre(int a, int b, int p) {}
	virtual void post(int a, int b, int p) {} // for dfs
	virtual void already_visited(int a, int b, int p) {}
	virtual void finalize(int p) {}
	virtual ~SearchVisitorPonderat() = default;
};

struct MakeSVPonderat : SearchVisitorPonderat {
	SearchVisitor& v;
	MakeSVPonderat(SearchVisitor& v) : v(v) {}
	void start(int p) override { v.start(p); }
	void in(int p) override { v.in(p); }
	void pre(int a, int b, int p) override { v.pre(a, b); }
	void post(int a, int b, int p) override { v.post(a, b); } // for dfs
	void already_visited(int a, int b, int p) override { v.already_visited(a, b); }
	void finalize(int p) override { v.finalize(p); }
};

template<typename T>
struct MakeSVPonderatComp : SearchVisitorPonderat {
	T v;
	template <typename... Ts>
	MakeSVPonderatComp(Ts&&... args) : v(std::forward<Ts>(args...)) {}
	void start(int p) override { v.start(p); }
	void in(int p) override { v.in(p); }
	void pre(int a, int b, int p) override { v.pre(a, b); }
	void post(int a, int b, int p) override { v.post(a, b); } // for dfs
	void already_visited(int a, int b, int p) override { v.already_visited(a, b); }
	void finalize(int p) override { v.finalize(p); }
};


void breadth_first_search(IGraf* g, SearchVisitor& visitor, std::vector<bool>& viz, int start_node = -1);
void depth_first_search(IGraf* g, SearchVisitor& visitor, std::vector<bool>& viz, int start_node = -1);

using SearchVisitorList = std::initializer_list<std::reference_wrapper<SearchVisitor>>;
void depth_first_search(IGraf* g, SearchVisitorList list, int start_node = -1);
void depth_first_search(IGraf* g, SearchVisitorList list, std::vector<bool>& viz, int start_node = -1);
void breadth_first_search(IGraf* g, SearchVisitorList list, int start_node = -1);
void breadth_first_search(IGraf* g, SearchVisitorList list, std::vector<bool>& viz, int start_node = -1);

void depth_first_search(IGrafPonderat* g, SearchVisitorPonderat& visitor, std::vector<bool>& viz, int start_node = -1);
void breadth_first_search(IGrafPonderat* g, SearchVisitorPonderat& visitor, std::vector<bool>& viz, int start_node = -1);

using SearchVisitorListPonderat = std::initializer_list<std::reference_wrapper<SearchVisitorPonderat>>;
void depth_first_search(IGrafPonderat* g, SearchVisitorListPonderat list, int start_node = -1);
void depth_first_search(IGrafPonderat* g, SearchVisitorListPonderat list, std::vector<bool>& viz, int start_node = -1);
void breadth_first_search(IGrafPonderat* g, SearchVisitorListPonderat list, int start_node = -1);
void breadth_first_search(IGrafPonderat* g, SearchVisitorListPonderat list, std::vector<bool>& viz, int start_node = -1);

struct SVNull : SearchVisitor {};

struct SVPath : SearchVisitor {
	std::vector<int> path;
	void in(int n) override
	{
		path.push_back(n);
	}
};

struct SVParents : SearchVisitor {
	std::vector<int> parent;
	SVParents(const IGraf* g) : parent(g->nr_noduri, -1) { }
	void pre(int p, int n) override
	{
		parent[n] = p;
	}
};

struct SVDistances : SearchVisitor {
	std::vector<int> distance;
	const int start_index;
	SVDistances(const IGraf* g) : distance(g->nr_noduri, -1), start_index(g->start_index) { }
	void start(int n) override
	{
		distance[n] = 0;
	}
	void pre(int p, int n) override
	{
		distance[n] = distance[p] + 1;
	}
};

struct SVIsConex : SearchVisitor {
	SVIsConex(IGraf* g) : g(g) {}
	void in(int n) override
	{
		visitedNum++;
	}
	bool isConex() const { return visitedNum == g->actual_nr_noduri(); }
private:
	int visitedNum = 0;
	IGraf* g;
};

struct SVFinalized : SearchVisitor {
	std::vector<bool> finalized;
	SVFinalized(IGraf* g) : finalized(g->nr_noduri, false) {}
	void finalize(int n) override
	{
		finalized[n] = true;
	}
};

struct SVFinalizedStack : SearchVisitor {
	std::stack<int> stack;
	void finalize(int n) override
	{
		stack.push(n);
	}
};

struct SVLowPath : SearchVisitor {
	SVParents svp;
	std::vector<int> nr;
	std::vector<int> nrMin;
	int nrdf = 0;
	SVLowPath(const IGraf* g) : svp(g), nr(g->nr_noduri), nrMin(g->nr_noduri)  {}
	void in(int n) override
	{
		nrMin[n] = nr[n] = ++nrdf;
	}
	void pre(int p, int n) override
	{
		svp.pre(p, n);
	}
	void post(int p, int n) override
	{
		nrMin[p] = std::min(nrMin[n], nrMin[p]);
	}
	void already_visited(int p, int n) override
	{
		if(svp.parent[p] != n)
			nrMin[p] = std::min(nrMin[p], nr[n]);
	}
};

//}
